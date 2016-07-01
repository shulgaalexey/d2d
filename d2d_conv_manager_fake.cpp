#include "d2d_conv_manager_fake.h"

#include <stdio.h>
#include <string>

class ScopeLogger {
	public:
		ScopeLogger(const std::string &file,
				const std::string &function,
				const int line) {
			printf("%c[%d;%dm", 0x1B, 0, 34);
			printf("scope logger %s:%d entered %s\n",
					file.c_str(), line, function.c_str());
			printf("%c[%d;%dm", 0x1B, 0, 32);
		}
};

#define ScopeLogger() const ScopeLogger __sl__(__FILE__, __func__, __LINE__)

// D2D Conv Manager
int conv_create(conv_h* handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_destroy(conv_h handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_discovery_start(conv_h handle,
		const int timeout_seconds,
		conv_discovery_cb callback, void* user_data) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_discovery_stop(conv_h handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}


// Channel
int conv_channel_create(conv_channel_h* handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_channel_destroy(conv_channel_h handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_channel_get_string(conv_channel_h handle,
		const char* key, char** value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_channel_set_string(conv_channel_h handle,
		const char* key, const char* value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}


// Payload
int conv_payload_create(conv_payload_h* handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_payload_destroy(conv_payload_h handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_payload_set_string(conv_payload_h handle,
		const char* key, const char* value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_payload_get_string(conv_payload_h handle,
		const char* key, char** value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

//int conv_payload_set_app_control(conv_payload_h handle, const char* key, app_control_h app_control) {
//int conv_payload_get_app_control(conv_payload_h handle, const char* key, app_control_h* app_control) {

int conv_payload_set_byte(conv_payload_h handle,
		const char* key,  int length, unsigned char* value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_payload_get_byte(conv_payload_h handle,
		const char* key,  int* length, unsigned char** value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}


// Service
int conv_service_create(conv_service_h* handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_clone(conv_service_h original_handle,
		conv_service_h* target_handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_destroy(conv_service_h handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_get_property_string(conv_service_h handle,
		const char* key, char** value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_set_property_string(conv_service_h handle,
		const char* key, const char* value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_get_connection_state(conv_service_h handle,
		conv_service_connection_state_e* state) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_get_type(conv_service_h handle, conv_service_e* value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_set_type(conv_service_h handle, conv_service_e value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_connect(conv_service_h handle,
		conv_service_connected_cb callback, void* user_data) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_disconnect(conv_service_h handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_start(conv_service_h handle,
		conv_channel_h channel, conv_payload_h payload) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_read(conv_service_h handle,
		conv_channel_h channel, conv_payload_h payload) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_publish(conv_service_h handle,
		conv_channel_h channel, conv_payload_h payload) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_stop(conv_service_h handle,
		conv_channel_h channel, conv_payload_h payload) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_set_listener_cb(conv_service_h handle,
		conv_service_listener_cb callback, void* user_data) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_service_unset_listener_cb(conv_service_h handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}


// Device
int conv_device_clone(conv_device_h original_handle,
		conv_device_h* target_handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_device_destroy(conv_device_h handle) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_device_get_property_string(conv_device_h handle,
		const char* key, char** value) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int conv_device_foreach_service(conv_device_h handle,
		conv_service_foreach_cb cb, void* user_data) {
	ScopeLogger();
	return CONV_ERROR_NONE;
}
