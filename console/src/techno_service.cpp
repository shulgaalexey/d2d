#include <pthread.h>
#include <unistd.h>
#include <sstream>

#include "techno_service.h"
#include "scope_logger.h"


_conv_handle_mock::_conv_handle_mock()
	: discovery_thread(0)
	  , discovery_thread_id(0)
	  , discovering(false)
	  , discovery_cb(NULL)
	  , discovery_user_data(NULL)
	  , discovery_start_time(0)
	  , discovery_timeout(0)
{
	ScopeLogger();
}

_conv_handle_mock::~_conv_handle_mock()
{
	ScopeLogger();
}

void *_conv_handle_mock::discovery_on_timer(void *arg)
{
	ScopeLogger();
	_conv_handle_mock *owner = (_conv_handle_mock *)arg;
	if (!owner)
		pthread_exit(NULL);

	{ // Immitating discovery iquiry
		_D("\nDiscovery will be going in...");
		sleep(1);
		_D("3");
		sleep(1);
		_D("2");
		sleep(1);
		_D("1");
		sleep(1);
		_D("BOOOM!!!");
	}

	while(true) {
		if (owner->discovery_timeout > 0)  {
			GTimeVal cur_time;
			g_get_current_time (&cur_time);
			long cur_discovery =
				cur_time.tv_sec - owner->discovery_start_time;
			if (cur_discovery > owner->discovery_timeout) {
				_D("DISCOVERY TIMEOUT");
				_D("DISCOVERY TIMEOUT");
				_D("DISCOVERY TIMEOUT");
				_D("DISCOVERY TIMEOUT");

				if (owner->discovery_cb)
					owner->discovery_cb(NULL,
						CONV_DISCOVERY_RESULT_FINISHED,
						owner->discovery_user_data);
				pthread_exit(NULL);
			}
		}


		if (owner->discovering) {

			// Create new instance of device
			_conv_device_mock *d = new _conv_device_mock();

			// Invoke discovery callback
			if (owner->discovery_cb)
				owner->discovery_cb((conv_device_h)d,
						CONV_DISCOVERY_RESULT_SUCCESS,
						owner->discovery_user_data);
		} else {

			// Invoke discovery finish callback
			if (owner->discovery_cb)
				owner->discovery_cb(NULL,
						CONV_DISCOVERY_RESULT_FINISHED,
						owner->discovery_user_data);
			break;
		}
		sleep(2);
	}
	pthread_exit(NULL);
}

int _conv_handle_mock::start_discovery(const int timeout_seconds,
		conv_discovery_cb callback, void* user_data)
{
	ScopeLogger();

	_D("STARTING DISCOVERY");

	discovering = true;
	discovery_cb = callback;
	discovery_user_data = user_data;

	GTimeVal cur_time;
	g_get_current_time(&cur_time);
	discovery_start_time = cur_time.tv_sec;
	discovery_timeout = timeout_seconds;

	_D("Starting timer...");

	pthread_t thread = 0;
	const int ret = pthread_create(&thread, NULL, discovery_on_timer, this);
	if (ret) {
		std::stringstream ss;
		ss << "ERROR! pthread_create() return code: " << ret;
		_E(ss.str());
		return ret;
	}

	_D("Started timer");
	sleep(1);

	return CONV_ERROR_NONE;
}

int _conv_handle_mock::stop_discovery()
{
	ScopeLogger();

	if (discovery_cb)
		discovery_cb(NULL,
				CONV_DISCOVERY_RESULT_FINISHED,
				discovery_user_data);

	discovering = false;
	discovery_cb = NULL;
	discovery_user_data = NULL;

	discovery_start_time = 0;
	discovery_timeout = 0;

	return CONV_ERROR_NONE;
}

///////////////////////////////////////////////////////////////////////////////

static int __device_cnt = 0;

_conv_device_mock::_conv_device_mock()
{
	ScopeLogger();
	{
		static const char *letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::stringstream ss;
		ss << "Tizen";
		int cur_device_cnt = __device_cnt;
		do { // FIXME There is a bug in this key generator
			ss << letters[cur_device_cnt % 26];
			cur_device_cnt /= 26;
		} while(cur_device_cnt);
		name = ss.str();
	}

	{
		std::stringstream ss;
		ss << "MyDeviceId_" << __device_cnt;
		id = ss.str();
	}

	{
		std::stringstream ss;
		ss << "Addr_127.0.0." << int(__device_cnt % 127 + 1);
		host_address = ss.str();
	}

	type = "mobile";

	_conv_service_handle_mock *s = new _conv_service_handle_mock();
	s->service_type = CONV_SERVICE_REMOTE_APP_CONTROL;
	s->data_version = "1";
	s->data_type = "remo-con-data";
	s->data_uri = "org.tizen/remocon";

	services.push_back(s);

	__device_cnt ++;
}

_conv_device_mock::~_conv_device_mock()
{
	ScopeLogger();
	for(size_t i = 0; i < services.size(); i ++)
		delete services[i];
	services.clear();
}

int _conv_device_mock::get_property_string(const char* key, char** value)
{
	ScopeLogger();

	if (g_strcmp0(key, CONV_DEVICE_ID) == 0) {
		*value = g_strdup(id.c_str());
	} else 	if (g_strcmp0(key, CONV_DEVICE_NAME) == 0) {
		*value = g_strdup(name.c_str());
	} else 	if (g_strcmp0(key, CONV_DEVICE_TYPE) == 0) {
		*value = g_strdup(type.c_str());
	} else 	if (g_strcmp0(key, "host_address") == 0) {
		*value = g_strdup(host_address.c_str());
	} else {
		*value = g_strdup("Unknown device property key");
	}

	return CONV_ERROR_NONE;
}

int _conv_device_mock::foreach_service(conv_service_foreach_cb cb,
		void* user_data)
{
	ScopeLogger();
	for(size_t i = 0; i < services.size(); i ++)
		cb((conv_service_h)services[i], user_data);
	return CONV_ERROR_NONE;
}

///////////////////////////////////////////////////////////////////////////////

_conv_service_handle_mock::_conv_service_handle_mock()
	: connect_callback(NULL)
	  , connect_user_data(NULL)
	  , listener_callback(NULL)
	  , listener_user_data(NULL)
	  , listener_working(false)
{
	ScopeLogger();
}

_conv_service_handle_mock::~_conv_service_handle_mock()
{
	ScopeLogger();
}

gboolean _conv_service_handle_mock::listener_func(gpointer user_data)
{
	ScopeLogger();

	_conv_service_handle_mock *owner =
		(_conv_service_handle_mock *)user_data;
	if (!owner || !owner->listener_callback || !owner->listener_working)
		return FALSE;

	conv_channel_h channel = NULL;
	conv_channel_create(&channel);
	conv_channel_set_string(channel, "my_chan", "123");

	conv_payload_h payload = NULL;
	conv_payload_create(&payload);
	conv_payload_set_string(payload, "my_payload", "good-listener-progress");

	owner->listener_callback((conv_service_h)owner, channel,
			CONV_ERROR_NONE, payload, owner->listener_user_data);

	return TRUE;
}

int _conv_service_handle_mock::set_listener_cb(
		conv_service_listener_cb callback,
		void* user_data)
{
	ScopeLogger();
	listener_callback = callback;
	listener_user_data = user_data;
	listener_working = true;
	g_timeout_add_seconds (1, listener_func, this);
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::unset_listener_cb()
{
	ScopeLogger();
	listener_callback = NULL;
	listener_user_data = NULL;
	listener_working = false;
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::start(conv_channel_h channel_handle,
		conv_payload_h payload)
{
	ScopeLogger();

	// TODO
	/*if (channel_handle) {
		_conv_channel_handle *c = (_conv_channel_handle *)channel_handle;
		_D(" ... CHANNEL: %s", c->jchannel.str().c_str());
	}

	if (payload) {
		_conv_payload_handle *p = (_conv_payload_handle *)payload;
		_D(" ... PAYLOAD: %s", p->jpayload.str().c_str());
	}*/

	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::stop(conv_channel_h channel_handle,
		conv_payload_h payload)
{
	ScopeLogger();

	// TODO
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::get(conv_channel_h channel_handle,
		conv_payload_h payload)
{
	ScopeLogger();

	// TODO
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::publish(conv_channel_h channel_handle,
		conv_payload_h payload)
{
	ScopeLogger();

	// TODO
	return CONV_ERROR_NONE;
}

gboolean _conv_service_handle_mock::connect_func(gpointer user_data)
{
	ScopeLogger();

	_conv_service_handle_mock *owner =
		(_conv_service_handle_mock *)user_data;
	if (!owner || !owner->connect_callback)
		return FALSE;

	conv_payload_h result = NULL;
	conv_payload_create(&result);
	conv_payload_set_string(result, "connection", "good-connection-lol");
	owner->connect_callback((conv_service_h)owner,
			CONV_ERROR_NONE, result, owner->connect_user_data);

	owner->connect_callback = NULL;
	owner->connect_user_data = NULL;

	return FALSE;
}

int _conv_service_handle_mock::connect(conv_service_connected_cb callback,
		void* user_data)
{
	ScopeLogger();
	connect_callback = callback;
	connect_user_data = user_data;
	g_timeout_add_seconds (1, connect_func, this);
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::disconnect()
{
	ScopeLogger();
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::get_type(conv_service_e* value)
{
	ScopeLogger();
	*value = service_type;
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::set_type(conv_service_e value)
{
	ScopeLogger();
	service_type = value;
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::get_property_string(const char* key,
		char** value)
{
	ScopeLogger();

	if (g_strcmp0(key, "version") == 0) {
		*value = g_strdup(data_version.c_str());
	} else 	if (g_strcmp0(key, "type") == 0) {
		*value = g_strdup(data_type.c_str());
	} else 	if (g_strcmp0(key, CONV_SERVICE_ID) == 0) {
		*value = g_strdup(data_uri.c_str());
	} else {
		*value = g_strdup("Unknown service property key");
	}
	return CONV_ERROR_NONE;
}

int _conv_service_handle_mock::set_property_string(const char* key,
		const char* value)
{
	ScopeLogger();
	return CONV_ERROR_NONE;
}
