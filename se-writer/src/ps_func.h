#ifndef PS_FUNC_H_
#define PS_FUNC_H_


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// API headers
#include "polysync_core.h"
#include "polysync_node.h"
#include "polysync_sdf.h"
#include "polysync_message.h"
#include "polysync_serial.h"
#include "polysync_node_template.h"




// *****************************************************
// static global types/macros
// *****************************************************

#ifndef NODE_FLAGS_VALUE
#define NODE_FLAGS_VALUE (0)
#endif

#define SERIAL_DEVICE_DATARATE DATARATE_115200
static const char SERIAL_PORT[] = "/dev/ttyS0";

static const char NODE_NAME[] = "polysync-serial-writer-c";
static const char OBJECTS_MSG_NAME[] = "ps_objects_msg";

static float CAR_WIDTH = 2.5;
static float DISTANCE_INIT = 300.0;
typedef unsigned long long ps_ull;
static const char CLASIFICATION[12][30] = {
                           
                           { "0: unclassified"},
                           { "1: unknown small"},
                           { "2: unknown big"},
                           { "3: pedestrians"},
                           { "4: two wheeler"},
                           { "5: car"},
                           { "6: truck"},
                           { "7: none definition"},
                           { "8: none definition"},
                           { "9: none definition"},
                           { "10: over drivable"},
                           { "11: under drivable"}
                                     };


        
        
// *****************************************************
// user-definition function declarations
// *****************************************************
void ps_printf( const ps_msg_ref const message );
int  ps_serial_send(void * const user_data, char *buf);


#endif
