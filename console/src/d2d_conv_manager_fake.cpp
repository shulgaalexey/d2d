#include "d2d_conv_manager_fake.h"
#include "techno_service.h"
#include "scope_logger.h"

#include <stdio.h>
#include <string>


//-----------------------------------------------------------------------------
// D2D Conv Manager
int conv_create(conv_h* handle) {
	ScopeLogger();

	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	*handle = (conv_h)new(std::nothrow) _conv_handle_mock();
	if(!(*handle))
		return CONV_ERROR_OUT_OF_MEMORY;
	return CONV_ERROR_NONE;
}

int conv_destroy(conv_h handle) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_handle_mock *h = (_conv_handle_mock *)handle;
	delete h;
	return CONV_ERROR_NONE;
}

int conv_discovery_start(conv_h handle,
		const int timeout_seconds,
		conv_discovery_cb callback, void* user_data) {
	ScopeLogger();

	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_handle_mock *cnv = (_conv_handle_mock *)handle;
	return cnv->start_discovery(timeout_seconds, callback, user_data);
}

int conv_discovery_stop(conv_h handle) {
	ScopeLogger();

	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_handle_mock *cnv = (_conv_handle_mock *)handle;
	cnv->stop_discovery();
	return CONV_ERROR_NONE;
}
//-----------------------------------------------------------------------------

// Channel
int conv_channel_create(conv_channel_h* handle) {
	ScopeLogger();

	// TODO
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	return CONV_ERROR_NONE;
}

int conv_channel_destroy(conv_channel_h handle) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}

int conv_channel_get_string(conv_channel_h handle,
		const char* key, char** value) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}

int conv_channel_set_string(conv_channel_h handle,
		const char* key, const char* value) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}


// Payload
int conv_payload_create(conv_payload_h* handle) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}

int conv_payload_destroy(conv_payload_h handle) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}

int conv_payload_set_string(conv_payload_h handle,
		const char* key, const char* value) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}

int conv_payload_get_string(conv_payload_h handle,
		const char* key, char** value) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}

//int conv_payload_set_app_control(conv_payload_h handle, const char* key, app_control_h app_control) {
//int conv_payload_get_app_control(conv_payload_h handle, const char* key, app_control_h* app_control) {

int conv_payload_set_byte(conv_payload_h handle,
		const char* key,  int length, unsigned char* value) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}

int conv_payload_get_byte(conv_payload_h handle,
		const char* key,  int* length, unsigned char** value) {
	ScopeLogger();
	// TODO
	return CONV_ERROR_NONE;
}


// Service
int conv_service_create(conv_service_h* handle) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	*handle = (conv_service_h) new(std::nothrow) _conv_service_handle_mock();
	return CONV_ERROR_NONE;
}

int conv_service_clone(conv_service_h original_handle,
		conv_service_h* target_handle) {
	ScopeLogger();
	if(!original_handle|| !target_handle)
		return CONV_ERROR_INVALID_PARAMETER;

	_conv_service_handle_mock *cloned =
		new(std::nothrow) _conv_service_handle_mock();

	_conv_service_handle_mock *origin =
		(_conv_service_handle_mock *)original_handle;

	*cloned = *origin;
	*target_handle = (conv_service_h) cloned;
	return CONV_ERROR_NONE;
}

int conv_service_destroy(conv_service_h handle) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	delete s;
	return CONV_ERROR_NONE;
}

int conv_service_get_property_string(conv_service_h handle,
		const char* key, char** value) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->get_property_string(key, value);
	return CONV_ERROR_NONE;
}

int conv_service_set_property_string(conv_service_h handle,
		const char* key, const char* value) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->set_property_string(key, value);
	return CONV_ERROR_NONE;
}

int conv_service_get_connection_state(conv_service_h handle,
		conv_service_connection_state_e* state) {
	ScopeLogger();
	*state = CONV_SERVICE_CONNECTION_STATE_CONNECTED; // TODO
	return CONV_ERROR_NONE;
}

int conv_service_get_type(conv_service_h handle, conv_service_e* value) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->get_type(value);
	return CONV_ERROR_NONE;
}

int conv_service_set_type(conv_service_h handle, conv_service_e value) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->set_type(value);
	return CONV_ERROR_NONE;
}

int conv_service_connect(conv_service_h handle,
		conv_service_connected_cb callback, void* user_data) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->connect(callback, user_data);
	return CONV_ERROR_NONE;
}

int conv_service_disconnect(conv_service_h handle) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->disconnect();
	return CONV_ERROR_NONE;
}

int conv_service_start(conv_service_h handle,
		conv_channel_h channel, conv_payload_h payload) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->start(channel, payload);
	return CONV_ERROR_NONE;
}

int conv_service_read(conv_service_h handle,
		conv_channel_h channel, conv_payload_h payload) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->get(channel, payload);
	return CONV_ERROR_NONE;
}

int conv_service_publish(conv_service_h handle,
		conv_channel_h channel, conv_payload_h payload) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->publish(channel, payload);
	return CONV_ERROR_NONE;
}

int conv_service_stop(conv_service_h handle,
		conv_channel_h channel, conv_payload_h payload) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->stop(channel, payload);
	return CONV_ERROR_NONE;
}

int conv_service_set_listener_cb(conv_service_h handle,
		conv_service_listener_cb callback, void* user_data) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->set_listener_cb(callback, user_data);
	return CONV_ERROR_NONE;
}

int conv_service_unset_listener_cb(conv_service_h handle) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_service_handle_mock *s = (_conv_service_handle_mock *)handle;
	s->unset_listener_cb();
	return CONV_ERROR_NONE;
}


// Device
int conv_device_clone(conv_device_h original_handle,
		conv_device_h* target_handle) {
	ScopeLogger();
	if(!original_handle || !target_handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_device_mock *d = (_conv_device_mock *)original_handle;
	_conv_device_mock *cloned = new(std::nothrow) _conv_device_mock();
	*cloned = *d;
	*target_handle = (conv_device_h)cloned;
	return CONV_ERROR_NONE;
}

int conv_device_destroy(conv_device_h handle) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_device_mock *d = (_conv_device_mock *)handle;
	delete d;
	return CONV_ERROR_NONE;
}

int conv_device_get_property_string(conv_device_h handle,
		const char* key, char** value) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_device_mock *d = (_conv_device_mock *)handle;
	d->get_property_string(key, value);
	return CONV_ERROR_NONE;
}

int conv_device_foreach_service(conv_device_h handle,
		conv_service_foreach_cb cb, void* user_data) {
	ScopeLogger();
	if(!handle)
		return CONV_ERROR_INVALID_PARAMETER;
	_conv_device_mock *d = (_conv_device_mock *)handle;
	d->foreach_service(cb, user_data);
	return CONV_ERROR_NONE;
}
