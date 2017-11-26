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
#include "polysync_socket.h"
#include "polysync_node_template.h"




// *****************************************************
// static global types/macros
// *****************************************************

#ifndef NODE_FLAGS_VALUE
#define NODE_FLAGS_VALUE (0)
#endif

#define UDP_PORT (9966)

static const char UDP_ADDRESS[] = "192.168.1.201";
static const char NODE_NAME[] = "polysync-socket-writer-c";
static const char OBJECTS_MSG_NAME[] = "ps_objects_msg";

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
// static declarations
// *****************************************************
static void ps_objects_msg__handler(
        const ps_msg_type msg_type,
        const ps_msg_ref const message,
        void * const user_data );

static int set_configuration(
        ps_node_configuration_data * const node_config );


static void on_init(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_release(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_error(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_fatal(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_warn(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );

static void on_ok(
        ps_node_ref const node_ref,
        const ps_diagnostic_state * const state,
        void * const user_data );
        
        
// *****************************************************
// user-definition function declarations
// *****************************************************
void ps_printf( const ps_msg_ref const message );
void ps_socket_error(ps_socket* socket);
void ps_socket_send_error(int ret);
void ps_socket_init_error(int ret);
void ps_socket_set_address_error(int ret);
void ps_socket_set_reuse_option_error(int ret);
void ps_message_get_type_by_name_error(int ret);
void ps_message_register_listener_error(int ret);
int  ps_socket_send(ps_socket *socket, unsigned char *buf);


#endif
