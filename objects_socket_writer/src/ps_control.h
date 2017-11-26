#ifndef PS_CONTROL_H_
#define PS_CONTROL_H_

const static double BRAKE_MAX = 60.0;  //deg
const static double VELOCITY_EXPECT = 0.0; // M
const static double DISTANCE_EXPECT = 0.5; // M
const static double PID_P = 3.0;
const static double PID_I = 40.0;
const static double PID_D = 3.0;
const static double PERIOD = 0.08;
const static double PID_A0 = PID_P*( 1 + PERIOD/PID_I + PID_D/PERIOD);
const static double PID_A1 = -PID_P*( 1 + 2 * PID_D/PERIOD);
const static double PID_A2 = -PID_P * PID_D/PERIOD;
const static double CAR_WIDTH = 2.0;
typedef struct velocity_distance_error
{
	double error_velocity;
	double error_distance;
} velocity_distance_error_t;

typedef struct velocity_error
{
	double error_pre_sec;
	double error_pre;
	double error_now;
} velocity_error_t;

typedef struct distance_error
{
	double error_pre_sec;
	double error_pre;
	double error_now;
} distance_error_t;

typedef struct velocity_distance
{
	double velocity;
	double distance;
} velocity_distance_t;

typedef struct raw_data
{
	double data_pre_thi;
	double data_pre_sec;
	double data_pre_fir;
	double data_now;
} raw_data_t;

void AEB_pid(velocity_error *vel, distance_error_t *dis, velocity_distance_error_t *vel_dis);
void assert_vel_dis();
bool is_receive_four(int *num);
bool is_object_front( double y);
#endif

