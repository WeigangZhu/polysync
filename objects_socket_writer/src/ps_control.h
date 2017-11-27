#ifndef PS_CONTROL_H_
#define PS_CONTROL_H_

static double BRAKE_MAX = 60.0;  //deg
static double VELOCITY_EXPECT = 0.0; // M
static double DISTANCE_EXPECT = 0.5; // M
static double PID_P = 3.0;
static double PID_I = 40.0;
static double PID_D = 3.0;
static double PERIOD = 0.08;
static double CAR_WIDTH = 2.0;


typedef struct velocity_distance_error
{
	double error_velocity;
	double error_distance;
} velocity_distance_error_t;

typedef struct velocity_error
{
	double error[3];
} velocity_error_t;

typedef struct distance_error
{
	double error[3];
} distance_error_t;

typedef struct velocity_distance
{
	double velocity[4];
	double distance[4];
} velocity_distance_t;



void AEB_pid(velocity_error_t *vel, distance_error_t *dis, velocity_distance_error_t *vel_dis);
int is_receive_four(int *num);
int is_object_front( double y);
int is_velocity_right(double x);
double return_velocity(double x, double y);


#endif

