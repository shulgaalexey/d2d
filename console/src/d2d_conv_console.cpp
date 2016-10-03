#include "d2d_conv_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "common.h"
#include "scope_logger.h"


d2d_conv_console::d2d_conv_console() {
}

d2d_conv_console::~d2d_conv_console() {
}

void d2d_conv_console::print_conv_error(const int error) {
	std::string err;
	switch(error) {
		case CONV_ERROR_NONE:
			return; // Not printing success
		case CONV_ERROR_INVALID_PARAMETER:
			err = "CONV_ERROR_INVALID_PARAMETER";
			break;
		case CONV_ERROR_INVALID_OPERATION:
			err = "CONV_ERROR_INVALID_OPERATION";
			break;
		case CONV_ERROR_OUT_OF_MEMORY:
			err = "CONV_ERROR_OUT_OF_MEMORY";
			break;
		case CONV_ERROR_PERMISSION_DENIED:
			err = "CONV_ERROR_PERMISSION_DENIED";
			break;
		case CONV_ERROR_NOT_SUPPORTED:
			err = "CONV_ERROR_NOT_SUPPORTED";
			break;
		case CONV_ERROR_NO_DATA:
			err = "CONV_ERROR_NO_DATA";
			break;
		default:
			err = "UNKNOWN ERROR CODE";
			break;

	}
	printf("D2D ERROR: %s", err.c_str());
}

int d2d_conv_console::start() {
	ScopeLogger();
	INFO("Starting Convergence Manager");
	// Create convergence manager
	const int error = conv_create(&convergence_manager);
	print_conv_error(error);
	return CONV_ERROR_NONE;
}

int d2d_conv_console::stop() {
	ScopeLogger();
	INFO("Stopping Convergence Manager");
	return CONV_ERROR_NONE;
}


int d2d_conv_console::process(std::vector<std::string> &cmd) {
	ScopeLogger();

	if (cmd.size() < 1) return INCORRECT_COMMAND;

	const std::string cmd_type = cmd[0];

	// Initial dispatching: which part of API is required
	if (cmd_type == "discovery")
		return process_discovery(cmd);
	else if (cmd_type == "device")
		return process_device(cmd);
	else if (cmd_type == "service")
		return process_service(cmd);
	else {
		// VERY BAD: we shouldn't get there
		// It indicates the error in command instruction
		ERR("Incorrect command instruction\n");
		return INCORRECT_COMMAND;
	}

	return CONV_ERROR_NONE;
}

// commands:
// discovery start 15
// discovery stop
int d2d_conv_console::process_discovery(const std::vector<std::string> &cmd) {
	ScopeLogger();
	if (cmd.size() < 1) {
		printf("Incorrect format of discovery command: "
				"no start or stop instruction\n");
		return INCORRECT_COMMAND;
	}

	if (cmd[1] == "start") {

		// Reset device list
		devices.clear();

		int timeout_seconds = 0; // default value

		if (cmd.size() > 2)
			timeout_seconds = atoi(cmd[2].c_str());

		const int error = conv_discovery_start(convergence_manager,
				timeout_seconds, __conv_discovery_cb, this);
		print_conv_error(error);
		return error;
	} else if (cmd[1] == "stop") {
		const int error = conv_discovery_stop(convergence_manager);
		print_conv_error(error);
		return error;
	} else {
		printf("Incorrect instruction of discovery\n");
		return INCORRECT_COMMAND;
	}
	return CONV_ERROR_NONE;
}

void d2d_conv_console::__conv_discovery_cb(conv_device_h device_handle,
		conv_discovery_result_e result, void* user_data) {
	ScopeLogger();


	switch(result) {
	case CONV_DISCOVERY_RESULT_SUCCESS: {

		// Get general device parameters
		char *id = NULL;
		conv_device_get_property_string(device_handle,
				CONV_DEVICE_ID, &id);

		char *name = NULL;
		conv_device_get_property_string(device_handle,
				CONV_DEVICE_NAME, &name);

		char *type = NULL;
		conv_device_get_property_string(device_handle,
				CONV_DEVICE_TYPE, &type);

		// Print device info on console
		printf("\nFound device   %s   handle: %p   type: %s   id: %s\n",
				name, device_handle, type, id);

		// Store discovered device in the global storage
		d2d_conv_console *owner = (d2d_conv_console *)user_data;
		if (owner)
			owner->devices.push_back(device_handle);

		if (id)
			free(id);
		if (name)
			free(name);
		if (type)
			free(type);
		break;
	}
	case CONV_DISCOVERY_RESULT_FINISHED:
		printf("Discovery finished\n");
		break;
	case CONV_DISCOVERY_RESULT_ERROR:
		printf("Discovery error\n");
		break;
	case CONV_DISCOVERY_RESULT_LOST:
		printf("Discovery lost\n");
		break;
	default:
		ERR("Unknown discovery status");
		break;
	}
}


void d2d_conv_console::__conv_service_foreach_cb(
		conv_service_h service_handle, void* user_data) {
	ScopeLogger();
	if (!service_handle)
		return;
	printf("handle: 0x%p", (void *)service_handle);

	{ // Printing service ID
		char *id = NULL;
		int error = conv_service_get_property_string(service_handle,
				CONV_SERVICE_ID, &id);
		print_conv_error(error);
		if (error == CONV_ERROR_NONE) {
			printf("   id: %s", id);
			free(id);
		} else {
			printf("   id: NULL");
		}
	}

	{ // Printing service version
		char *version = NULL;
		int error = conv_service_get_property_string(service_handle,
				CONV_SERVICE_VERSION, &version);
		print_conv_error(error);
		if (error == CONV_ERROR_NONE) {
			printf("   version: %s", version);
			free(version);
		} else {
			printf("   version: NULL");
		}
	}
}

// Accepting handles of types <device_handle_ptr>
conv_device_h d2d_conv_console::get_device_handle_by_handle_string(
		const std::string &handle_str) const {
	ScopeLogger();
	for(size_t i = 0; i < devices.size(); i ++) {
		std::stringstream ss;
		ss << ((void *)devices[i]);
		if (handle_str == ss.str())
			return devices[i];
	}
	return NULL;
}

conv_device_h d2d_conv_console::get_device_handle_by_name(
		const std::string &name) const {
	ScopeLogger();
	for(size_t i = 0; i < devices.size(); i ++) {
		char *cur_name = NULL;
		conv_device_get_property_string(devices[i],
				CONV_DEVICE_NAME, &cur_name);
		if (cur_name && name == cur_name) {
			free(cur_name);
			return devices[i];
		}

		if (cur_name)
			free(cur_name);
	}
	return NULL;
}

conv_service_h d2d_conv_console::get_service_handle_by_handle_string(
		const std::string &handle_str) const {
	ScopeLogger();
	ERR("TODO"); // TODO
	return NULL;
}

// commands:
// device <dhandle1 | TizenA> services
// device <dhandle1 | TizenA> id
// device <dhandle1 | TizenA> name
// device <dhandle1 | TizenA> type
int d2d_conv_console::process_device(const std::vector<std::string> &cmd) {
	ScopeLogger();
	INFO("Starting Convergence Manager");
	if (cmd.size() != 3) {
		printf("Incorrect format of discovery command: "
				"no start or stop instruction\n");
		return INCORRECT_COMMAND;
	}

	// Assuming the device is specified with handle
	conv_device_h device = get_device_handle_by_handle_string(cmd[1]);
	if (!device)
		// Assuming the device is specified with name
		device = get_device_handle_by_name(cmd[1]);

	if (!device) {
		printf("Incorrect handle or name of the device\n");
		return INCORRECT_COMMAND; // No device handle, can not continue
	}

	if (cmd[2] == "services") {
		const int error = conv_device_foreach_service(device,
				__conv_service_foreach_cb, NULL);
		print_conv_error(error);
	} else if (cmd[2] == "id") {
		char *id = NULL;
		const int error = conv_device_get_property_string(device,
				CONV_DEVICE_ID, &id);
		print_conv_error(error);
		if (error == CONV_ERROR_NONE) {
			printf("%s\n", id);
			free(id);
		}
	} else if (cmd[2] == "name") {
		char *name = NULL;
		const int error = conv_device_get_property_string(device,
				CONV_DEVICE_NAME, &name);
		print_conv_error(error);
		if (error == CONV_ERROR_NONE) {
			printf("%s\n", name);
			free(name);
		}
	} else if (cmd[2] == "type") {
		char *type = NULL;
		const int error = conv_device_get_property_string(device,
				CONV_DEVICE_TYPE, &type);
		print_conv_error(error);
		if (error == CONV_ERROR_NONE) {
			printf("%s\n", type);
			free(type);
		}
	}
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service(const std::vector<std::string> &cmd) {
	ScopeLogger();
	if (cmd.size() < 2) {
		printf("Incorrect format of service command: "
				"too few parameters\n");
		return INCORRECT_COMMAND;
	}

	if (cmd[1] == "create") {
		// Creating serviceA
		process_service_create(cmd);
	} else {
		// The arg1 must be service handle
		//     arg2 must be a command, such as connect, start, etc

		if (cmd.size() < 3) {
			printf("Incorrect format of service command: "
					"too few parameters\n");
			return INCORRECT_COMMAND;
		}

		conv_service_h service =
			get_service_handle_by_handle_string(cmd[1]);
		if (!service) {
			printf("Incorrect handle of the service\n");
			return INCORRECT_COMMAND;
		}

		if (cmd[2] == "id")
			return process_service_id(service);
		else if (cmd[2] == "name")
			return process_service_name(service);
		else if (cmd[2] == "properties")
			return process_service_properties(service);
		else if (cmd[2] == "destroy")
			return process_service_destroy(service);
		else if (cmd[2] == "connect")
			return process_service_connect(service, cmd);
		else if (cmd[2] == "disconnect")
			return process_service_disconnect(service, cmd);
		else if (cmd[2] == "start")
			return process_service_start(service, cmd);
		else if (cmd[2] == "stop")
			return process_service_stop(service, cmd);
		else if (cmd[2] == "send")
			return process_service_send(service, cmd);
		else if (cmd[2] == "read")
			return process_service_read(service, cmd);
		else {
			printf("Unknown service command [%s]\n",
					cmd[2].c_str());
			return INCORRECT_COMMAND;
		}

	}
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_create(
		const std::vector<std::string> &cmd){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_id(conv_service_h service){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_name(conv_service_h service){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_properties(conv_service_h service){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_destroy(conv_service_h service){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_connect(conv_service_h service,
		const std::vector<std::string> &cmd){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_disconnect(conv_service_h service,
		const std::vector<std::string> &cmd){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_start(conv_service_h service,
		const std::vector<std::string> &cmd){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_stop(conv_service_h service,
		const std::vector<std::string> &cmd){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_send(conv_service_h service,
		const std::vector<std::string> &cmd){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

int d2d_conv_console::process_service_read(conv_service_h service,
		const std::vector<std::string> &cmd){
	ScopeLogger();
	ERR("TODO"); // TODO
	return CONV_ERROR_NONE;
}

