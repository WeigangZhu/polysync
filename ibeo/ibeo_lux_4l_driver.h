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
 * @file ibeo_lux_4l_driver.h
 * @brief Ibeo LUX Hardware Driver.
 *
 * @warning See the data types descriptions for use of little or big endian byte order.
 *
 * Default LUX sensors use default IP 192.168.0.1, TCP data port 12002.
 * Default LUX ECUs use default IP 192.168.0.100, TCP data port 12002.
 *
 * Protocol document: FF5.7.4 (version 1.38)
 *
 * Addressed systems are Ibeo LUX sensors with firmware version 1.2.x
 * and ECUs or applications using
 * the current Ibeo FUSION SYSTEM/software version >= 1.5.
 *
 * NTP64 timestamps represent the time encoded in 8 bytes.
 * In order to decode NTP64 timestamps, the corresponding 8 bytes
 * need to be interpreted as uint64_t.
 * The higher 4 bytes are the number of seconds since 1.1.1900 - 0:00:00.
 * The lower 4 bytes represent the fractional seconds with a resolution of 2-32 seconds.
 *
 * The following parameters require a reset
 * to take effect (ie after \re IBEO_LUX_COMMAND_RESET):
 * \li 0x1000 - IP Address - \ref IBEO_LUX_PARAMETER_INDEX_IP_ADDRESS
 * \li 0x1001 - TCP Port
 * \li 0x1002 - Subnet Mask
 * \li 0x1003 - Standard Gateway
 * \li 0x1011 - CAN Baud Rate
 * \li 0x1019 - Interface Flags
 *
 * PUBLIC_HEADER
 */




#ifndef IBEO_LUX_DRIVER_H
#define	IBEO_LUX_DRIVER_H




#include <inttypes.h>

#include "polysync_core.h"
#include "polysync_socket.h"
#include "polysync_can.h"




/**
 * @brief Default Ibeo LUX Ethernet TCP port number.
 *
 */
#define IBEO_LUX_DEFAULT_TCP_PORT (12002)


/**
 * @brief Default Ibeo LUX Ethernet TCP port number.
 *
 */
#define IBEO_SCALA_DEFAULT_TCP_PORT (12004)


/**
 * @brief Default recommended buffer size for reading Ibeo LUX messages. [bytes]
 *
 * @warning Readers must be able to handle message sizes greater than this.
 *
 */
#define IBEO_LUX_DEFAULT_MESSAGE_SIZE (60000)

/**
 * @brief Default recommended buffer size for reading Ibeo ScaLa messages. [bytes]
 *
 * @warning Readers must be able to handle message sizes greater than this.
 *
 */
#define IBEO_SCALA_DEFAULT_MESSAGE_SIZE (600000)


/**
 * @brief Amount of time to wait for the TCP connection to complete. [microseconds]
 *
 */
#define IBEO_LUX_CONNECT_TIMEOUT (500000)


/**
 * @brief Ego/motion CAN channel datarate. [ps_datarate_kind]
 *
 */
#define IBEO_LUX_MOTION_CAN_DATARATE (DATARATE_500K)


/**
 * @brief Ego/motion CAN channel flags. [unsigned long]
 *
 */
#define IBEO_LUX_MOTION_CAN_CHANNEL_FLAGS (PSYNC_CAN_MSG_EXT)


/**
 * @brief Update interval to publish ego/motion CAN frames at. [microseconds]
 *
 */
#define IBEO_LUX_MOTION_CAN_UPDATE_INTERVAL (50000)


/**
 * @brief Ego/motion CAN ID for yaw rate. [unsigned long]
 *
 */
#define IBEO_LUX_MOTION_CAN_ID_YAW_RATE (0x306)


/**
 * @brief Ego/motion CAN ID for vehicle velocity. [unsigned long]
 *
 */
#define IBEO_LUX_MOTION_CAN_ID_VELOCITY (0x303)


/**
 * @brief Ego/motion CAN ID for steering angle. [unsigned long]
 *
 */
#define IBEO_LUX_MOTION_CAN_ID_STEER_ANGLE (0x305)


/**
 * @brief Ego/motion CAN ID for yaw rate. [unsigned long]
 *
 */
#define IBEO_SCALA_MOTION_CAN_ID_YAW_RATE (0x101)


/**
 * @brief Ego/motion CAN ID for vehicle velocity. [unsigned long]
 *
 */
#define IBEO_SCALA_MOTION_CAN_ID_VELOCITY (0x100)


/**
 * @brief Ego/motion CAN ID for steering angle. [unsigned long]
 *
 */
#define IBEO_SCALA_MOTION_CAN_ID_DRIVING_DIRECTION (0x116)


/**
 * @brief Maximum detection range for the Ibeo LUX Fusion ECU. [meters]
 *
 */
#define IBEO_LUX_ECU_MAX_RANGE (300.0)


/**
 * @brief Ego/motion CAN yaw rate signal scale. [radians]
 *
 */
#define IBEO_LUX_MOTION_CAN_YAW_RATE_SCALE (0.0001)


/**
 * @brief Ego/motion CAN vehicle velocity signal scale. [meters/second]
 *
 */
#define IBEO_LUX_MOTION_CAN_VELOCITY_SCALE (0.01)


/**
 * @brief Ego/motion CAN steering angle signal scale. [radians]
 *
 */
#define IBEO_LUX_MOTION_CAN_STEER_ANGLE_SCALE (0.001)



/**
 * @brief Ego/motion CAN yaw rate signal scale read from sensor. [double]
 *
 */
#define IBEO_SCALA_MOTION_CAN_YAW_RATE_READ_SCALE (0.0001)



/**
 * @brief Ego/motion CAN yaw rate signal scale written to sensor. [double]
 *
 */
#define IBEO_SCALA_MOTION_CAN_YAW_RATE_WRITE_SCALE (0.01)


/**
 * @brief Ego/motion CAN vehicle velocity signal scale. [double]
 *
 */
#define IBEO_SCALA_VELOCITY_SCALE (0.01)



/**
 * @brief Ibeo LUX message header magic word value. [uint32_t]
 *
 */
#define IBEO_LUX_HEADER_MAGIC_WORD (0xAFFEC0C2)


#define IBEO_LUX_DATA_TYPE_COMMAND (0x2010)


#define IBEO_LUX_DATA_TYPE_REPLY (0x2020)


#define IBEO_LUX_DATA_TYPE_ERRORS_AND_WARNINGS (0x2030)


#define IBEO_LUX_DATA_TYPE_SCAN_DATA (0x2202)


#define IBEO_LUX_DATA_TYPE_VEHICLE_STATE (0x2805)


#define IBEO_LUX_DATA_TYPE_ECU_SCAN_DATA (0x2205)


#define IBEO_LUX_DATA_TYPE_ECU_OBJECT_DATA (0x2280)


#define IBEO_LUX_DATA_TYPE_OBJECT_DATA (0x2221)


#define IBEO_LUX_DATA_TYPE_ECU_VEHICLE_STATE (0x2807)


#define IBEO_SCALA_DATA_TYPE_SCAN_DATA (0x2208) /*!< Data type ID indicated in message header */


#define IBEO_SCALA_DATA_TYPE_OBJECT_DATA (0x2271) /*!< Data type ID indicated in message header */


#define IBEO_SCALA_DATA_TYPE_CAMERA_IMAGE_DATA (0x2403) /*!< Data type ID indicated in message header */


#define IBEO_SCALA_DATA_TYPE_HOST_VEHICLE_STATE_FROM_SCALA (0x2805) /*!< Data type ID indicated in message header */


#define IBEO_SCALA_DATA_TYPE_HOST_VEHICLE_STATE (0x2806) /*!< Data type ID indicated in message header */


#define IBEO_SCALA_DATA_TYPE_HOST_VEHICLE_STATE_EXTENDED (0x2807) /*!< Data type ID indicated in message header */


#define IBEO_SCALA_DATA_TYPE_DEVICE_STATUS (0x6301) /*!< Data type ID indicated in message header */


#define IBEO_SCALA_DATA_TYPE_RESERVED_0 (0x6430) /*!< Data type ID indicated in message header */


#define IBEO_SCALA_DATA_TYPE_RESERVED_1 (0x1100) /*!< Data type ID indicated in message header */


// reset sensor, same as power-cycle
// command data length is sizeof(ibeo_lux_command_header_s), no reply
#define IBEO_LUX_COMMAND_RESET (0x0000)

// command data length is sizeof(ibeo_lux_command_header_s)
#define IBEO_LUX_COMMAND_GET_STATUS (0x0001)

// command data length is sizeof(ibeo_lux_command_header_s)
#define IBEO_LUX_COMMAND_START_MEASURE (0x0020)

// command data length is sizeof(ibeo_lux_command_header_s)
#define IBEO_LUX_COMMAND_STOP_MEASURE (0x0021)

// command data length is sizeof(ibeo_lux_ecu_set_filter_command_s)
#define IBEO_LUX_COMMAND_ECU_SET_FILTER (0x0005)






// command data length is sizeof(ibeo_lux_command_header_s)
#define IBEO_LUX_COMMAND_SAVE_CONFIG (0x0004)


//
#define IBEO_LUX_COMMAND_SET_PARAMETER (0x0010)


//
#define IBEO_LUX_COMMAND_GET_PARAMETER (0x0011)


//
#define IBEO_LUX_COMMAND_RESET_DEFAULT_PARAMETERS (0x001A)






/**
 * @brief Ibeo LUX parameter - IP address.
 *
 * Data type: uint32_t
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_IP_ADDRESS (0x1000)


/**
 * @brief Ibeo LUX parameter - TCP port.
 *
 * Data type: uint16_t
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_TCP_PORT (0x1001)


/**
 * @brief Ibeo LUX parameter - CAN base ID.
 *
 * Data type: uint32_t
 *
 * Valid values: <= 0x7F0
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_CAN_BASE_ID (0x1010)


/**
 * @brief Ibeo LUX parameter - data output flags.
 *
 * Data type: uint16_t
 *
 * Bit set means output is disabled, bit clear means output is enabled.
 *
 * Value 0xFFFF is invalid.
 *
 * bit0: ETH scan data
 * bit1: reserved/internal
 * bit2: ETH object data
 * bit3: ETH vehicle data
 * bit4: ETH errors/warnings
 * bit5: CAN errors/warnings
 * bit6: CAN object data
 * bit7...15: reserved/internal
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_DATA_OUTPUT_FLAGS (0x1012)


/**
 * @brief Ibeo LUX parameter - contour point density.
 *
 * Data type: uint16_t
 *
 * Valid: < 3
 * 0: closest point only
 * 1: low density
 * 2: high density
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_CONTOUR_POINT_DENSITY (0x1014)


/**
 * @brief Ibeo LUX parameter - Scan frequency.
 *
 * Data type: uint16_t
 *
 * In 1/256 Hz.
 *
 * Valid:
 * \li 3200 (12.5 Hz)
 * \li 6400 (25.0 Hz)
 * \li 12800 (50.0 Hz)
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_SCAN_FREQUENCY (0x1102)


/**
 * @brief Ibeo LUX parameter - sensor mounting X.
 *
 * Data type: int16_t
 *
 * In cm, relative to vehicle reference point, rear axle.
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_SENSOR_MOUNTING_X (0x1200)


/**
 * @brief Ibeo LUX parameter - sensor mounting Y.
 *
 * Data type: int16_t
 *
 * In cm, relative to vehicle reference point, rear axle.
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_SENSOR_MOUNTING_Y (0x1201)


/**
 * @brief Ibeo LUX parameter - sensor mounting Z.
 *
 * Data type: int16_t
 *
 * In cm, relative to vehicle reference point, rear axle.
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_SENSOR_MOUNTING_Z (0x1202)


/**
 * @brief Ibeo LUX parameter - sensor mounting yaw.
 *
 * Data type: int16_t
 *
 * In 1/32°, order is yaw, pitch, roll.
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_SENSOR_MOUNTING_YAW (0x1203)


/**
 * @brief Ibeo LUX parameter - sensor mounting pitch.
 *
 * Data type: int16_t
 *
 * In 1/32°, order is yaw, pitch, roll.
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_SENSOR_MOUNTING_PITCH (0x1204)


/**
 * @brief Ibeo LUX parameter - sensor mounting roll.
 *
 * Data type: int16_t
 *
 * In 1/32°, order is yaw, pitch, roll.
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_SENSOR_MOUNTING_ROLL (0x1205)


/**
 * @brief Ibeo LUX parameter - device type.
 *
 * Data type: uint16_t
 *
 * Supported/known device types start at \ref IBEO_LUX_DEVICE_TYPE_LUX3.
 *
 */
#define IBEO_LUX_PARAMETER_INDEX_DEVICE_TYPE (0x3301)


/**
 * @brief Ibeo LUX device type - LUX3.
 *
 * Parameter value for \ref IBEO_LUX_PARAMETER_INDEX_DEVICE_TYPE.
 *
 */
#define IBEO_LUX_DEVICE_TYPE_LUX3 (0x06)


/**
 * @brief Ibeo LUX device type - LUX4.
 *
 * Parameter value for \ref IBEO_LUX_PARAMETER_INDEX_DEVICE_TYPE.
 *
 */
#define IBEO_LUX_DEVICE_TYPE_LUX4 (0x10)


/**
 * @brief Ibeo LUX device type - LUX4 HD.
 *
 * Parameter value for \ref IBEO_LUX_PARAMETER_INDEX_DEVICE_TYPE.
 *
 */
#define IBEO_LUX_DEVICE_TYPE_LUX4HD (0x11)


/**
 * @brief Ibeo LUX device type - LUX4 8 Layer.
 *
 * Parameter value for \ref IBEO_LUX_PARAMETER_INDEX_DEVICE_TYPE.
 *
 */
#define IBEO_LUX_DEVICE_TYPE_LUX4_8L (0x18)


/**
 * @brief Ibeo LUX device type - LUX4 HD 8 Layer.
 *
 * Parameter value for \ref IBEO_LUX_PARAMETER_INDEX_DEVICE_TYPE.
 *
 */
#define IBEO_LUX_DEVICE_TYPE_LUX4HD_8L (0x19)


//
#define IBEO_LUX_CLASSIFICATION_UNCLASSIFIED (0x00)


//
#define IBEO_LUX_CLASSIFICATION_UNKNOWN_SMALL (0x01)


//
#define IBEO_LUX_CLASSIFICATION_UNKNOWN_BIG (0x02)

//
#define IBEO_LUX_CLASSIFICATION_PEDESTRIAN (0x03)


//
#define IBEO_LUX_CLASSIFICATION_BIKE (0x04)


//
#define IBEO_LUX_CLASSIFICATION_CAR (0x05)


//
#define IBEO_LUX_CLASSIFICATION_TRUCK (0x06)


//
#define IBEO_LUX_CLASSIFICATION_UNDRIVABLE (0x0C)


//
#define IBEO_LUX_CLASSIFICATION_OVER_DRIVABLE (0x12)


//
#define IBEO_LUX_CLASSIFICATION_UNDER_DRIVABLE (0x13)



// enforce 1 byte alignment so we can do linear packing
#pragma pack(push)
#pragma pack(1)


/**
 * @brief Ibeo LUX message header.
 *
 * @warning Message header bytes are encoded in big endian format.
 *
 * Each message starts with a header, followed by data.
 * To re-sync search for the \ref ibeo_lux_message_header_s.magic_word field.
 *
 */
typedef struct
{
    //
    //
    uint32_t magic_word; /*!< Constant sync token, used to find the beginning of a message.
                          * Valid messages have a magic word of \ref IBEO_LUX_HEADER_MAGIC_WORD. */
    //
    //
    uint32_t prev_message_size; /*!< Size of previous message not including the header size, not used with live data. [bytes] */
    //
    //
    uint32_t message_size; /*!< Size of this message not including the header size. [bytes] */
    //
    //
    uint8_t reserved_0; /*!< Reserved data. */
    //
    //
    uint8_t device_id; /*!< Identifier of the connected device, not used in data received directly from Ibeo LUX sensors. */
    //
    //
    uint16_t data_type; /*!< Data type of the message data. */
    //
    //
    uint64_t ntp_timestamp; /*!< Timestamp when the message was created. [NTP64] */
    //
    //
    // message data - may not exist for some messages
} ibeo_lux_message_header_s;


/**
 * @brief Ibeo LUX command data header.
 *
 * @warning Command header bytes are encoded in big endian format.
 *
 * Each command starts with a header, followed by data.
 * Most commands will be replied with a \ref ibeo_reply_header_s message.
 *
 * Data type: \ref IBEO_LUX_DATA_TYPE_COMMAND
 *
 */
typedef struct
{
    //
    //
    uint16_t id; /*!< Command identifier. */
    //
    //
    uint16_t reserved_0; /*!< Reserved data. */
    //
    //
    // command data - may not exist for some commands
} ibeo_lux_command_header_s;


/**
 * @brief Ibeo LUX reply data header.
 *
 * @warning Reply header bytes are encoded in big endian format.
 *
 * Each reply starts with a header, followed by data.
 *
 * Data type: \ref IBEO_LUX_DATA_TYPE_REPLY
 *
 */
typedef struct
{
    //
    //
    uint16_t id; /*!< Reply identifier.
                  * If a command succeeded, the reply identifier is equal
                  * to the corresponding command identifier.
                  * If a command failed, the reply identifier
                  * is the command identifier + 0x8000.
                  * Thus, the most significant bit indicates a failed command. */
    //
    //
    // reply data - may not exist for some commands
} ibeo_lux_reply_header_s;


/**
 * @brief Ibeo LUX ECU set filter command data.
 *
 * @warning Command bytes are encoded in big endian format.
 *
 */
typedef struct
{
    //
    //
    uint16_t id; /*!< Command identifier. */
    //
    //
    uint16_t num_types; /*!< Number of data types. */
    //
    //
    uint16_t start_range; /*!< Start of data type range. */
    //
    //
    uint16_t end_range; /*!< End of data type range. */
} ibeo_lux_ecu_set_filter_command_s;


/**
 * @brief Ibeo LUX set parameter command data.
 *
 * @warning Parameter index bytes are encoded in big endian format on ECU types
 * and little endian format on LUX types. Parameter value bytes are
 * always encoded in little endian format.
 *
 * Follows after \ref ibeo_lux_command_header_s data.
 *
 * Acknowledged by the same command ID without any reply data.
 *
 */
typedef struct
{
    //
    //
    uint16_t parameter_index; /*!< Parameter memory index, used as the parameter identifier. */
    //
    //
    uint32_t parameter;/*!< Parameter value. Note not all bytes are used in all cases.
                        * Bytes start at byte 0, zero any remaining bytes. */
} ibeo_lux_set_parameter_command_s;


/**
 * @brief Ibeo LUX get parameter command data.
 *
 * @warning Parameter index bytes are encoded in big endian format on ECU types
 * and little endian format on LUX types.
 *
 * Follows after \ref ibeo_lux_command_header_s data.
 *
 */
typedef struct
{
    //
    //
    uint16_t parameter_index; /*!< Parameter memory index, used as the parameter identifier. */
} ibeo_lux_get_parameter_command_s;


/**
 * @brief Ibeo LUX get parameter reply data.
 *
 * Follows after \ref ibeo_lux_reply_header_s data.
 *
 */
typedef struct
{
    //
    //
    uint16_t parameter_index; /*!< Parameter memory index, used as the parameter identifier. */
    //
    //
    uint32_t parameter; /*!< Parameter value. Note not all bytes are used in all cases.
                         * Bytes start at byte 0, remaining bytes are zero. */
} ibeo_lux_get_parameter_reply_s;


/**
 * @brief Ibeo LUX get status reply data.
 *
 * Follows after \ref ibeo_lux_reply_header_s data.
 *
 * @note ECU's do not have valid serial numbers.
 *
 * Scanner status bits:
 * 0x0001: motor on
 * 0x0002: laser on
 * 0x0004: internal feedback
 * 0x0008: set frequency reached
 * 0x0010: external sync signal detected
 * 0x0020: sync ok
 * 0x0040: sync master (instead of slave)
 * 0x0100: epw compensation on
 * 0x0200: system compensation on
 * 0x0400: start pulse compensation on
 * 0x8000: upside down (Release FPGA Version 0x9604)
 *
 */
typedef struct
{
    //
    //
    uint16_t firmware_version; /*!< Firmware version.
                                * 0x1230 = version 1.2.3, 0x123B =version 1.2.3b. */
    //
    //
    uint16_t fpga_version; /*!< FPGA version.
                            * 0x1230 = version 1.2.3, 0x123B =version 1.2.3b. */
    //
    //
    uint16_t scanner_status; /*!< Scanner status bits, see description on structure for meaning. */
    //
    //
    uint32_t reserved_0; /*!< Reserved data. */
    //
    //
    uint16_t temperature; /*!< Temperature, T[°C] = - (temperature - 579.2364) / 3.63. */
    //
    //
    uint16_t serial_number_0; /*!< Serial number lower word,
                               * YYCW (z. B. YYCW = 0x0740 = year '07,calendar week 40). */
    //
    //
    uint16_t serial_number_1; /*!< Serial number counter component, upper word of serial number. */
    //
    //
    uint16_t reserved_1; /*!< Reserved data. */
    //
    //
    uint16_t fpga_timestamp_0; /*!< First 2 bytes of FPGA timestamp,
                                * YYYY MMDD hhmm (FPGA version state decimal coded). */
    //
    //
    uint16_t fpga_timestamp_1; /*!< Middle 2 bytes of FPGA timestamp,
                                * YYYY MMDD hhmm (FPGA version state decimal coded). */
    //
    //
    uint16_t fpga_timestamp_2; /*!< Last 2 bytes of FPGA timestamp,
                                * YYYY MMDD hhmm (FPGA version state decimal coded). */
} ibeo_lux_get_status_reply_s;


/**
 * @brief Ibeo LUX vehicle state data.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 * Data type: \ref IBEO_LUX_DATA_TYPE_VEHICLE_STATE
 *
 * All angles, position and distances are given in the
 * ISO 8855 / DIN 70000 scanner coordinate system.
 *
 * Error flags:
 * 0x0001: Axle dist parameter is not set, i.e. is set to zero
 * 0x0100: Measurement of steering wheel angle not up-to-date
 * 0x0200: Measurement of front wheel angle not up-to-date or could not be calculated using SteeringWheelAngle
 * 0x0800: No CAN-data received
 *
 */
typedef struct
{
    //
    //
    uint64_t ntp_timestamp; /*!< NTP timestamp. [NTP64] */
    //
    //
    uint16_t scan_number; /*!< Current scan number. */
    //
    //
    uint16_t error_flags; /*!< Error flags, see description on structure for meaning. */
    //
    //
    int16_t longitudinal_velocity; /*!< Longitudinal velocity. [centimeters/second] */
    //
    //
    int16_t steering_wheel_angle; /*!< Angle by which the steering wheel is
                                   * rotated compared to its middle position. [1/1000 radians] */
    //
    //
    int16_t front_wheel_angle; /*!< Front wheel angle
                                * (calculated from steering wheel angle if available). [1/10,000 radians] */
    //
    //
    uint16_t reserved_0; /*!< Reserved data. */
    //
    //
    int32_t position_x; /*!< Position X of vehicle relative to the configured origin. [centimeters] */
    //
    //
    int32_t position_y; /*!< Position Y of vehicle relative to the configured origin. [centimeters] */
    //
    //
    int16_t course_angle; /*!< Orientation or heading at time timestamp. [1/10,000 radians] */
    //
    //
    uint16_t time_difference; /*!< Time difference between this and last vehicle state message. [milliseconds] */
    //
    //
    int16_t difference_x; /*!< Distance driven in X during time difference. [millimeters] */
    //
    //
    int16_t difference_y; /*!< Distance driven in Y during time difference. [millimeters] */
    //
    //
    int16_t heading_difference; /*!< Difference in Heading during time difference. [1/10,000 radians] */
    //
    //
    uint16_t reserved_1; /*!< Reserved data. */
    //
    //
    int16_t current_yaw_rate; /*!< Yaw rate from latest CAN message received.
                               * Available since firmware version 2.5.00. [1/10,000 radians] */
    //
    //
    uint32_t reserved_2; /*!< Reserved data. */
} ibeo_lux_vehicle_state_data_s;


/**
 * @brief Ibeo LUX ECU vehicle state data.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 * Data type: \ref IBEO_LUX_DATA_TYPE_ECU_VEHICLE_STATE
 *
 * Vehicle state data available from Ibeo FUSION SYSTEM and Ibeo AppBase2 (ECU).
 *
 * All angles, position and distances are given in the
 * ISO 8855 / DIN 70000 scanner coordinate system.
 *
 */
typedef struct
{
    //
    //
    uint32_t reserved_0; /*!< Reserved data. */
    //
    //
    uint64_t ntp_timestamp; /*!< NTP timestamp. [NTP64] */
    //
    //
    int32_t position_x; /*!< Position X of vehicle relative to the configured origin. [millimeters] */
    //
    //
    int32_t position_y; /*!< Position Y of vehicle relative to the configured origin. [millimeters] */
    //
    //
    uint32_t course_angle; /*!< Orientation or heading at time timestamp. [float32 radians] */
    //
    //
    uint32_t longitudinal_velocity; /*!< Longitudinal velocity. [float32 meters/second] */
    //
    //
    uint32_t yaw_rate; /*!< Current yaw rate of vehicle. [float32 radians/second] */
    //
    //
    uint32_t steering_wheel_angle; /*!< Angle by which the steering wheel is
                                    * rotated compared to its middle position. [float32 radians] */
    //
    //
    uint32_t reserved_1; /*!< Reserved data. */
    //
    //
    uint32_t front_wheel_angle; /*!< Front wheel angle
                                * (calculated from steering wheel angle if available). [float32 radians] */
    //
    //
    uint16_t reserved_2; /*!< Reserved data. */
    //
    //
    uint32_t vehicle_width; /*!< Vehicle width. [float32 meters] */
    //
    //
    uint32_t reserved_3; /*!< Reserved data. */
    //
    //
    uint32_t distance_front_to_front_axle; /*!< Front axle to vehicle front distance. [float32 meters] */
    //
    //
    uint32_t distance_rear_axle_to_front_axle; /*!< Rear axle to front axle distance. [float32 meters] */
    //
    //
    uint32_t distance_rear_axle_to_rear; /*!< Rear axle to vehicle rear distance. [float32 meters] */
    //
    //
    uint32_t reserved_4; /*!< Reserved data. */
    //
    //
    uint32_t steer_ratio_poly0; /*!< Coefficient 0. [float32] */
    //
    //
    uint32_t steer_ratio_poly1; /*!< Coefficient 0. [float32] */
    //
    //
    uint32_t steer_ratio_poly2; /*!< Coefficient 0. [float32] */
    //
    //
    uint32_t steer_ratio_poly3; /*!< Coefficient 0. [float32] */
    //
    //
    uint32_t longitudinal_acceleration; /*!< Current longitudinal acceleration. [float32 meters/second^2] */
} ibeo_lux_ecu_vehicle_state_data_s;


/**
 * @brief Ibeo LUX ECU scan data.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 * Data type: \ref IBEO_LUX_DATA_TYPE_ECU_SCAN_DATA
 *
 * FUSION SYSTEM version 2.2 and later and Ibeo Laser view 1.6 and later.
 *
 * Child structures:
 * \li (NA) ibeo_lux_ecu_scan_data_info_s (size == 148 bytes per scanner info)
 * \li \ref ibeo_lux_ecu_scan_data_point_s
 *
 */
typedef struct
{
    //
    //
    uint64_t ntp_scan_start_time; /*!< NTP time when first measurement was done. [NTP64] */
    //
    //
    uint32_t ntp_scan_end_time_offset; /*!< Time difference between first and last measurment. [microseconds] */
    //
    //
    uint32_t flags; /*!< Scanner flags bit field. */
    //
    //
    uint16_t scan_number; /*!< Number of this scan. The number will be increased
                           * from scan to scan. Overflow occurs after 2^16 scans. */
    //
    //
    uint16_t num_points; /*!< Number of scan points in this scan. */
    //
    //
    uint8_t num_scanner_infos; /*!< Number of scanner infos in this scan. */
    //
    //
    uint16_t reserved_0; /*!< Reserved data. */
    //
    //
    uint8_t reserved_1; /*!< Reserved data. */
    //
    //
    // scanner info array starts here
    //
    //
    // scan points array starts after scanner info array data
} ibeo_lux_ecu_scan_data_s;


/**
 * @brief Ibeo LUX ECU scan data point type.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 */
typedef struct
{
    //
    //
    uint32_t position_x; /*!< Postion X. [float32 meters] */
    //
    //
    uint32_t position_y; /*!< Postion Y. [float32 meters] */
    //
    //
    uint32_t position_z; /*!< Postion Z. [float32 meters] */
    //
    //
    uint32_t echo_width; /*!< Echo width. [float32 meters] */
    //
    //
    uint8_t device_id; /*!< Device identifier. */
    //
    //
    uint8_t layer; /*!< Scan layer. */
    //
    //
    uint8_t echo; /*!< Echo number. */
    //
    //
    uint8_t reserved_0; /*!< Reserved data. */
    //
    //
    uint32_t timestamp; /*!< Time offset when this scan point was measured based on the scan start time. [microseconds] */
    //
    //
    uint16_t flags; /*!< Point flags bit field. */
    //
    //
    uint16_t reserved_1; /*!< Reserved data. */
} ibeo_lux_ecu_scan_data_point_s;


/**
 * @brief Ibeo LUX ECU object data.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 * Data type: \ref IBEO_LUX_DATA_TYPE_ECU_OBJECT_DATA
 *
 * Child structures:
 * \li \ref ibeo_lux_ecu_object_data_object_s
 *
 */
typedef struct
{
    //
    //
    uint64_t mid_scan_time; /*!< Average timestamp between start and end of scan. [NTP64] */
    //
    //
    uint16_t num_objects; /*!< Number of objects in this message. */
    //
    //
    // object array starts here
} ibeo_lux_ecu_object_data_s;


/**
 * @brief Ibeo LUX ECU Point2DFloat type.
 *
 * Used in the Ibeo LUX ECU data types, see Ibeo documentation usage for endian format.
 *
 */
typedef struct
{
    //
    //
    uint32_t position_x; /*!< Postion X. [float32 meters] */
    //
    //
    uint32_t position_y; /*!< Postion Y. [float32 meters] */
} ibeo_lux_ecu_point_2d_s;


/**
 * @brief Ibeo LUX ECU object data object type.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 * Child structures:
 * \li \ref ibeo_lux_ecu_point_2d_s
 *
 */
typedef struct
{
    //
    //
    uint16_t id; /*!< Object identifier. */
    //
    //
    uint16_t flags; /*!< Object flags bit field. */
    //
    //
    uint32_t age; /*!< Number of scans this object has been tracked for. */
    //
    //
    uint64_t ntp_timestamp; /*!< Timestamp of last measurement (COG of Segment) that was used for updating this object. [NTP64] */
    //
    //
    uint16_t prediction_age; /*!< Number of update cycles that this object has only been predicted without measurement updates. */
    //
    //
    uint8_t classification; /*!< Most likely classification of this object. */
    //
    //
    uint8_t classification_quality; /*!< Higher the value, the more reliable the object classification is. */
    //
    //
    uint32_t classification_age; /*!< Time that this object has been classified as current classification. [milliseconds] */
    //
    //
    uint64_t reserved_0; /*!< Reserved data. */
    //
    //
    uint64_t reserved_1; /*!< Reserved data. */
    //
    //
    ibeo_lux_ecu_point_2d_s box_center; /*!< Box center position. [xy meters] */
    //
    //
    uint64_t reserved_2; /*!< Reserved data. */
    //
    //
    ibeo_lux_ecu_point_2d_s box_size; /*!< Box size. [xy meters] */
    //
    //
    uint64_t reserved_3; /*!< Reserved data. */
    //
    //
    uint32_t course_angle; /*!< Orientation or heading of the object box. [float32 radians] */
    //
    //
    uint32_t course_angle_sigma; /*!< Standard deviation of the course angle. [float32 radians] */
    //
    //
    ibeo_lux_ecu_point_2d_s relative_velocity; /*!< Velocity of this object relative to the ego vehicle in the ego vehicle coordinate system. [xy meters/second] */
    //
    //
    ibeo_lux_ecu_point_2d_s relative_velocity_sigma; /*!< Standard deviation of the relative velocity. [xy meters/second] */
    //
    //
    ibeo_lux_ecu_point_2d_s absolute_velocity; /*!< Absolute velocity of this object. [xy meters/second] */
    //
    //
    ibeo_lux_ecu_point_2d_s absolute_velocity_sigma; /*!< Standard deviation of the absolute velocity. [xy meters/second] */
    //
    //
    uint64_t reserved_4; /*!< Reserved data. */
    //
    //
    uint64_t reserved_5; /*!< Reserved data. */
    //
    //
    uint16_t reserved_6; /*!< Reserved data. */
    //
    //
    uint8_t num_contour_points; /*!< Number of contour points transmitted with this object. */
    //
    //
    uint8_t closest_point_index; /*!< Closest contour point of this object as an index, zero base. */
    //
    //
    uint16_t reference_point_location; /*!< Location of the reference point. */
    //
    //
    ibeo_lux_ecu_point_2d_s reference_point; /*!< Tracked object reference point position. [xy meters] */
    //
    //
    ibeo_lux_ecu_point_2d_s reference_point_sigma; /*!< Standard deviation of the reference point. [xy meters] */
    //
    //
    uint64_t reserved_7; /*!< Reserved data. */
    //
    //
    uint32_t reserved_8; /*!< Reserved data. */
    //
    //
    uint16_t priority; /*!< Value determining priority of the object. The higher the numbeer, the hihght the priority.
                        * Values are based on (1) motion classification and (2) distance. */
    //
    //
    uint32_t reserved_9; /*!< Reserved data. */
    //
    //
    // contour points array starts here, type is \ref ibeo_lux_ecu_point_2d_s
} ibeo_lux_ecu_object_data_object_s;


/**
 * @brief Ibeo LUX scan data.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 * Data type: \ref IBEO_LUX_DATA_TYPE_SCAN_DATA
 *
 * Angles are given in the ISO 8855 / DIN 70000 scanner coordinate system.
 *
 * Scanner status bits:
 * 0x0001: motor on
 * 0x0002: laser on
 * 0x0004: internal feedback
 * 0x0008: set frequency reached
 * 0x0010: external sync signal detected
 * 0x0020: sync ok
 * 0x0040: sync master (instead of slave)
 * 0x0100: epw compensation on
 * 0x0200: system compensation on
 * 0x0400: start pulse compensation on
 * 0x8000: upside down (Release FPGA Version 0x9604)
 *
 * Scan points flags bits:
 * Bit 0: ground labeled
 * Bit 1: dirt labeled
 * Bit 2: rain labeled
 * Bit 3: reserved
 * Bit 4..6: internal
 * Bit 7..9: reserved
 * Bit 10: mirror side (0=front,1=rear)
 * Bit 11..15: reserved
 *
 * Child structures:
 * \li \ref ibeo_lux_scan_data_point_s
 *
 */
typedef struct
{
    //
    //
    uint16_t scan_number; /*!< Number of this scan. The number will be increased
                           * from scan to scan. Overflow occurs after 2^16 scans. */
    //
    //
    uint16_t scanner_status; /*!< Scanner status bits, see description on structure for meaning. */
    //
    //
    uint16_t sync_phase_offset; /*!< Phase difference
                                 * (conversion factor 409.6 ns) between sync
                                 * signal and scanner mirror
                                 * crossing the synchronization angle. */
    //
    //
    uint64_t ntp_scan_start_time; /*!< NTP time when first measurement was done. [NTP64] */
    //
    //
    uint64_t ntp_scan_end_time; /*!< NTP time when last measurement was done. [NTP64] */
    //
    //
    uint16_t angle_ticks_per_rotation; /*!< Number of angle ticks per rotation. */
    //
    //
    int16_t start_angle; /*!< Start angle of this scan. [angle ticks] */
    //
    //
    int16_t end_angle; /*!< End angle of this scan. [angle ticks] */
    //
    //
    uint16_t num_points; /*!< Number of scan points in this scan. */
    //
    //
    int16_t mounting_yaw; /*!< Mounting yaw angle. [angle ticks] */
    //
    //
    int16_t mounting_pitch; /*!< Mounting pitch angle. [angle ticks] */
    //
    //
    int16_t mounting_roll; /*!< Mounting roll angle. [angle ticks] */
    //
    //
    int16_t mounting_x; /*!< Mounting X position. [centimeters] */
    //
    //
    int16_t mounting_y; /*!< Mounting Y position. [centimeters] */
    //
    //
    int16_t mounting_z; /*!< Mounting Z position. [centimeters] */
    //
    //
    uint16_t flags; /*!< Scan points flags, flags that apply to entire scan points data.
                     *  See description on structure for meaning. */
    //
    //
    // scan points array starts here
} ibeo_lux_scan_data_s;


/**
 * @brief Ibeo ScaLa scan data used in B2 sensor with firmware version >= X027
 *
 * @warning Data bytes are encoded in big endian format.
 *
 * Data type: \ref IBEO_SCALA_DATA_TYPE_SCAN_DATA
 *
 *
 *
 */
typedef struct
{
    //
    //
    uint16_t scan_number; /*!< Number of this scan. The number will be increased
                           * from scan to scan. Overflow occurs after 2^16 scans. */
    //
    //
    uint16_t scanner_type; /*!< Scala-B2: 0x0062 */
    //
    //
    uint16_t scanner_status; /*!< Scanner status bits, see description on structure for meaning. */
    //
    //
    uint16_t angle_ticks_per_rotation; /*!< Number of angle ticks per rotation. */
    //
    //
    uint32_t scan_flags; /*!< Reserved */
    //
    //
    int16_t mounting_yaw; /*!< Mounting yaw angle. [angle ticks] */
    //
    //
    int16_t mounting_pitch; /*!< Mounting pitch angle. [angle ticks] */
    //
    //
    int16_t mounting_roll; /*!< Mounting roll angle. [angle ticks] */
    //
    //
    int16_t mounting_x; /*!< Mounting X position. [centimeters] */
    //
    //
    int16_t mounting_y; /*!< Mounting Y position. [centimeters] */
    //
    //
    int16_t mounting_z; /*!< Mounting Z position. [centimeters] */
    //
    //
    uint64_t reserved_0; /*!< Reserved data. */
    //
    //
    uint64_t reserved_1; /*!< Reserved data. */
    //
    //
    uint64_t reserved_2; /*!< Reserved data. */
    //
    //
    uint16_t reserved_3; /*!< Reserved data. */
    //
    //
    uint8_t device_id; /*!< ID of the scanner */
    //
    //
    uint8_t reserved_4; /*!< Reserved data. */
    //
    //
    uint64_t ntp_scan_start_time; /*!< NTP time when first measurement was done. [NTP64] */
    //
    //
    uint64_t ntp_scan_end_time; /*!< NTP time when last measurement was done. [NTP64] */
    //
    //
    int16_t start_angle; /*!< Start angle of this scan. [angle ticks] */
    //
    //
    int16_t end_angle; /*!< End angle of this scan. [angle ticks] */
    //
    //
    uint8_t subflags; /*!< Reserved data. */
    //
    //
    uint8_t mirror_side; /*!< Bit 0: 0 == front mirror side, 1 == rear mirror side. */
    //
    //
    uint32_t reserved_5; /*!< Reserved data. */
    //
    //
    int16_t mirror_tilt; /*!< Current mirror plane relative to the rotation axis. */
    //
    //
    uint32_t reserved_6; /*!< Reserved data. */
    //
    //
    uint16_t reserved_7; /*!< Reserved data. */
    //
    //
    uint16_t num_points; /*!< Number of scan points in this scan. */
    //
    //
    // scan points array starts here
} ibeo_scala_scan_data_s;


/**
 * @brief Ibeo LUX scan data point type.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 * Scan point flags bits:
 * 0x01: transparent point
 * 0x02: clutter (atmospheric)
 * 0x04: ground
 * 0x08: dirt
 * 0xF0: reserved
 *
 */
typedef struct
{
    //
    //
    uint8_t layer_echo; /*!< Scan layer and echo.
                         * Scan layer uses lower nibble, zero based.
                         * Scan echo uses upper nibble, zero based. */
    //
    //
    uint8_t flags; /*!< Scan point flags, see description on structure for meaning. */
    //
    //
    int16_t horizontal_angle; /*!< Angle of this point. [angle ticks] */
    //
    //
    uint16_t radial_distance; /*!< Distance of this point. [centimeters] */
    //
    //
    uint16_t echo_pulse_width; /*!< Detected width of this echo pulse. [centimeters] */
    //
    //
    uint16_t reserved_0; /*!< Reserved data. */
} ibeo_lux_scan_data_point_s;


/**
 * @brief Ibeo SCALA scan data point type.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 */
typedef struct
{
    //
    //
    uint8_t echo; /*!< Scan echo couched in reserved bits.
                   * Bits 0-3 reserved. Bits 6-7 reserved.
                   * Access Echo in bits 4-5. */
    //
    //
    uint8_t layer; /*!< Scan layer of this point */
    //
    //
    uint16_t flags; /*!< Scan point flags, see description on structure for meaning. */
    //
    //
    int16_t horizontal_angle; /*!< Angle of this point. [angle ticks] */
    //
    //
    uint16_t radial_distance; /*!< Distance of this point. [centimeters] */
    //
    //
    uint16_t echo_pulse_width; /*!< Detected width of this echo pulse. [centimeters] */
    //
    //
    uint8_t reserved_0; /*!< Reserved data. */
} ibeo_scala_scan_data_point_s;


/**
 * @brief Ibeo LUX object data.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 * Data type: \ref IBEO_LUX_DATA_TYPE_OBJECT_DATA
 *
 * Child structures:
 * \li \ref ibeo_lux_object_data_object_s
 *
 */
typedef struct
{
    //
    //
    uint64_t ntp_scan_start_time; /*!< NTP time when first measurement of the
                                   * scan these objects are updated with. [NTP64] */
    //
    //
    uint16_t num_objects; /*!< Number of objects in this message. */
    //
    //
    // object array starts here
} ibeo_lux_object_data_s;


/**
 * @brief Ibeo LUX Point2D type.
 *
 * Used in the Ibeo LUX data types, see Ibeo documentation usage for endian format.
 *
 */
typedef struct
{
    //
    //
    int16_t x; /*!< Postion X. [see Ibeo documentation usage] */
    //
    //
    int16_t y; /*!< Postion Y. [see Ibeo documentation usage] */
} ibeo_lux_point_2d_s;

/**
 * @brief Ibeo ScaLa integer Point2D type.
 *
 * Used in the Ibeo ScaLa data types, see Ibeo documentation usage for endian format.
 *
 */
typedef struct
{
    //
    //
    int16_t x; /*!< Postion X. [see Ibeo documentation usage] */
    //
    //
    int16_t y; /*!< Postion Y. [see Ibeo documentation usage] */
} ibeo_scala_point_2di_s;


/**
 * @brief Ibeo ScaLa unsigned integer Point2D type.
 *
 * Used in the Ibeo LUX data types, see Ibeo documentation usage for endian format.
 *
 */
typedef struct
{
    //
    //
    uint16_t x; /*!< Postion X. [see Ibeo documentation usage] */
    //
    //
    uint16_t y; /*!< Postion Y. [see Ibeo documentation usage] */
} ibeo_scala_point_2dui_s;


/**
 * @brief Ibeo LUX Size2D type.
 *
 * Used in the Ibeo LUX data types, see Ibeo documentation usage for endian format.
 *
 */
typedef struct
{
    //
    //
    uint16_t x; /*!< Size X / width. [see Ibeo documentation usage] */
    //
    //
    uint16_t y; /*!< Size Y / length. [see Ibeo documentation usage] */
} ibeo_lux_size_2d_s;


/**
 * @brief Ibeo LUX object data object type.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 * Child structures:
 * \li \ref ibeo_lux_point_2d_s
 * \li \ref ibeo_lux_size_2d_s
 *
 */
typedef struct
{
    //
    //
    uint16_t id; /*!< Object identifier. */
    //
    //
    uint16_t age; /*!< Number of scans this object has been tracked for. */
    //
    //
    uint16_t prediction_age; /*!< Number of update cycles that this object has only been predicted without measurement updates. */
    //
    //
    uint16_t relative_timestamp; /*!< Timestamp of this object relative to
                                  * the scan start time.
                                  * The time is based on the object reference point. [milliseconds] */
    //
    //
    ibeo_lux_point_2d_s reference_point; /*!< Tracked object reference point position. [xy centimeters] */
    //
    //
    ibeo_lux_point_2d_s reference_point_sigma; /*!< Standard deviation of the reference point. [xy centimeters] */
    //
    //
    ibeo_lux_point_2d_s closest_point; /*!< Unfiltered position of the closest object point. [xy centimeters] */
    //
    //
    ibeo_lux_point_2d_s bouning_box_center; /*!< Container of all object points center position. [xy centimeters] */
    //
    //
    ibeo_lux_size_2d_s bounding_box_size; /*!< Container of all object points size. [xy centimeters] */
    //
    //
    ibeo_lux_point_2d_s box_center; /*!< Box center position. [xy centimeters] */
    //
    //
    ibeo_lux_size_2d_s box_size; /*!< Box size. [xy centimeters] */
    //
    //
    int16_t course_angle; /*!< Orientation or heading of the object box. [1/100 of a degree] */
    //
    //
    ibeo_lux_point_2d_s absolute_velocity; /*!< Absolute velocity of this object.
                                            * Value 0x8000 means invalid. [xy centimeters/second] */
    //
    //
    ibeo_lux_size_2d_s absolute_velocity_sigma; /*!< Standard deviation of the absolute velocity. [xy centimeters/second] */
    //
    //
    ibeo_lux_point_2d_s relative_velocity; /*!< Velocity of this object relative
                                            * to the ego vehicle in the ego vehicle coordinate system. [xy centimeters/second] */
    //
    //
    uint16_t classification; /*!< Most likely classification of this object.
                              * Value \ref IBEO_LUX_CLASSIFICATION_UNCLASSIFIED means unclassified. */
    //
    //
    uint16_t classification_age; /*!< Number of scans this object has been classified as current class for. */
    //
    //
    uint16_t classification_certainty; /*!< The higher this value is the more reliable is the assigned object class. */
    //
    //
    uint16_t num_contour_points; /*!< Number of contour points transmitted with this object. */
    //
    //
    // contour points array starts here, type is \ref ibeo_lux_point_2d_s. [xy centimeters]
} ibeo_lux_object_data_object_s;


/**
 * @brief Ibeo ScaLa object list header data type available from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 */
typedef struct
{
    //
    //
    uint64_t ntp_scan_start_time; /*!< NTP time when first measurement of the
                                   * scan these objects are updated with. [NTP64] */
    //
    //
    uint16_t scan_number; /*!< The scan number this object list belongs to */
    //
    //
    uint64_t internal_0; /*!< Internal data. */
    //
    //
    uint16_t num_objects; /*!< Number of objects in this message. */
    //
    //
    // object array starts here
} ibeo_scala_object_list_header_s;


/**
 * @brief Ibeo ScaLa contour point data type available in ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 */
typedef struct
{
    //
    //
    int16_t x; /*!< Postion X. [see Ibeo documentation usage] */
    //
    //
    int16_t y; /*!< Postion Y. [see Ibeo documentation usage] */
    //
    //
    uint8_t x_sigma; /*!< Postion X sigma. [see Ibeo documentation usage] */
    //
    //
    uint8_t y_sigma; /*!< Postion Y sigma. [see Ibeo documentation usage] */
    //
    //
    uint16_t internal_0; /*!< Internal data. */
} ibeo_scala_contour_point_s;


/**
 * @brief Ibeo ScaLa object untracked properties data type available
 * from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 * Child structures:
 * \li \ref ibeo_scala_point_2di_s
 * \li \ref ibeo_scala_point_2dui_s
 *
 */
typedef struct
{
    //
    //
    uint8_t internal_0; /*!< Internal data. */
    //
    //
    uint16_t relative_time_of_measure; /*!< In microseconds since start scan timestamp. */
    //
    //
    ibeo_scala_point_2di_s closest_point; /*!< In centimeters the closest point
                                        * location of this object in sensor
                                        * coordinate system. */
    //
    //
    uint16_t internal_1; /*!< Internal data. */
    //
    //
    ibeo_scala_point_2di_s box_size; /*!< In cenitmeters length/width. */
    //
    //
    ibeo_scala_point_2dui_s box_size_sigma; /*!< In cenitmeters width/length.  */
    //
    //
    int16_t box_orientation; /*!< [1/100 deg] */
    //
    //
    uint16_t box_orientation_sigma; /*!< [1/100 deg] */
    //
    //
    uint16_t internal_2; /*!< Internal data. */
    //
    //
    ibeo_scala_point_2di_s tracking_point; /*!< Tracked object reference point position, xy centimeters. */
    //
    //
    ibeo_scala_point_2dui_s tracking_point_sigma; /*!< xy centimeters. */
    //
    //
    uint16_t internal_3; /*!< Internal data. */
    //
    //
    uint8_t internal_4; /*!< Internal data. */
    //
    //
    uint8_t num_contour_points; /*!< Number of contour points transmitted with this object. */
    //
    //
    // contour points start here
}ibeo_scala_object_untracked_properties_s;


/**
 * @brief Ibeo ScaLa object tracked properties data type available
 * from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 * Child structures:
 * \li \ref ibeo_scala_point_2di_s
 * \li \ref ibeo_scala_point_2dui_s
 *
 */
typedef struct
{
    uint8_t internal_0;  /*!< Internal data. */
    //
    //
    uint16_t object_age; /*!< Number of scans in which
                          * this object has been tracked */
    //
    //
    uint16_t prediction_age; /*!< Number of scan the object
                              * has been predicted*/
    //
    //
    uint8_t dynamic_flags; /*!< Indicating the dynamic state
                            * of an object:
                            * Bit 0:
                            * 0: obj. in initialization phase
                            * 1: obj. in tracking phase
                            * Bit 1..3: reserved
                            * Bit 4..6: dynamic property
                            * 1: dynamic and moving
                            * 2: dynamic and stopped
                            * 3: internal
                            * 4: a priori stationary
                            * 5..7: reserved Bit 7: reserved */
    //
    //
    uint16_t relative_time_of_measure; /*!< Time since start scan timestamp. [microseconds] */
    //
    //
    ibeo_scala_point_2di_s closest_point; /*!< The closest x/y Point
                                           * location of this object in
                                           * sensor coordinate system. [centimeters]*/
    //
    //
    ibeo_scala_point_2di_s relative_velocity; /*!< x,y. [cenitmeters/second] */
    //
    //
    ibeo_scala_point_2dui_s relative_velocity_sigma; /*!< x,y. [cenitmeters/second] */
    //
    //
    uint8_t object_class; /*!< The class assigned by the
                           * classifier:
                           * 0: unclassified
                           * 1: unknown small
                           * 2: unknown big
                           * 3: pedestrians
                           * 4: two wheeler
                           * 5: car
                           * 6: truck
                           * 10: over drivable
                           * 11: under drivable
                           * Others: internal*/
    //
    //
    uint8_t internal_1; /*!< Internal data. */
    //
    //
    uint16_t classification_age; /*!< How long has this object
                                  * been classified with this class */
    //
    //
    uint16_t internal_2; /*!< Internal data. */
    //
    //
    ibeo_scala_point_2di_s box_size; /*!< Length/width. [centimeters] */
    //
    //
    ibeo_scala_point_2dui_s box_size_sigma; /*!< Width/length. [centimeters] */
    //
    //
    int16_t box_orientation; /*!< [1/100 degree] */
    //
    //
    uint16_t box_orientation_sigma; /*!< [1/100 degree] */
    //
    //
    uint8_t internal_3; /*!< Internal data. */
    //
    //
    uint8_t tracking_point_location; /*!< The tracking point (stated below)
                                      * is located at the following position
                                      * of the object box.
                                      * 0: Center of gravity
                                      * 1: Front/Left
                                      * 2: Front/Right
                                      * 3: Rear/Right
                                      * 4: Rear/Left
                                      * 5: Front/Center
                                      * 6: Right/Center
                                      * 7: Rear/Center
                                      * 8: Left/Center
                                      * 9: Object Center
                                      * 0xF: unknown */
    //
    //
    ibeo_scala_point_2di_s tracking_point_coordinate; /*!< Coordinates (x/y) of the
                                                       * tracking point in the reference
                                                       * coordinate system. [centimeters] */
    //
    //
    ibeo_scala_point_2di_s tracking_point_coordinate_sigma; /*!< x,y. [centimeters] */
    //
    //
    uint16_t internal_4; /*!< Internal data. */
    //
    //
    uint8_t internal_5; /*!< Internal data. */
    //
    //
    ibeo_scala_point_2di_s velocity; /*!< x,y. [centimeters/second] */
    //
    //
    ibeo_scala_point_2di_s velocity_sigma; /*!< x,y. [centimeters/second] */
    //
    //
    uint16_t internal_6; /*!< Internal data. */
    //
    //
    ibeo_scala_point_2di_s acceleration; /*!< x,y. [centimeters/second ^ 2] */
    //
    //
    ibeo_scala_point_2di_s acceleration_sigma; /*!< x,y. [centimeters/second ^ 2] */
    //
    //
    uint16_t internal_7; /*!< Internal data. */
    //
    //
    int16_t yaw_rate; /*!< [1/10000 radians] */
    //
    //
    uint16_t yaw_rate_sigma; /*!< [1/10000 radians] */
    //
    //
    uint8_t num_contour_points; /*!< Number of contour points transmitted with this object. */
    //
    //
    // contour points start here
}ibeo_scala_object_tracked_properties_s;


/**
 * @brief Ibeo ScaLa object data object type available from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in big endian format.
 * @warning Must account for internal 32 bits that follow object properties when calculating offsets.
 *
 * Child structures:
 * \li \ref ibeo_scala_object_untracked_properties
 * \li \ref ibeo_scala_object_tracked_properties
 *
 */
typedef struct
{
    //
    //
    uint32_t id; /*!< Object identifier. */
    //
    //
    uint16_t internal_0; /*!< Internal data. */
    //
    //
    uint8_t properties_available; /*<! These Flags indicate what
                                   * kind of Properties are
                                   * available for this particular
                                   * Object.
                                   * Bit1: untracked Properties
                                   * Bit3: tracked Properties
                                   * Others: internal
                                   */
    //
    //
    // obejct properties start here and are followed by uint32_t internal_1;
} ibeo_scala_object_data_s;


/**
 * @brief Ibeo ScaLa mounting position data type.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 */
typedef struct
{
    int32_t yaw_angle; /*!< Mounting angles relative to vehicle
                        * coordinate system. 32 bit float value. [radians -Pi to Pi ] */
    //
    //
    int32_t pitch_angle; /*!< Mounting angles relative to vehicle
                          * coordinate system. 32 bit float value. [radians -Pi to Pi ] */
    //
    //
    int32_t roll_angle; /*!< Mounting angles relative to vehicle
                         * coordinate system. 32 bit float value. [radians -Pi to Pi ] */
    //
    //
    int32_t x_position; /*!< Mounting position relative to
                         * vehicle coordinate system. [32 bit float value. meters] */
    //
    //
    int32_t y_position; /*!< Mounting position relative to
                         * vehicle coordinate system. 32 bit float value. [meters] */
    //
    //
    int32_t z_position; /*!< Mounting position relative to
                         * vehicle coordinate system. 32 bit float value. [meters] */


}ibeo_scala_mounting_positionf_s;


/**
 * @brief Ibeo ScaLa camera image data type available
 * from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in big endian format.
 *
 * Child structures:
 * \li \ref ibeo_scala_mounting_positionf_s
 *
 */
typedef struct
{
    uint16_t image_format; /*!< 0 : JPEG ,
                            * 1 : MJPEG,
                            * 2 : GRAY8,
                            * 3 : YUV420,
                            * 4 : YUV422 */
    //
    //
    uint32_t timestamp; /*!< Time since power on. [microseconds] */
    //
    //
    uint64_t ntp_timestamp; /*!< NTP timestamp. [NTP64] */
    //
    //
    uint8_t device_id; /*!< Each IBEO device has a
                        * system wide unique id. */
    //
    //
    ibeo_scala_mounting_positionf_s mounting_position; /*!< Mounting position of the
                                                        * camera in vehicle
                                                        * coordinate system. */
    //
    //
    int64_t horizontal_opening_angle; /*!< 64 bit double value. [radians] */
    //
    //
    int64_t vertical_opening_angle; /*!< 64 bit double value. [radians] */
    //
    //
    uint16_t image_width; /*!< Pixel line count. */
    //
    //
    uint16_t image_height; /*!< Pixel column count. */
    //
    //
    uint32_t compressed_size; /*!< Size in bytes of the following image buffer. */
    //
    //
    // image buffer start here
}ibeo_scala_camera_image_data_s;


/**
 * @brief Ibeo ScaLa vehicle state from ScaLa data type available
 * from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 */
typedef struct
{
    uint64_t ntp_timestamp; /*!< NTP timestamp. [NTP64] */
    //
    //
    uint16_t scan_number; /*!< For synchronisation with Scan */
    //
    //
    uint16_t error_flags; /*!< Currently not used in ScaLa-B1/B2 */
    //
    //
    int16_t longitudinal_velocity; /*!< Longitudinal velocity [0.01m/s] */
    //
    //
    int16_t steering_wheel_angle; /*!< Angle by which the steering wheel is
                                   * rotated compared to its middle position.
                                   * [0.001rad] */
    //
    //
    int16_t front_wheel_angle; /*!< Wheel angle (calculated from steering
                                * wheel angle if available) [0.0001 rad] */
    //
    //
    uint16_t reserved_0; /*!< Reserved data. */
    //
    //
    int32_t x_position; /*!< Distance from origin in X-Direction. [0.01m]*/
    //
    //
    int32_t y_position; /*!< Distance from origin in Y-Direction. [0.01m]*/
    //
    //
    int16_t course_angle; /*!< Orientation at time timestamp [0.0001rad] */
    //
    //
    uint16_t time_difference; /*!< Time difference between this and last
                               * vehicle state message [ms] */
    //
    //
    int16_t x_difference; /*!< Distance driven in X during time
                           * difference [0.001m] */
    //
    //
    int16_t y_difference; /*!< Distance driven in Y during time
                           * difference [0.001m] */
    //
    //
    int16_t heading_difference; /*!< Difference in Heading during time
                                 * difference [0.0001 rad] */
    //
    //
    uint16_t reserved_1; /*!< Reserved data. */
    //
    //
    int16_t current_yaw_rate; /*!< Yaw rate from latest CAN-Message received.
                               * Available since firmware version 2.5.00.
                               * [0.0001 rad/s] */
    //
    //
    uint32_t reserved_2; /*!< Reserved data. */

}ibeo_scala_host_vehicle_state_from_scala_s;


/**
 * @brief Ibeo ScaLa vehicle state data type available
 * from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 */
typedef struct
{
    uint32_t reserved_0; /*!< Reserved data. */
    //
    //
    uint64_t ntp_timestamp; /*!< NTP timestamp. [NTP64] */
    //
    //
    int32_t distance_x; /*!< Distance from origin in x-direction. [10^-4 m] */
    //
    //
    int32_t distance_y; /*!< Distance from origin in y-direction. [10^-4 m] */
    //
    //
    int32_t course_angle; /*!< Course angle. 32 bit float value. [radians] */
    //
    //
    int32_t longitudinal_velocity; /*!< Longitudinal velocity. 32 bit float value. [m/s] */
    //
    //
    int32_t yaw_rate; /*!< * Current yaw rate of vehicle. 32 bit float value. [rad/s] */
    //
    //
    int32_t steering_wheel_angle; /*!< Angle by which the steering wheel is
                                   * rotated compared to its middle
                                   * position. 32 bit float value. [rad] */
    //
    //
    int32_t cross_acceleration; /*!< 32 bit float value. [m/s^2]*/
    //
    //
    int32_t front_wheel_angle; /*!< Angle by which the front wheel is
                                * rotated compared to the vehicle’s x-
                                * axis. 32 bit float value. [rad] */
    //
    //
    uint16_t reserved_1; /*!< Reserved data. */
    //
    //
    int32_t vehile_width; /*!< 32 bit float value. [meters] */
    //
    //
    uint32_t reserved_2; /*!< Reserved data. */
    //
    //
    int32_t distance_to_front_axle; /*!< Distance: front axle to vehicle’s front.
                                     * 32 bit float value. [meters] */
    //
    //
    int32_t distance_to_rear_axle; /*!< Distance: rear axle to vehicle’s rear.
                                     * 32 bit float value. [meters] */
    //
    //
    uint32_t reserved_3; /*!< Reserved data. */
    //
    //
    int32_t s0; /*!< Steer ratio. 32 bit float value. Coefficients for transfer
                 * function of steering wheel angle (x).
                 * s3 * x^3 + s2 * x^2 + s1 * x + s0 */
    //
    //
    int32_t s1; /*!< Steer ratio. 32 bit float value. Coefficients for transfer
                 * function of steering wheel angle (x).
                 * s3 * x^3 + s2 * x^2 + s1 * x + s0 */
    //
    //
    int32_t s2; /*!< Steer ratio. 32 bit float value. Coefficients for transfer
                 * function of steering wheel angle (x).
                 * s3 * x^3 + s2 * x^2 + s1 * x + s0 */
    //
    //
    int32_t s3; /*!< Steer ratio. 32 bit float value. Coefficients for transfer
                 * function of steering wheel angle (x).
                 * s3 * x^3 + s2 * x^2 + s1 * x + s0 */
} ibeo_scala_host_vehicle_state_s;


/**
 * @brief Ibeo ScaLa vehicle state extended data type available
 * from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 * Child structures:
 * \li \ref ibeo_scala_host_vehicle_state
 *
 */
typedef struct
{
    ibeo_scala_host_vehicle_state_s vehicle_state; /*!< \ref ibeo_scala_host_vehicle_state */
    //
    //
    int32_t longitudinal_acceleration; /*!< [m/s^2]*/
} ibeo_scala_host_vehicle_state_extended_s;


/**
 * @brief Ibeo ScaLa device status data type available
 * from ScaLa B2 Laser scanners.
 *
 * @warning Data bytes are encoded in little endian format.
 *
 *
 */
typedef struct
{
    uint16_t reserved_0; /*!< Reserved data. */
    //
    //
    uint16_t reserved_1; /*!< Reserved data. */
    //
    //
    uint16_t reserved_2; /*!< Reserved data. */
    //
    //
    uint8_t scanner_type; /*!< Type of the scanner */
    //
    //
    uint64_t reserved_3; /*!< Reserved data. */
    //
    //
    uint64_t reserved_4; /*!< Reserved data. */
    //
    //
    uint64_t reserved_5; /*!< Reserved data. */
    //
    //
    uint32_t reserved_6; /*!< Reserved data. */
    //
    //
    uint8_t reserved_7; /*!< Reserved data. */
    //
    //
    int32_t sensor_temperature; /*!< Temperature of the sensor (near APD).
                                  *  32 bit float value. */
    //
    //
    int32_t frequency; /* 32 bit float value. */
    //
    //
    // 124 more bytes of reserved data follow
}ibeo_scala_device_status_s;




// restore alignment
#pragma pack(pop)




/**
 * @brief Configure and connect to Ethernet socket. Sets non-blocking IO and expects an initialized socket.
 * 
 * @warning Connection error timeout is tuned for an Ibeo ScaLa, other Ibeo devices may be able to get away with a uch smaller timeout. 
 * 
 * @param [out] sock A pointer to \ref ps_socket that receives the configuration.
 * @param [in] Address of socket that receives configuration.
 * @param [out] port Port number of socket that receives configuration.
 * 
 * @return DTC code: 
 * \ref DTC_NONE on success.
 */
int ibeo_lux_configure_socket(
        ps_socket * const sock,
        const char * const address,
        const unsigned long port );


// configure motion CAN channel
int ibeo_lux_configure_motion_can_channel( ps_can_channel * const channel );


// DTC_DATAERR if invalid
int ibeo_lux_is_message_header_valid(
        const unsigned char * const buffer,
        const unsigned long buffer_len );

// DTC_DATAERR if invalid
int ibeo_scala_is_message_header_valid(
        const unsigned char * const buffer,
        const unsigned long buffer_len );


// DTC_DATAERR if invalid
int ibeo_lux_read_message_header(
        ps_socket * const sock,
        unsigned char * const buffer,
        const unsigned long buffer_len,
        unsigned long * const bytes_read,
        ps_timestamp * const timestamp );


// DTC_DATAERR if invalid
int ibeo_scala_read_message_header(
        ps_socket * const sock,
        unsigned char * const buffer,
        const unsigned long buffer_len,
        unsigned long * const bytes_read,
        ps_timestamp * const timestamp );

// tries to do mulitple reads up to length
int ibeo_lux_read_message_data(
        ps_socket * const sock,
        const unsigned long message_size,
        unsigned char * const buffer,
        const unsigned long buffer_len,
        ps_timestamp * const timestamp );


// user fills command data after command header, message size is set by function
// command data length is usually sizeof(ibeo_lux_command_header_s)
// for ECU types, command ID must be byte-swapped to big endian
int ibeo_lux_fill_command_header(
        const uint16_t id,
        const uint32_t command_data_len,
        unsigned char * const buffer,
        const unsigned long buffer_len
        );


// enables all types
int ibeo_lux_fill_command_ecu_start(
        unsigned char * const buffer,
        const unsigned long buffer_len );


// sets current timestamp
// radians/second
int ibeo_lux_fill_vehicle_yawrate_frame(
        ps_can_frame * const frame,
        const double yaw_rate );


// sets current timestamp
// meters/second
int ibeo_lux_fill_vehicle_velocity_frame(
        ps_can_frame * const frame,
        const double velocity );


//
int ibeo_lux_fill_vehicle_steer_angle_frame(
        ps_can_frame * const frame,
        const double steering_angle );


/**
 * @brief Populates a CAN frame with Ego motion yaw rate.
 *
 * @param [out] frame CAN frame containing ego motion data sent to device.
 * @param [in] yaw_rate Takes radians/second. Converts to degrees.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if frame is valid.
 * \li \ref DTC_USAGE if arguments invalid.
 */
int ibeo_scala_fill_vehicle_yawrate_frame(
        ps_can_frame * const frame,
        const double yaw_rate );


/**
 * @brief Populates a CAN frame with Ego motion velocity.
 *
 * @param [out] frame CAN frame containing ego motion data sent to device.
 * @param [in] velocity Takes meters/second. Converts to kilometers/hour.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if frame is valid.
 * \li \ref DTC_USAGE if arguments invalid.
 */
int ibeo_scala_fill_vehicle_velocity_frame(
        ps_can_frame * const frame,
        const double velocity );


/**
 * @brief Populates a CAN frame with Ego motion drive direction.
 * Always reports forward direction.
 *
 * @param [out] frame CAN frame containing ego motion data sent to device.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if frame is valid.
 * \li \ref DTC_USAGE if arguments invalid.
 */
int ibeo_scala_fill_vehicle_drive_direction_frame(
        ps_can_frame * const frame );




#endif	/* IBEO_LUX_DRIVER_H */
