#include"ps_control.h"
#include<math.h>



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

	

