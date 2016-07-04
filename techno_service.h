#ifndef __CONV_TECHNO_SERVICE_H__
#define __CONV_TECHNO_SERVICE_H__

#include <glib.h>
//#include </usr/include/glib-2.0/glib.h>
#include <string>
#include <vector>
#include "d2d_conv_manager_fake.h"

class _conv_handle_mock {
	private:
		pthread_t discovery_thread;
		int discovery_thread_id;
		volatile bool discovering;
		conv_discovery_cb discovery_cb;
		void *discovery_user_data;
		long discovery_start_time;
		long discovery_timeout;
	public:
		_conv_handle_mock();
		~_conv_handle_mock();
	public:
		int start_discovery(const int timeout_seconds,
				conv_discovery_cb callback,
				void* user_data);
		int stop_discovery();
	private:
		//static void *discovery_thread_func(void *arg);
		//static gboolean discovery_on_timer(gpointer user_data);
		static void *discovery_on_timer(void *arg);
};

class _conv_device_mock {
	public:
		std::string id;
		std::string name;
		std::string type;
		std::string host_address;
		std::vector<class _conv_service_handle_mock *> services;
	public:
		_conv_device_mock();
		~_conv_device_mock();
	public:
		int get_property_string(const char* key, char** value);
		int foreach_service(conv_service_foreach_cb cb,
				void* user_data);
};

class _conv_service_handle_mock {
	public:
		conv_service_e service_type;
		std::string data_version;
		std::string data_type;
		std::string data_uri;
	private:
		conv_service_connected_cb connect_callback;
		void *connect_user_data;
		conv_service_listener_cb listener_callback;
		void *listener_user_data;
		bool listener_working;
	public:
		_conv_service_handle_mock();
		~_conv_service_handle_mock();
	public:
		int set_listener_cb(conv_service_listener_cb callback,
				void* user_data);
		int unset_listener_cb();
		int start(conv_channel_h channel_handle, conv_payload_h payload);
		int stop(conv_channel_h channel_handle, conv_payload_h payload);
		int get(conv_channel_h channel_handle, conv_payload_h payload);
		int publish(conv_channel_h channel_handle, conv_payload_h payload);
		int connect(conv_service_connected_cb callback, void* user_data);
		int disconnect();
		int get_type(conv_service_e* value);
		int set_type(conv_service_e value);
		int get_property_string(const char* key, char** value);
		int set_property_string(const char* key, const char* value);
	private:
		static gboolean connect_func(gpointer user_data);
		static gboolean listener_func(gpointer user_data);
		//static gboolean listener_func(gpointer user_data);

};

#endif // __CONV_TECHNO_SERVICE_H__
