#include"ps_control.h"
#include<math.h>


// PID process, return control variables
void AEB_pid(velocity_error_t *vel, distance_error_t *dis, velocity_distance_error_t *vel_dis)
{
	double PID_A0 = PID_P*( 1 + PERIOD/PID_I + PID_D/PERIOD);
	double PID_A1 = -PID_P*( 1 + 2 * PID_D/PERIOD);
	double PID_A2 = -PID_P * PID_D/PERIOD;
	
	vel_dis->error_velocity = 
			PID_A0 * vel->error[2] + 
			PID_A1 * vel->error[1] +
			PID_A2 * vel->error[0];
		
	vel_dis->error_distance = 
			PID_A0 * dis->error[2] + 
			PID_A1 * dis->error[1] +
			PID_A2 * dis->error[0];
}

// testing whether receive four data, in order to active PID program
int is_receive_four(int num)
{
	if (num >0 && num % 3 ==0 )
	{
		//*num = 0;
		return 1;
	}
	else 
		return 0;
}

// judge whether the objects is in front
int is_object_front( double y)
{
	if(abs(y) < CAR_WIDTH / 2)
		return 1;
	else
		return 0;
}

// testing whether the velocity is right
int is_velocity_right(double x)
{
	if( x < 0 || x >100)
		return 0;
	else 
		return 1;
}

// return the relative velocity of objects with car
double return_velocity(double x, double y)
{
	int temp_x = is_velocity_right(x);
	int temp_y = is_velocity_right(y);
	
	return sqrt(x*x*temp_x + y*y*temp_y);
}

void ps_memory(velocity_distance_t *data, 
			   velocity_distance_error_t *vel_dis,
			   velocity_error_t *vel_err,
			   distance_error_t *dis_err)
{
	data = (struct  velocity_distance*)malloc(sizeof(struct  velocity_distance));
    
    vel_dis = (struct  velocity_distance_error*)malloc(sizeof(struct  velocity_distance_error))  ;
    
    vel_err = (struct  velocity_error*)malloc(sizeof(struct  velocity_error));
    
    dis_err = (struct  distance_error*)malloc(sizeof(struct  distance_error));
}

void ps_free_memory(velocity_distance_t *data, 
			   velocity_distance_error_t *vel_dis,
			   velocity_error_t *vel_err,
			   distance_error_t *dis_err)
{
	free(data);
	free(vel_dis);
	free(vel_err);
	free(dis_err);
}
	

