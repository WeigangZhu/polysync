
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// API headers
#include "polysync_core.h"
#include "polysync_node.h"
#include "polysync_sdf.h"
#include "polysync_message.h"
#include "polysync_node_template.h"




// *****************************************************
// static global types/macros
// *****************************************************

#ifndef NODE_FLAGS_VALUE
#define NODE_FLAGS_VALUE (0)
#endif

#define DEBUG_DEAN
#ifdef DEBUG_DEAN

	//#define TESE
	//#define DETAIL
	#define OUTPUT

#endif

static const char NODE_NAME[] = "polysync-publish-subscribe-c";
static const char LIDAR_POINTS_MSG_NAME[] = "ps_lidar_points_msg";
static const char POINTS_MSG_NAME[] = "ps_lidar_points_msg";
typedef struct
{
    //
    //
    ps_msg_ref radar_targets_msg; /*!< 'ps_radar_targets_msg' message. */
    //
    //
    ps_msg_ref lidar_points_msg; /*!< 'ps_lidar_points_msg' message. */
    //
    //
    ps_msg_ref objects_msg; /*!< 'ps_objects_msg' message. */
} node_data_s;



static void ps_lidar_points_msg__handler(const ps_msg_type msg_type, const ps_msg_ref const message, void * const user_data );
static int  set_configuration(ps_node_configuration_data * const node_config );
static void on_init(ps_node_ref const node_ref, const ps_diagnostic_state * const state, void * const user_data );
static void on_release(ps_node_ref const node_ref, const ps_diagnostic_state * const state, void * const user_data );
static void on_error(ps_node_ref const node_ref, const ps_diagnostic_state * const state, void * const user_data );
static void on_fatal(ps_node_ref const node_ref, const ps_diagnostic_state * const state, void * const user_data );
static void on_warn(ps_node_ref const node_ref, const ps_diagnostic_state * const state, void * const user_data );
static void on_ok( ps_node_ref const node_ref, const ps_diagnostic_state * const state, void * const user_data );
void lidar_points_init( const ps_timestamp time, ps_lidar_points_msg * const msg );

// *****************************************************
// static definitions
// *****************************************************


//
static void ps_lidar_points_msg__handler(
        const ps_msg_type msg_type,
        const ps_msg_ref const message,
        void * const user_data )
{
    // cast to message
    const ps_lidar_points_msg * const lidar_points_msg = (ps_lidar_points_msg*) message;

    #ifdef DETAIL
		// print details
    	printf( "  received lidar_points message from node 0x%016llX (%llu)\n",
            	(unsigned long long) lidar_points_msg->header.src_guid,
            	(unsigned long long) lidar_points_msg->header.src_guid );

    	printf( "  received lidar_points message from node 0x%016llX (%llu)\n",
            	(unsigned long long) lidar_points_msg->header.timestamp,
            	(unsigned long long) lidar_points_msg->header.timestamp );
	#endif

    unsigned long lidar_points_index = 0;
    // 
    while( lidar_points_index < lidar_points_msg->points._length )
    {

        const ps_lidar_point *_buffer = lidar_points_msg->points._buffer;

	#ifdef DETAIL
        	printf( "  publishers lidar_points :\n");
        	printf( "  received length = 0x%016llX\n",
               	 	(unsigned long) lidar_points_msg->points._length);
			printf( "  received position:\nx = %016lf\ny = %016lf\nz = %016lf\n",
				(double) _buffer[lidar_points_index].position[0],
				(double) _buffer[lidar_points_index].position[1],
				(double) _buffer[lidar_points_index].position[2]);
	#endif

	#ifdef	OUTPUT

		if(		_buffer[lidar_points_index].position[0] < 15 && 
				abs( _buffer[lidar_points_index].position[1]*10) < 12 && 
				_buffer[lidar_points_index].position[2] > 0)
		
			printf( "%016lf\t%016lf\t%016lf\n",
                		(double) _buffer[lidar_points_index].position[0],
						(double) _buffer[lidar_points_index].position[1],
						(double) _buffer[lidar_points_index].position[2]);
	#endif

        lidar_points_index++;
    }

    printf( "\n" );
    
    
}




//
static int set_configuration(
        ps_node_configuration_data * const node_config )
{
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
	
	// local vars
    node_data_s *node_data = NULL;
    
	// create node data
    if( (node_data = malloc( sizeof(*node_data) )) == NULL )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- failed to allocate node data structure",
                __FILE__,
                __LINE__ );

        return DTC_MEMERR;
    }

    // zero
    memset( node_data, 0, sizeof(*node_data) );

    // set user data pointer to our top-level node data
    // this will get passed around to the various interface routines
    node_config->user_data = (void*) node_data;

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
    ps_msg_type msg_type = PSYNC_MSG_TYPE_INVALID;
	

    // get diagnostic trace message type identifier
    ret = psync_message_get_type_by_name(
            node_ref,
            POINTS_MSG_NAME,
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

    // register subscriber for message
    ret = psync_message_register_listener(
            node_ref,
            msg_type,
            ps_lidar_points_msg__handler,
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
    
    
	node_data_s *node_data = NULL;
     
    // cast
    node_data = (node_data_s*) user_data;
    
     // check reference since other routines don't
    if( node_data == NULL )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- invalid node context",
                __FILE__,
                __LINE__ );

        psync_node_activate_fault( node_ref, DTC_USAGE, NODE_STATE_FATAL );
        return;
    }
    
    // get lidar points message type identifier
    ret = psync_message_get_type_by_name(
            node_ref,
            LIDAR_POINTS_MSG_NAME,
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

    // create lidar points message
    ret = psync_message_alloc(
            node_ref,
            msg_type,
            &node_data->lidar_points_msg );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_message_alloc returned DTC %d",
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
    node_data_s *node_data = NULL;


    // cast
    node_data = (node_data_s*) user_data;

    // if valid
    if( node_data != NULL )
    {
        // free messages
        (void) psync_message_free(
                node_ref,
                (ps_msg_ref*) &node_data->lidar_points_msg );
        // free
        free( node_data );
        node_data = NULL;
    }
    
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
  	int ret = DTC_NONE;
    ps_timestamp current_time = 0;
    node_data_s *node_data = NULL;

    // cast
    node_data = (node_data_s*) user_data;

    // check reference since other routines don't
    if( node_data == NULL )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- invalid node context",
                __FILE__,
                __LINE__ );

        psync_node_activate_fault( node_ref, DTC_USAGE, NODE_STATE_FATAL );
        return;
    }

    // get current timestamp
    ret = psync_get_timestamp( &current_time );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_get_timestamp returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
  	// publish lidar points message
    ret = psync_message_publish(
            node_ref,
            node_data->lidar_points_msg );

    // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_message_publish returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
 
}




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
