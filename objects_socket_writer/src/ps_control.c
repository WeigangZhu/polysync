#include"ps_control.h"
#include<math.h>

void AEB_pid(velocity_error *vel, distance_error_t *dis, velocity_distance_error_t *vel_dis)
{
	vel_dis->velocity = PID_A0 * vel->error_now + PID_A1 * vel->error_pre +PID_A2 * vel->error_pre_sec;
	vel_dis->distance = PID_A0 * dis->error_now + PID_A1 * dis->error_pre +PID_A2 * dis->error_pre_sec;
}


bool is_receive_four(int *num)
{
	if (*num >0 && *num % 4 ==0 )
	{
		*num = 0;
		return 1;
	}
	else 
		return 0;
}

bool is_object_front( double y)
{
	if(abs(y) < CAR_WIDTH / 2)
		return 1;
	else
		return 0;
}
	
	

