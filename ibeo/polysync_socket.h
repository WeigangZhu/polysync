/*
 * Copyright (c) 2016 PolySync
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


/**
 * @file polysync_socket.h
 * @brief Synchronous IO operations (ie polling-timeouts/multiplexing) can be achieved
 * through the use of the standard select() system call mechanisms performed
 * on the \ref ps_socket.fd file descriptor.
 *
 * PUBLIC_HEADER
 */




#ifndef POLYSYNC_SOCKET_H
#define	POLYSYNC_SOCKET_H


#ifdef __cplusplus
extern "C" {
#endif




#include <netinet/in.h>




/**
 * @brief Invalid \ref ps_socket.fd value.
 *
 */
#define     PSYNC_SOCKET_FD_INVALID     (-1)


/**
 * @brief Specifies that the OS should pick the appropriate port number.
 *
 */
#define     PSYNC_SOCKET_PORT_ANY       (0)


/**
 * @brief Specifies that any address is acceptable.
 *
 */
#define     PSYNC_SOCKET_ADDRESS_ANY    (NULL)




/**
 * @brief PolySync socket.
 *
 */
typedef struct
{
    //
    //
    int                             fd; /*!< File descriptor.
                                         * Value \ref PSYNC_SOCKET_FD_INVALID means invalid socket. */
    //
    //
    int                             domain; /*!< Communications domain.
                                             * In Linux, standard socket domains (ie AF_INET, AF_INET6, etc) are allowed. */
    //
    //
    int                             type; /*!< Communications type.
                                           * In Linux, standard types (ie SOCK_STREAM, SOCK_DGRAM, etc) are allowed. */
    //
    //
    int                             protocol; /*!< Communications protocol.
                                               * In Linux, standard protocols (ie IPPROTO_UDP, IPPROTO_TCP, etc) are allowed. */
    //
    //
    struct sockaddr_in              address; /*!< Socket adddress, used to store the source address for the socket. */
} ps_socket;




/**
 * @brief Initialize socket endpoint for communications.
 *
 * Establishes an endpoint for communications using the PolySync time domain.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the initialization.
 * @param [in] domain Communications domain. See \ref ps_socket.domain for details.
 * @param [in] type Communications type. See \ref ps_socket.type for details.
 * @param [in] protocol Communications protocol. See \ref ps_socket.protocol for details.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket configuration is invalid or not supported.
 *
 */
int psync_socket_init( ps_socket * const sock, const int domain, const int type, const int protocol );


/**
 * @brief Release socket.
 *
 * Closes a communications endpoint.
 *
 * @param [in] sock A pointer to \ref ps_socket which gets released.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 *
 */
int psync_socket_release( ps_socket * const sock );


/**
 * @brief Set socket's address data.
 *
 * The address data is used to specify the source address.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 * @param [in] address A pointer to char buffer which specifies the address to use. Value \ref PSYNC_SOCKET_ADDRESS_ANY is allowed.
 * @param [in] port Port number associated with the address. Value \ref PSYNC_SOCKET_PORT_ANY is allowed.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 *
 */
int psync_socket_set_address( ps_socket * const sock, const char * const address, const unsigned long port );


/**
 * @brief Set socket broadcast option.
 *
 * Allows UDP datagram (SOCK_DGRAM) sockets to send and receive packets sent to and from the broadcast address.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 * @param [in] enabled If not zero, enables the option. Otherwise the option is disabled.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or option not supported.
 *
 */
int psync_socket_set_broadcast_option( ps_socket * const sock, const unsigned int enabled );


/**
 * @brief Set socket reuse address option.
 *
 * Allows other sockets to bind to this port, unless there is an active listening socket
 * bound to the port already.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 * @param [in] enabled If not zero, enables the option. Otherwise the option is disabled.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or option not supported.
 *
 */
int psync_socket_set_reuse_option( ps_socket * const sock, const unsigned int enabled );


/**
 * @brief Set socket non-blocking option.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 * @param [in] enabled If not zero, enables the option. Otherwise the option is disabled.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or option not supported.
 *
 */
int psync_socket_set_nonblock_option( ps_socket * const sock, const unsigned int enabled );


/**
 * @brief Set socket receive timeout option.
 *
 * Sets the maximum amount of time a read operation is allowed to use.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 * @param [in] timeout Timeout value to use. If zero, disables the option. [microseconds]
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or option not supported.
 *
 */
int psync_socket_set_rxtimeout_option( ps_socket * const sock, const ps_timestamp timeout );


/**
 * @brief Set socket transmit timeout option.
 *
 * Sets the maximum amount of time a send operation is allowed to use.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 * @param [in] timeout Timeout value to use. If zero, disables the option. [microseconds]
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or option not supported.
 *
 */
int psync_socket_set_txtimeout_option( ps_socket * const sock, const ps_timestamp timeout );


/**
 * @brief Bind a socket to a symbolic name.
 *
 * Bind this socket to a symbolic device name like "eth0"
 * instead of using \ref psync_socket_bind to bind it to an internal address.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 * @param [in] name A pointer to char buffer which specifies the device name value.
 * @param [in] name_len Length of provided name.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or option not supported.
 *
 */
int psync_socket_bind_by_name( ps_socket * const sock, const char * const name, const size_t name_len );


/**
 * @brief Bind a socket to its address.
 *
 * The socket's address data is used in the assignment.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or configuration not supported.
 * \li \ref DTC_INTR if bind call was interrupted by the system.
 * \li \ref DTC_INPROGRESS if socket is set to non-blocking IO and operation would block.
 *
 */
int psync_socket_bind( ps_socket * const sock );


/**
 * @brief Connect a socket to its address.
 *
 * The socket's address data is used to initiate a connection.
 *
 * @param [in] sock A pointer to \ref ps_socket which receives the configuration.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or configuration not supported.
 * \li \ref DTC_INTR if connect call was interrupted by the system.
 * \li \ref DTC_INPROGRESS if socket is set to non-blocking IO and operation would block.
 *
 */
int psync_socket_connect( ps_socket * const sock );


/**
 * @brief Receive data from a socket connection.
 *
 * @param [in] sock A pointer to \ref ps_socket which holds the configuration.
 * @param [out] buffer A pointer to unsigned char buffer which receives the data read.
 * @param [in] buffer_len Length of provided buffer.
 * @param [out] bytes_read A pointer to unsigned long which receives the count value.
 * @param [out] timestamp A pointer to \ref ps_timestamp which receives the receive timestamp value.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or configuration not supported.
 * \li \ref DTC_INTR if call was interrupted by the system.
 * \li \ref DTC_UNAVAILABLE if timeout exceeded or would block.
 *
 */
int psync_socket_recv( const ps_socket * const sock, unsigned char * const buffer, const size_t buffer_len, unsigned long * const bytes_read, ps_timestamp * const timestamp );

/**
 * @brief Receive data from a connection or connection-less socket.
 *
 * Uses the internal source address to receive data.
 * The address can be configured with \ref psync_socket_set_address prior to using this call.
 *
 * @param [in] sock A pointer to \ref ps_socket which holds the configuration.
 * @param [out] buffer A pointer to unsigned char buffer which receives the data read.
 * @param [in] buffer_len Length of provided buffer.
 * @param [out] bytes_read A pointer to unsigned long which receives the count value.
 * @param [out] timestamp A pointer to \ref ps_timestamp which receives the receive timestamp value.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or configuration not supported.
 * \li \ref DTC_INTR if call was interrupted by the system.
 * \li \ref DTC_UNAVAILABLE if timeout exceeded or would block.
 *
 */
int psync_socket_recv_from( const ps_socket * const sock, unsigned char * const buffer, const size_t buffer_len, unsigned long * const bytes_read, ps_timestamp * const timestamp );


/**
 * @brief Send data on a socket connection.
 *
 * @param [in] sock A pointer to \ref ps_socket which holds the configuration.
 * @param [in] buffer A pointer to unsigned char buffer which specifies the data to be sent.
 * @param [in] buffer_len Length of provided buffer.
 * @param [out] bytes_sent A pointer to unsigned long which receives the count value.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or configuration not supported.
 * \li \ref DTC_INTR if call was interrupted by the system.
 * \li \ref DTC_UNAVAILABLE if timeout exceeded or would block.
 *
 */
int psync_socket_send( const ps_socket * const sock, unsigned char * const buffer, const size_t buffer_len, unsigned long * const bytes_sent );


/**
 * @brief Send data on a connection-mode socket.
 *
 * Uses the internal source address to send data.
 * The address can be configured with \ref psync_socket_set_address prior to using this call.
 *
 * @param [in] sock A pointer to \ref ps_socket which holds the configuration.
 * @param [in] buffer A pointer to unsigned char buffer which specifies the data to be sent.
 * @param [in] buffer_len Length of provided buffer.
 * @param [out] bytes_sent A pointer to unsigned long which receives the count value.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if socket invalid or configuration not supported.
 * \li \ref DTC_INTR if call was interrupted by the system.
 * \li \ref DTC_UNAVAILABLE if timeout exceeded or would block.
 *
 */
int psync_socket_send_to( const ps_socket * const sock, unsigned char * const buffer, const size_t buffer_len, unsigned long * const bytes_sent );




#ifdef __cplusplus
}
#endif


#endif	/* POLYSYNC_SOCKET_H */
