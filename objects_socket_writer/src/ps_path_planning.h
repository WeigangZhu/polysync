#ifndef PS_PATH_PLANNING_H_
#define PS_PATH_PLANNING_H_

#define OBJECT_NUMBER 20

int *path_left_right_objects_fsae(
			const ps_msg_ref const message,
			double left_objects[OBJECT_NUMBER][2],
			double right_objects[OBJECT_NUMBER][2]);
			
void *staight_line_fsae(
			double left_objects[OBJECT_NUMBER][2],
			double right_objects[OBJECT_NUMBER][2],
			int left_right_length[2]);
			
void *cube_line_fsae(void *);






#endif
