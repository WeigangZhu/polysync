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
 * @file polysync_core.h
 * @brief PolySync Core API.
 *
 * PUBLIC_HEADER
 */




#ifndef POLYSYNC_CORE_H
#define	POLYSYNC_CORE_H


#ifdef __cplusplus
extern "C" {
#endif




// Communication resources
#include "dds_dcps.h"
#include "psyncDcps.h"

// Node resources
#include "polysync_node_ref.h"




/**
 * @brief Enabled standard output logging.
 *
 * PolySync initialization flag bit provided to \ref psync_init, if set all messages produced with
 * \ref psync_log_message will be printed to standard output in addition
 * to the system log.
 *
 * @note This flag can be ignored when using the standard logger configuration, see PSYNC_HOME/db/polysync_logger.conf for more information.
 *
 */
#define PSYNC_INIT_FLAG_STDOUT_LOGGING (0x4)


/**
 * @brief PolySync generic memory release function.
 *
 */
#define psync_free(x) DDS_free(x)


/**
 * @brief PolySync printf format checking attribute.
 *
 */
# if defined __GNUC__
#define PSYNC_CHECK_PRINTF_ATTRIBUTE(m,n) __attribute__((format(printf,m,n)))
#else
#define PSYNC_CHECK_PRINTF_ATTRIBUTE(m,n)
#endif


/**
 * @brief PolySync debug log function.
 *
 * This a wrapper to \ref psync_log, using log level \ref LOG_LEVEL_DEBUG.
 *
 */
#define psync_log_debug(...) \
        psync_log(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
        LOG_LEVEL_DEBUG, __VA_ARGS__)


/**
 * @brief PolySync informative log function.
 *
 * This a wrapper to \ref psync_log, using log level \ref LOG_LEVEL_INFO.
 *
 */
#define psync_log_info(...) \
        psync_log(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
        LOG_LEVEL_INFO, __VA_ARGS__)


/**
 * @brief PolySync warning log function.
 *
 * This a wrapper to \ref psync_log, using log level \ref LOG_LEVEL_WARN.
 *
 */
#define psync_log_warn(...) \
        psync_log(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
        LOG_LEVEL_WARN, __VA_ARGS__)


/**
 * @brief PolySync error log function.
 *
 * This a wrapper to \ref psync_log, using log level \ref LOG_LEVEL_ERROR.
 *
 */
#define psync_log_error(...) \
        psync_log(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
        LOG_LEVEL_ERROR, __VA_ARGS__)




/**
 * @brief PolySync data model core module name.
 *
 */
extern const char       PSYNC_CORE_MODULE_NAME[];




/**
 * @brief Create PolySync node and initialize its resources.
 *
 * Establishes a node connection to the specified PolySync domain.
 *
 * @param [in] node_name A pointer to char which specifies the node name buffer.
 * @param [in] node_type Node type to configure, some values are not available under certain license models.
 * Value \ref PSYNC_NODE_TYPE_API_USER is the default.
 * @param [in] domain_id Domain the node will operate in. Value \ref PSYNC_DEFAULT_DOMAIN is the default.
 * @param [in] sdf_key Node SDF key. Value \ref PSYNC_SDF_ID_INVALID means node is not in the SDF.
 * @param [in] flags Node flags. Value zero is the default.
 * @param [out] node_ref_ptr A pointer to \ref ps_node_ref which receives the initialization.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if node_ref is already initialized or SDF configuration is invalid.
 * \li \ref DTC_LICERR if license does not exist or does not allow the given configuration.
 * \li \ref DTC_PROTOCOL if core/API versions do not match or other nodes are on the domain with different data model versions.
 * \li \ref DTC_OSERR if failed to initialize core resources.
 * \li \ref DTC_MEMERR if failed to allocate core resources.
 *
 */
int psync_init( const char * node_name, const ps_node_type node_type, const unsigned long domain_id, const unsigned long sdf_key, const unsigned long flags, ps_node_ref * node_ref_ptr );


/**
 * @brief Release PolySync node and its resources.
 *
 * @param [in] node_ref_ptr A pointer to \ref ps_node_ref which receives the release.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_OSERR if failed to release core resources.
 *
 */
int psync_release( ps_node_ref * node_ref_ptr );


/**
 * @brief Log a formatted message.
 *
 * Typically used by logger macros (\ref psync_log_debug, \ref psync_log_error, etc).
 *
 * Example:
 * @code
 * psync_log_info( "my message - val: %.2f", 1.23 );
 * @endcode
 *
 * @param [in] file_name Name of the file called from.
 * @param [in] file_len Size of the file called from.
 * @param [in] func_name Name of the function called from.
 * @param [in] func_len Size of the function called from.
 * @param [in] line Line number called from.
 * @param [in] level Log level/priority.
 * @param [in] format Log string formatter.
 * @param [in] ... Additional arguments to format.
 *
 */
void psync_log(
        const char * const file_name,
        const unsigned long file_len,
        const char * const func_name,
        const unsigned long func_len,
        const unsigned long line,
        const ps_log_level_kind level,
        const char * const format,
        ... ) PSYNC_CHECK_PRINTF_ATTRIBUTE(7,8);


/**
 * @brief Log a message using the core logger format.
 *
 * Logs a formatted message given the provided log level.
 *
 * @note It is recommended to use the logger macros (\ref psync_log_debug, \ref psync_log_error, etc).
 *
 * Example:
 * @code
 * psync_log_message( LOG_LEVEL_WARN, "my message - val: %.2f", 1.23 );
 * @endcode
 *
 * @param [in] level Log level/priority.
 * @param [in] format Log string formatter.
 * @param [in] ... Additional arguments to format.
 *
 */
void psync_log_message( const ps_log_level_kind level, const char * format, ... );


/**
 * @brief Get the default logger configuration file path.
 *
 * @param [in] node_ref Node reference which contains the environment to use.
 * @param [out] path A pointer to char which receives the fully qualified path.
 * @param [in] path_len Maximum size of provided path.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if node reference is invalid
 * \li \ref DTC_ENVERR if node environment is not readable.
 *
 */
int psync_get_logger_configuration_file_path(
        const ps_node_ref node_ref,
        char * const path,
        const unsigned long path_len );


/**
 * @brief Set the logger configuration file path and reload the logger interface.
 *
 * @param [in] node_ref Node reference which contains the environment to use.
 * @param [out] path A pointer to char which specifies the fully qualified path.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if node reference is invalid
 * \li \ref DTC_ENVERR if node environment is not readable.
 *
 */
int psync_set_logger_configuration_file(
        const ps_node_ref node_ref,
        const char * const path );


/**
 * @brief Set the logger category name.
 *
 * The default category is 'polysync'.
 *
 * See the logger configuration documentation for more information on
 * logger categories.
 *
 * @param [in] node_ref Node reference which contains the environment to use.
 * @param [out] path A pointer to char which specifies the category name.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if node reference is invalid
 * \li \ref DTC_ENVERR if node environment is not readable.
 *
 */
int psync_set_logger_category(
        const ps_node_ref node_ref,
        const char * const category );


/**
 * @brief Set the logger node context information.
 *
 * The default node context is 'node_name-node_guid'.
 *
 * This function is used to set the logger node context information from a separate thread.
 * Logger node context information (node name, node GUID) is by default mapped
 * to the thread that called \ref psync_init.
 *
 * See the logger configuration documentation for more information on
 * logger categories and formatting.
 *
 * @param [in] node_ref Node reference which contains the environment, node name, and node GUID to use.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if node reference is invalid
 * \li \ref DTC_ENVERR if node environment is not readable.
 *
 */
int psync_set_logger_node_context(
        const ps_node_ref node_ref );


/**
 * @brief Get current timestamp.
 *
 * Get a synchronized timestamp in microseconds.
 *
 * The PolySync time domain uses UTC microsecond timestamps unless specified otherwise.
 *
 * @param [out] timestamp A pointer to \ref ps_timestamp which receives the system timestamp value. [UTC microseconds]
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 *
 */
int psync_get_timestamp( ps_timestamp * const timestamp );


/**
 * @brief Get current monotonic timestamp.
 *
 * @param [out] timestamp A pointer to \ref ps_timestamp which receives the system monotonic timestamp value. [UTC microseconds]
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 *
 */
int psync_get_monotonic_timestamp( ps_timestamp * const timestamp );


/**
 * @brief Sleep microseconds.
 *
 * Sleep for a given length of time.
 *
 * @param [in] interval Amount of time to sleep for. [microseconds]
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 *
 */
int psync_sleep_micro( const ps_timestamp interval );


/**
 * @brief Check for a license feature by name.
 *
 * Query if this node has the given licensed feature available.
 *
 * @param [in] node_ref Node reference which contains the licensed feature set.
 * @param [in] feature_name A pointer to char which specifies the null-terminated feature name buffer.
 * @param [out] is_valid A pointer to unsigned int which receives the status of the feature. Value zero means feature not valid or available.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if node is invalid.
 *
 */
int psync_check_license_feature( const ps_node_ref node_ref, const char * feature_name, unsigned int * const is_valid );


/**
 * @brief Check if current license is valid.
 *
 * Query if the caller has a valid PolySync license.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if license is valid.
 * \li \ref DTC_LICERR if license is invalid.
 *
 */
int psync_is_license_valid( void );


/**
 * @brief Check if current license is valid using an additional license file.
 *
 * Query if the caller has a valid PolySync license.
 *
 * If any license rights exist in the provided file, they will be added to the license.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if license is valid.
 * \li \ref DTC_LICERR if license is invalid.
 *
 */
int psync_is_license_with_file_valid( const char * const license_file );


/**
 * @brief Get module information from the PolySync Data Model.
 *
 * Query the data model for information on a given module by name.
 * Standard modules:
 * \li 'core' - contains core PolySync definitions
 * \li 'navigation'
 * \li 'sensor'
 * \li 'control'
 *
 * @note Expects a hash buffer of at least \ref PSYNC_MODULE_VERIFY_HASH_LEN bytes.
 *
 * @param [in] node_ref Node reference which contains the data model.
 * @param [in] name A pointer to char buffer which specifies the module name.
 * @param [out] major A pointer to unsigned int which receives the module major version.
 * @param [out] minor A pointer to unsigned int which receives the module minor version.
 * @param [out] subminor A pointer to unsigned int which receives the module subminor version.
 * @param [out] build_date A pointer to unsigned long which receives the module build date. [UTC seconds]
 * @param [out] hash_buffer A pointer to unsigned char buffer which receives the module MD5 hash.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_CONFIG if node reference is invalid.
 *
 */
int psync_get_module_info(
        const ps_node_ref node_ref,
        const char * name,
        unsigned int * const major,
        unsigned int * const minor,
        unsigned int * const subminor,
        unsigned long * const build_date,
        unsigned char * const hash_buffer );


/**
 * @brief Get PolySync API build version information.
 *
 * @param [out] major Major version digit.
 * @param [out] minor Minor version digit.
 * @param [out] subminor Sub-minor version digit.
 * @param [out] build_date Build date. [UTC seconds]
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 *
 */
int psync_get_build_version( unsigned int * const major, unsigned int * const minor, unsigned int * const subminor, unsigned long * const build_date );


/**
 * @brief Fill buffer with PolySync API build version information.
 *
 * Fills buffer with major-minor-subminor-build_date.
 *
 * @param [out] A pointer to unsigned long long which receives the build version data.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 *
 */
int psync_get_build_version_buffer( unsigned long long * const version_buffer );


/**
 * @brief Fill @ref ps_version with PolySync API build version information.
 *
 * @param [out] A pointer to @ref ps_version which receives the build version data.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 *
 */
int psync_get_build_ps_version(
        ps_version * const version );


/**
 * @brief Get MD5 hash of a file.
 *
 * @note Expects a buffer of at least \ref PSYNC_MODULE_VERIFY_HASH_LEN bytes.
 *
 * @param [in] file_path A pointer to char buffer which specifies the file path.
 * @param [out] name_buffer A pointer to char buffer which receives the hash value.
 * @param [in] name_buffer_len Length of provided name_buffer.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) if success.
 * \li \ref DTC_USAGE if arguments invalid.
 * \li \ref DTC_OSERR if failed to initialize core resources.
 * \li \ref DTC_IOERR if failed to open file.
 * \li \ref DTC_ENVERR if failed to calculate hash.
 *
 */
int psync_get_file_hash( const char * file_path, unsigned char * const buffer, const unsigned long buffer_len );


/**
 * @brief Convert interface address string to a unsigned long value.
 *
 * Expects a format similar to "X.X.X.X".
 * Example: [in] "160.176.192.208" -> [out] 0xA0B0C0D0.
 * Example: [in] "AUTO" -> [out] \ref PSYNC_INTERFACE_ADDRESS_AUTO.
 *
 * @param [in] address A pointer to char buffer which specifies the address buffer.
 * @param [out] value A pointer to \ref ps_interface_address which receives the value.
 *
 * @return DTC code, DTC_NONE if success.
 *
 */
int psync_interface_address_string_to_value( const char * address, ps_interface_address * const value );


/**
 * @brief Convert interface address unsigned long value to string.
 *
 * @note Expects a buffer of at least \ref PSYNC_MODULE_VERIFY_HASH_LEN bytes.
 *
 * Example: [in] 0xA0B0C0D0 -> [out] "160.176.192.208".
 * Example: [in] \ref PSYNC_INTERFACE_ADDRESS_AUTO -> [out] "AUTO".
 *
 * @param [in] value Value to convert.
 * @param [out] address A pointer to char buffer which receives the address value.
 * @param [in] address_len Length of provided address.
 *
 * @return DTC code, DTC_NONE if success.
 *
 */
int psync_interface_address_value_to_string( const ps_interface_address value, char * const address, const unsigned long address_len );


/**
 * @brief Extract node identifier from a GUID.
 *
 * The node identifier is a unique identifier for the node with respect to the PolySync data-bus domain.
 *
 * @param [in] guid GUID to parse.
 * @param [out] node_id A pointer to \ref ps_identifier which receives the node identifier value.
 *
 * @return DTC code, DTC_NONE if success.
 *
 */
int psync_guid_get_node_id( const ps_guid guid, ps_identifier * const node_id );


/**
 * @brief Extract node SDF identifier from GUID.
 *
 * @deprecated This function should be replaced with \ref psync_guid_get_sdf_id.
 *
 * @param [in] guid GUID to parse.
 * @param [out] key A pointer to unsigned long which receives the node SDF identifier value.
 *
 * @return DTC code, DTC_NONE if success.
 *
 */
int psync_guid_get_sdf_key( const ps_guid guid, unsigned long * const key );


/**
 * @brief Extract node SDF identifier from a GUID.
 *
 * @param [in] guid GUID to parse.
 * @param [out] id A pointer to unsigned long which receives the node SDF identifier value.
 *
 * @return DTC code, DTC_NONE if success.
 *
 */
int psync_guid_get_sdf_id( const ps_guid guid, unsigned long * const id );


/**
 * @brief Extract node type from a GUID.
 *
 * @param [in] guid GUID to parse.
 * @param [out] node_type A pointer to \ref ps_node_type which receives the node type value.
 *
 * @return DTC code, DTC_NONE if success.
 *
 */
int psync_guid_get_node_type( const ps_guid guid, ps_node_type * const node_type );


/**
 * @brief Set the node identifier field in a GUID.
 *
 * @param [in] node_id The node identifier value to be copied.
 * @param [out] guid A pointer to \ref ps_guid which receives the node identifier value.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) If success.
 * \li \ref DTC_USAGE If arguments are invalid.
 *
 */
int psync_guid_set_node_id( const ps_identifier node_id, ps_guid * const guid );


/**
 * @brief Set the node SDF identifier field in a GUID.
 *
 * @param [in] sdf_id The node SDF identifier value to be copied.
 * @param [out] guid A pointer to \ref ps_guid which receives the node SDF identifier value.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) If success.
 * \li \ref DTC_USAGE If arguments are invalid.
 *
 */
int psync_guid_set_sdf_id( const unsigned long sdf_id, ps_guid * const guid );


/**
 * @brief Set the node type field in a GUID.
 *
 * @param [in] node_type The node type value to be copied.
 * @param [out] guid A pointer to \ref ps_guid which receives the node type value.
 *
 * @return DTC code:
 * \li \ref DTC_NONE (zero) If success.
 * \li \ref DTC_USAGE If arguments are invalid.
 *
 */
int psync_guid_set_node_type( const ps_node_type node_type, ps_guid * const guid );




#ifdef __cplusplus
}
#endif


#endif	/* POLYSYNC_CORE_H */
