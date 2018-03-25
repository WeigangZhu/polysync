#include"ps_path_planning.h"



int *path_left_right_objects_fsae(const ps_msg_ref const message, 
						 double left_objects[OBJECT_NUMBER][2],
						 double right_objects[OBJECT_NUMBER][2])
{
	// cast to message
    const ps_objects_msg * const objects_msg = (ps_objects_msg*) message;
    
    unsigned long objects_index = 0;
    const ps_object *_buffer = objects_msg->objects._buffer;
    int left_index = 0;
    int right_index = 0;
    
    while( objects_index < objects_msg->objects._length )
    {
		if(_buffer[objects_index].position[1] < 0)
		{
			left_objects[left_index][0] = _buffer[objects_index].position[0];
			left_objects[left_index][1] = _buffer[objects_index].position[1];
			left_index++;
		}
		else
		{
			right_objects[right_index][0] = _buffer[objects_index].position[0];
			right_objects[right_index][1] = _buffer[objects_index].position[1];
			right_index++;
		}
		
        objects_index++;
        
    }
    
    int re[2] = {left_index, right_index};
    return re;
}


void *staight_line_fsae(
			double left_objects[OBJECT_NUMBER][2],
			double right_objects[OBJECT_NUMBER][2],
			int left_right_length[2])
{
	if (left_right_length[0] < 3 && left_right_length[1] < 3)
	{
		printf(" the objects of left and right are no more than 3\n");
		exit(1);
	}
	
	
	
	double sum_left_x = 0.0;
	double sum_left_y = 0.0;
	double sum_left_xy = 0.0;
	
	double sum_right_x = 0.0;
	double sum_right_y = 0.0;
	double sum_right_xy = 0.0;
	
	double sum_left_x2 = 0.0;
	double sum_right_x2 = 0.0;
	
	for(int i = 0; i < left_right[0]; i++)
	{
		sum_left_x += left_objects[i][0];
		sum_left_y += left_objects[i][1];
		sum_left_x2 += left_objects[i][0] * left_objects[i][0];
		sum_left_xy += left_objects[i][0] * left_objects[i][1];
		
	}
	
	for(int i = 0; i < left_right[1]; i++)
	{
		sum_right_x += right_objects[i][0];
		sum_right_y += right_objects[i][1];
		sum_right_x2 += right_objects[i][0] * right_objects[i][0];
		sum_right_xy += right_objects[i][0] * right_objects[i][1];
	}
	
	double k_left = (sum_left_xy -sum_left_x*sum_left_y/left_right_length[0])/ 
					(sum_left_x2-sum_left_x * sum_left_x /left_right[0]);
	double b_left = sum_left_y/left_right_length[0] - 
					k_left * sum_left_x / left_right_length[0];
	
	double k_right = (sum_right_xy -sum_right_x*sum_right_y/left_right_length[0])/ 
					(sum_right_x2-sum_right_x * sum_right_x /left_right_length[0]);
	double b_right = sum_right_y/left_right_length[0] - 
					k_left * sum_right_x / left_right_length[0];
	

	
}

void *cube_line_fsae(const ps_msg_ref const message)
{
	int re[2];
	double left_objects[OBJECT_NUMBER][2],
	double right_objects[OBJECT_NUMBER][2]
	
	re = staight_line_fsae(message, left_objects, right_objects);
	
}

double cube_insert_fsae(double point[3])
{
	
}
		
