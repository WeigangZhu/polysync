#include"ps_func.h"
#include"ps_control.h"

int pid_raw_data_count = 0;
//
ps_socket *my_socket = NULL;

// *****************************************************
// static definitions
// *****************************************************

static void ps_objects_msg__handler(
        const ps_msg_type msg_type,
        const ps_msg_ref const message,
        void * const user_data )
{
    
    // local vars
    int ret = DTC_NONE;
    unsigned char buffer[1248];
   	ps_socket *socket = NULL;

    // cast
    socket = (ps_socket*) my_socket;
	ps_socket_error(socket);
	//
    //ret = ps_socket_send(socket, buffer);
	//ps_socket_send_error(ret);
	//
    //ps_printf(message);
    
    
    const ps_objects_msg * const objects_msg = (ps_objects_msg*) message;
    const ps_object *_buffer = objects_msg->objects._buffer; 
    unsigned long objects_index = 0;
    double distance_min = 1000.0;
    double velocity_now = 0;
    
    while(objects_index < objects_msg->objects._length)
    {
    	double x = _buffer[objects_index].position[0];
    	double y = _buffer[objects_index].position[1];
    	if(is_object_front(y) && x < distance_min)
    	{
    		distance_min = x;
    		velocity_now = 
    			return_velocity(_buffer[objects_index].velocity[0], 
    						    _buffer[objects_index].velocity[0]);
    	
    	
    	}
    	objects_index++;
    }
    
    //printf("%lf\t%lf\n",distance_min, velocity_now);
    
   
 velocity_distance_t *data = (struct  velocity_distance*)malloc(sizeof(struct  velocity_distance));
    
    velocity_distance_error_t *vel_dis = (struct  velocity_distance_error*)malloc(sizeof(struct  velocity_distance_error))  ;
    
    velocity_error_t *vel_err = (struct  velocity_error*)malloc(sizeof(struct  velocity_error));
    
    distance_error_t *dis_err = (struct  distance_error*)malloc(sizeof(struct  distance_error));
 
	if(is_receive_four(pid_raw_data_count))
    {
    	for (int i =1; i <4; i++)
		{
    		vel_err->error[i] = data->velocity[i] - data->velocity[i-1];
			dis_err->error[i] = data->distance[i] - data->distance[i-1];
		}
		pid_raw_data_count = 0;
		
		/*------------------------------------*/
		AEB_pid(vel_err, dis_err, vel_dis );
		printf("%lf\t%lf\n",vel_dis->error_velocity, vel_dis->error_distance);
		/*------------------------------------*/
	}
	else
	{
		data->velocity[pid_raw_data_count] = velocity_now;
		data->distance[pid_raw_data_count] = distance_min;
		pid_raw_data_count++;
	}
    	 
    
    
}


/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/



// *****************************************************
// public definitions
// *****************************************************

//
int main( int argc, char **argv )
{
    // callback data
    ps_node_callbacks callbacks;


    // zero
    memset( &callbacks, 0, sizeof(callbacks) );

    // set callbacks
    callbacks.set_config = &set_configuration;
    callbacks.on_init = &on_init;
    callbacks.on_release = &on_release;
    callbacks.on_warn = &on_warn;
    callbacks.on_error = &on_error;
    callbacks.on_fatal = &on_fatal;
    callbacks.on_ok = &on_ok;


    // use PolySync main entry, this will give execution context to node template machine
    return( psync_node_main_entry( &callbacks, argc, argv ) );
}


//
static int set_configuration(
        ps_node_configuration_data * const node_config )
{
    // local vars
    ps_socket *socket = NULL;


    // set node configuration default values

    // node type
    node_config->node_type = PSYNC_NODE_TYPE_API_USER;

    // set node domain
    node_config->domain_id = PSYNC_DEFAULT_DOMAIN;

    // set node SDF key
    node_config->sdf_key = PSYNC_SDF_ID_INVALID;

    // set node flags
    node_config->flags = NODE_FLAGS_VALUE | PSYNC_INIT_FLAG_STDOUT_LOGGING;

    // set user data
    node_config->user_data = NULL;

    // set node name
    memset( node_config->node_name, 0, sizeof(node_config->node_name) );
    strncpy( node_config->node_name, NODE_NAME, sizeof(node_config->node_name) );

    // create CAN channel
    if( (socket = malloc( sizeof(*socket) )) == NULL )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- failed to allocate socket data structure",
                __FILE__,
                __LINE__ );

        return DTC_MEMERR;
    }

    // zero
    memset( socket, 0, sizeof(*socket) );

    // set user data pointer to our top-level node data
    // this will get passed around to the various interface routines
    node_config->user_data = (void*) socket;

    return DTC_NONE;
}


//
static void on_init(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data )
{
    // local vars
    int ret = DTC_NONE;
    ps_socket *socket = NULL;
    ps_msg_type msg_type = PSYNC_MSG_TYPE_INVALID;

    // cast
    socket = (ps_socket*) user_data;

    // check reference since other routines don't
    ps_socket_error(socket);

    // init UDP socket
    ret = psync_socket_init(
            socket,
            AF_INET,
            SOCK_DGRAM,
            IPPROTO_UDP );

    ps_socket_init_error(ret);

    // set address and port
    ret = psync_socket_set_address(
            socket,
            UDP_ADDRESS,
            UDP_PORT );

    ps_socket_set_address_error(ret);
	
    // set socket reuse option for multiple connections
    ret = psync_socket_set_reuse_option( socket, 1 );
    ps_socket_set_reuse_option_error(ret);


    my_socket = socket;


    // get objects message type identifier
    ret = psync_message_get_type_by_name(
            node_ref,
            OBJECTS_MSG_NAME,
            &msg_type );

    ps_message_get_type_by_name_error(ret);
    
    // register subscriber for objects message
    ret = psync_message_register_listener(
            node_ref,
            msg_type,
            ps_objects_msg__handler,
            NULL );

    ps_message_register_listener_error(ret);
    
    
       
}


//
static void on_release(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data )
{
    // do nothing, sleep for 10 milliseconds
    (void) psync_sleep_micro( 10000 );
    
}


//
static void on_error(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data )
{
    // do nothing, sleep for 10 milliseconds
    (void) psync_sleep_micro( 10000 );
}


//
static void on_fatal(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data )
{
    // do nothing, sleep for 10 milliseconds
    (void) psync_sleep_micro( 10000 );
}


//
static void on_warn(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data )
{
    // do nothing, sleep for 10 milliseconds
    (void) psync_sleep_micro( 10000 );
}


//
static void on_ok(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data )
{

    // sleep for 500 milliseconds
    (void) psync_sleep_micro( 100000 );


}
