#ifndef __FAKE_TIZEN_D2D_CONV_MANAGER_H__
#define __FAKE_TIZEN_D2D_CONV_MANAGER_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define CONV_SERVICE_ID "service_id"
#define CONV_SERVICE_VERSION "service_version"
#define CONV_DEVICE_ID "device_id"
#define CONV_DEVICE_NAME "device_name"
#define CONV_DEVICE_TYPE "device_type"


typedef enum {
	CONV_ERROR_NONE = 0,
	CONV_ERROR_INVALID_PARAMETER,
	CONV_ERROR_INVALID_OPERATION,
	CONV_ERROR_OUT_OF_MEMORY,
	CONV_ERROR_PERMISSION_DENIED,
	CONV_ERROR_NOT_SUPPORTED,
	CONV_ERROR_NO_DATA
} conv_error_e;

typedef enum {
	CONV_DISCOVERY_RESULT_ERROR = -1,
	CONV_DISCOVERY_RESULT_SUCCESS = 0,
	CONV_DISCOVERY_RESULT_FINISHED,
	CONV_DISCOVERY_RESULT_LOST
} conv_discovery_result_e;

typedef enum {
	CONV_SERVICE_NONE = -1,
	CONV_SERVICE_APP_TO_APP_COMMUNICATION = 0,
	CONV_SERVICE_REMOTE_APP_CONTROL,
	CONV_SERVICE_REMOTE_INTERACTION
} conv_service_e;

typedef enum {
	CONV_SERVICE_CONNECTION_STATE_NONE = -1,
	CONV_SERVICE_CONNECTION_STATE_CONNECTED = 0,
	CONV_SERVICE_CONNECTION_STATE_NOT_CONNECTED,
	CONV_SERVICE_CONNECTION_STATE_CONNECTING
} conv_service_connection_state_e;


// Handle types
typedef void *conv_h;
typedef void *conv_channel_h;
typedef void *conv_payload_h;
typedef void *conv_service_h;
typedef void *conv_device_h;


// D2D Conv Manager
typedef void(* conv_discovery_cb)(conv_device_h device_handle, conv_discovery_result_e result, void* user_data);
int conv_create(conv_h* handle);
int conv_destroy(conv_h handle);
int conv_discovery_start(conv_h handle, const int timeout_seconds, conv_discovery_cb callback, void* user_data);
int conv_discovery_stop(conv_h handle);

// Channel
int conv_channel_create(conv_channel_h* handle);
int conv_channel_destroy(conv_channel_h handle);
int conv_channel_get_string(conv_channel_h handle, const char* key, char** value);
int conv_channel_set_string(conv_channel_h handle, const char* key, const char* value);

// Payload
int conv_payload_create(conv_payload_h* handle);
int conv_payload_destroy(conv_payload_h handle);
int conv_payload_set_string(conv_payload_h handle, const char* key, const char* value);
int conv_payload_get_string(conv_payload_h handle, const char* key, char** value);
//int conv_payload_set_app_control(conv_payload_h handle, const char* key, app_control_h app_control);
//int conv_payload_get_app_control(conv_payload_h handle, const char* key, app_control_h* app_control);
int conv_payload_set_byte(conv_payload_h handle, const char* key,  int length, unsigned char* value);
int conv_payload_get_byte(conv_payload_h handle, const char* key,  int* length, unsigned char** value);

// Service
typedef void(* conv_service_connected_cb)(conv_service_h service_handle, conv_error_e error, conv_payload_h result, void* user_data);
typedef void(* conv_service_listener_cb)(conv_service_h service_handle, conv_channel_h channel_handle, conv_error_e error, conv_payload_h result, void* user_data);
int conv_service_create(conv_service_h* handle);
int conv_service_clone(conv_service_h original_handle, conv_service_h* target_handle);
int conv_service_destroy(conv_service_h handle);
int conv_service_get_property_string(conv_service_h handle, const char* key, char** value);
int conv_service_set_property_string(conv_service_h handle, const char* key, const char* value);
int conv_service_get_connection_state(conv_service_h handle, conv_service_connection_state_e* state);
int conv_service_get_type(conv_service_h handle, conv_service_e* value);
int conv_service_set_type(conv_service_h handle, conv_service_e value);
int conv_service_connect(conv_service_h handle, conv_service_connected_cb callback, void* user_data);
int conv_service_disconnect(conv_service_h handle);
int conv_service_start(conv_service_h handle, conv_channel_h channel, conv_payload_h payload);
int conv_service_read(conv_service_h handle, conv_channel_h channel, conv_payload_h payload);
int conv_service_publish(conv_service_h handle, conv_channel_h channel, conv_payload_h payload);
int conv_service_stop(conv_service_h handle, conv_channel_h channel, conv_payload_h payload);
int conv_service_set_listener_cb(conv_service_h handle, conv_service_listener_cb callback, void* user_data);
int conv_service_unset_listener_cb(conv_service_h handle);

// Device
typedef void(* conv_service_foreach_cb)(conv_service_h service_handle, void* user_data);
int conv_device_clone(conv_device_h original_handle, conv_device_h* target_handle);
int conv_device_destroy(conv_device_h handle);
int conv_device_get_property_string(conv_device_h handle, const char* key, char** value);
int conv_device_foreach_service(conv_device_h handle, conv_service_foreach_cb cb, void* user_data);


#ifdef __cplusplus
}
#endif/* __cplusplus */

/**
* @}
*/

#endif
