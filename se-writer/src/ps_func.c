#include"ps_func.h"

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
		
		printf( "Course_angle = %016ld\n", (ps_ull) _buffer[objects_index].course_angle);
		
		printf( "Classification = %s\n", CLASIFICATION[(int) _buffer[objects_index].classification]);
		
		printf( "Classification quality = %ld\n", (ps_ull) _buffer[objects_index].classification_quality);
		
        objects_index++;
        
    }

    printf( "\n\n" );
}




