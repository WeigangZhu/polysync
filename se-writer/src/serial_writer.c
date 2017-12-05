#include"ps_func.h"
#include"ps_control.h"

ps_serial_device *my_serial_device = NULL;

#define PS_DEBUG
#define PS_SERIAL_SEND
// *****************************************************
// static declarations
// *****************************************************
static void ps_objects_msg__handler(
        const ps_msg_type msg_type,
        const ps_msg_ref const message,
        void * const user_data );

static int set_configuration(
        ps_node_configuration_data * const node_config );


static void on_init(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_release(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_error(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_fatal(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_warn(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_ok(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

// *****************************************************
// static definitions
// *****************************************************

static void ps_objects_msg__handler(
        const ps_msg_type msg_type,
        const ps_msg_ref const message,
        void * const user_data )
{
  	
/*---------------------------------- start SERIAL send ------------------------------------------------*/   
    #ifdef PS_SERIAL_SEND

		unsigned char buffer[10];
		const ps_objects_msg * const objects_msg = (ps_objects_msg*) message;
    	const ps_object *_buffer = objects_msg->objects._buffer; 
    	unsigned long objects_index = 0;
    	double distance_min = 1000.0;
    	double x;
    	double y;
    	while(objects_index < objects_msg->objects._length)
    	{
    		x = _buffer[objects_index].position[0];
    		y = _buffer[objects_index].position[1];
    		if(is_object_front(y) && x < distance_min)
    		{
    			distance_min = x;
    	
    		}
    		objects_index++;
   	 	}
		unsigned char * temp = (unsigned char *) &x;
		for(int i= 0; i < 8; i++)
		{
			buffer[i] = *temp;
			temp++;
		}
		buffer[8] = 255;
		buffer[9] = '\n';
		
	unsigned long buffer_size = 0;
    unsigned long bytes_written = 0;
    ps_serial_device *serial_device = NULL;
	int ret = DTC_NONE;

    // cast
    serial_device = (ps_serial_device*) my_serial_device;

    // check reference since other routines don't
    if( serial_device == NULL )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- invalid serial device",
                __FILE__,
                __LINE__ );

        //psync_node_activate_fault( node_ref, DTC_USAGE, NODE_STATE_FATAL );
        return;
    }

    // set buffer size
    buffer_size = strlen(buffer) + 1;

    printf( "writing serial buffer %lu bytes\n", buffer_size );
	for( int i = 0; i < 10; i++)
		printf("%x\t",buffer[i]);
	printf("\n");
    // write data
    ret = psync_serial_write(
            serial_device,
            (unsigned char*) buffer,
            buffer_size,
            &bytes_written );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_serial_write returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

       // psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }


	#endif //// end if define PS_SERIAL_SEND
	
/*---------------------------------- end UDP send ---------------------------------------------------*/ 	
	
	
/*---------------------------------- start print objects information---------------------------------*/

	#ifdef PS_DEBUG
    	//ps_printf(message);
    #endif // end if define PS_DEBUG
    
/*---------------------------------- end of print objects information--------------------------------*/    
    

    
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
    ps_serial_device *serial_device = NULL;


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
    if( (serial_device = malloc( sizeof(*serial_device) )) == NULL )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- failed to allocate serial device data structure",
                __FILE__,
                __LINE__ );

        return DTC_MEMERR;
    }

    // zero
    memset( serial_device, 0, sizeof(*serial_device) );

    // set user data pointer to our top-level node data
    // this will get passed around to the various interface routines
    node_config->user_data = (void*) serial_device;

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
    ps_serial_device *serial_device = NULL;


    // cast
    serial_device = (ps_serial_device*) user_data;

    // check reference since other routines don't
    if( serial_device == NULL )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- invalid serial device",
                __FILE__,
                __LINE__ );

        psync_node_activate_fault( node_ref, DTC_USAGE, NODE_STATE_FATAL );
        return;
    }

    // init serial device
    ret = psync_serial_init(
            serial_device,
            SERIAL_PORT );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_serial_init returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }

    // open device
    ret = psync_serial_open(
            serial_device );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_serial_open returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }

    // set data rate in the device's cached settings structure
    ret = psync_serial_set_datarate_setting(
            &serial_device->settings,
            SERIAL_DEVICE_DATARATE );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_can_set_bit_rate returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }

    // apply the settings to the device
    ret = psync_serial_apply_settings(
            serial_device,
            &serial_device->settings );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_serial_apply_settings returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
    my_serial_device = serial_device;
    
    ps_msg_type msg_type = PSYNC_MSG_TYPE_INVALID;
    // get objects message type identifier
    ret = psync_message_get_type_by_name(
            node_ref,
            OBJECTS_MSG_NAME,
            &msg_type );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_message_get_type_by_name returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
    
 
    
    // register subscriber for objects message
    ret = psync_message_register_listener(
            node_ref,
            msg_type,
            ps_objects_msg__handler,
            NULL );

    
    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_message_register_listener returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
       
}


//
static void on_release(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data )
{
    // local vars
    ps_serial_device *serial_device = NULL;


    // cast
    serial_device = (ps_serial_device*) user_data;

    // if valid
    if( serial_device != NULL )
    {
        // close device
        (void) psync_serial_close( serial_device );

        // free
        free( serial_device );
        serial_device = NULL;
    }
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
