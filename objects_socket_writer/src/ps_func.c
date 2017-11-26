#include"ps_func.h"
int  ps_socket_send(ps_socket *socket, unsigned char *buffer)
{
	int ret = DTC_NONE;
	unsigned long buffer_size = 0;
    unsigned long bytes_written = 0;
    
    
    // zero
    memset( buffer, 0, sizeof(buffer) );
    
    // copy bytes into buffer
    strcpy( buffer, "message from socket write");
    
    // set buffer size
    buffer_size = strlen(buffer) + 1;

	printf( "writing socket buffer '%s' - %lu bytes\n",
            buffer,
            buffer_size );

    // send data
    ret = psync_socket_send_to(
            socket,
            (unsigned char*) buffer,
            buffer_size,
            &bytes_written );
	return ret;
}


void ps_printf( const ps_msg_ref const message )
{
	// cast to message
    const ps_objects_msg * const objects_msg = (ps_objects_msg*) message;

    // print details
    printf( "received objects message from node 0x%016llX\n", (ps_ull) objects_msg->header.src_guid );
    printf( "received timestamp 0x%016llX\n", (ps_ull) objects_msg->header.timestamp);
	
    unsigned long objects_index = 0;
    const ps_object *_buffer = objects_msg->objects._buffer;
    
    while( objects_index < objects_msg->objects._length )
    {

        printf( "-------------------Publishers objects--------------------\n");
        printf( "Id = 0x%016llX\n", (ps_ull) _buffer[objects_index].id);
		printf( "Position:\nx = %016lf\ny = %016lf\nz = %016lf\n",
                (double) _buffer[objects_index].position[0],
				(double) _buffer[objects_index].position[1],
				(double) _buffer[objects_index].position[2]);
				
		printf( "Size:\nx = %016lf\ny = %016lf\nz = %016lf\n",
                (double) _buffer[objects_index].size[0],
				(double) _buffer[objects_index].size[1],
				(double) _buffer[objects_index].size[2]);
		
		printf( "Velocity:\nx = %016lf\ny = %016lf\nz = %016lf\n",
                (double) _buffer[objects_index].velocity[0],
				(double) _buffer[objects_index].velocity[1],
				(double) _buffer[objects_index].velocity[2]);
		
		printf( "Course_angle = %016lf\n", (ps_ull) _buffer[objects_index].course_angle);
		
		printf( "Classification = %s\n", CLASIFICATION[(int) _buffer[objects_index].classification]);
		
		printf( "Classification quality = %d\n", (ps_ull) _buffer[objects_index].classification_quality);
		
        objects_index++;
        
    }

    printf( "\n\n" );
}

void ps_socket_error(ps_socket* socket)
{
	// check reference since other routines don't
    if( socket == NULL )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- invalid socket",
                __FILE__,
                __LINE__ );

        //psync_node_activate_fault( node_ref, DTC_USAGE, NODE_STATE_FATAL );
        return;
    }
}

void ps_socket_send_error(int ret)
{
	 // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_socket_send_to returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

       // psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
}

void ps_socket_init_error(int ret)
{
	// activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_socket_init returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        //psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
}

void ps_socket_set_address_error(int ret)
{
	// activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_socket_set_address returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        //psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
}


void ps_socket_set_reuse_option_error(int ret)
{
	// activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_socket_set_reuse_option returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        //psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
}

void ps_message_get_type_by_name_error(int ret)
{
	  // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_message_get_type_by_name returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        //psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
}

void ps_message_register_listener_error(int ret)
{
  // activate fatal error and return if failed
    if( ret != DTC_NONE )
    {
        psync_log_message(
                LOG_LEVEL_ERROR,
                "%s : (%u) -- psync_message_register_listener returned DTC %d",
                __FILE__,
                __LINE__,
                ret );

        //psync_node_activate_fault( node_ref, ret, NODE_STATE_FATAL );
        return;
    }
}




