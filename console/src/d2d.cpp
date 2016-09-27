#include "d2d_conv_manager_fake.h"
#include "scope_logger.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <sstream>
#include <map>



static void process_discovery(const std::vector<std::string> &cmd);
static void process_device(const std::vector<std::string> &cmd);
static void process_service(const std::vector<std::string> &cmd);
static void pre_parse_command(const std::string &input, std::vector<std::string> *cmd);

static conv_h __convergence_manager = NULL;
static std::vector<conv_device_h> __devices;


static std::string trim(const std::string &input) {
	std::string str = input;
	std::stringstream trimmer;
	trimmer << str;
	str.clear();
	trimmer >> str;
	return str;
}

static void show_usage() {
	printf("\n");
	printf("D2D Convergence CLI 0.0.1\n\n");
	printf("usage:\n");
	printf(" help                                               - print instructions\n");
	printf(" discovery start <timeout_seconds>                  - start or stop discovery\n");
	printf(" device <handle> service | id | name | type         - get device services and properties\n");
	printf(" service <handle> id | type | properties            - get service properties\n");
	printf(" service create                                     - create local service\n");
	printf(" service <handle> destroy                           - destroy local service\n");
 // TODO: Consider accessing service as servicename@deviceName
 // for example, App2App@TizenA
	printf(" service <handle> connect | disconnect              - connect or disconnect service\n");
	printf(" service <handle> start | stop [channel] [payload]  - start or stop service\n");
	printf(" service <handle> send | read [channel] [payload]   - send or read data of service\n");
	printf(" quit\n");
}

static void print_conv_error(const int error) {
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

class ConsoleColorSetter {
	public:
		ConsoleColorSetter() {
			printf("%c[%d;%dm", 0x1B, 0, 32);
		}
		~ConsoleColorSetter() {
			printf("%c[%d;%d;%dm", 0x1B, 0, 0, 0);
		}

};

int main(int argc, char *argv[])
{
	ConsoleColorSetter ccs;

	// Create convergence manager
	int error = conv_create(&__convergence_manager);
	print_conv_error(error);


	// Continuously process user input
	std::string input;
	while(true) {
		getline(std::cin, input);
		const std::string command = trim(input);

		if(command.empty()) {
			continue;
		}

		std::vector<std::string> cmd;
		pre_parse_command(command, &cmd);

		if(cmd.empty()) {
			show_usage();
			continue;
		}

		const std::string cmd_type = cmd[0];

		// TODO: link functions to the nodes of behavioural graph
		if(cmd_type == "quit") {
			break;
		} else if(cmd_type == "help")
			show_usage();
		else if(cmd_type == "discovery")
			process_discovery(cmd);
		else if(cmd_type == "device")
			process_device(cmd);
		else if(cmd_type == "service")
			process_service(cmd);
		else {
			// VERY BAD: we shouldn't get there
			// It indicates the error in pre_parse_command function
			printf("Incorrect command\n");
		}
	}

	// Destroy convergence manager
	error = conv_destroy(__convergence_manager);
	print_conv_error(error);

	return 0;
}

static void __conv_discovery_cb(conv_device_h device_handle,
		conv_discovery_result_e result, void* user_data) {
	ScopeLogger();

	switch(result) {
		case CONV_DISCOVERY_RESULT_SUCCESS: {

			// Get general device parameters
			char *id = NULL;
			conv_device_get_property_string(device_handle, CONV_DEVICE_ID, &id);

			char *name = NULL;
			conv_device_get_property_string(device_handle, CONV_DEVICE_NAME, &name);

			char *type = NULL;
			conv_device_get_property_string(device_handle, CONV_DEVICE_TYPE, &type);

			// Print device info on console
			printf("\nFound device   %s   handle: %p   type: %s   id: %s\n",
				name, device_handle, type, id);

			// Store discovered device in the global storage
			__devices.push_back(device_handle);

			if(id)
				free(id);
			if(name)
				free(name);
			if(type)
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
			_E("Unknown discovery status");
			break;
	}
}

// commands:
// discovery start 15
// discovery stop
static void process_discovery(const std::vector<std::string> &cmd) {

	if(cmd.size() < 1) {
		printf("Incorrect format of discovery command: no start or stop instruction\n");
		return;
	}

	if(cmd[1] == "start") {

		// Reset device list
		__devices.clear();

		int timeout_seconds = 0; // default value

		if(cmd.size() > 2)
			timeout_seconds = atoi(cmd[2].c_str());

		const int error = conv_discovery_start(__convergence_manager,
				timeout_seconds, __conv_discovery_cb, NULL);
		print_conv_error(error);
	} else if(cmd[1] == "stop") {
		const int error = conv_discovery_stop(__convergence_manager);
		print_conv_error(error);
	} else {
		printf("Incorrect instruction of discovery\n");
	}
}

static void __conv_service_foreach_cb(conv_service_h service_handle,
		void* user_data) {
	if(!service_handle)
		return;
	printf("handle: 0x%p", (void *)service_handle);

	{ // Printing service ID
		char *id = NULL;
		int error = conv_service_get_property_string(service_handle,
				CONV_SERVICE_ID, &id);
		print_conv_error(error);
		if(error == CONV_ERROR_NONE) {
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
		if(error == CONV_ERROR_NONE) {
			printf("   version: %s", version);
			free(version);
		} else {
			printf("   version: NULL");
		}
	}
	printf("\n D2D >> ");
	fflush(stdout);
}

// Accepting handles of types <device_handle_ptr>
conv_device_h get_device_handle_by_handle_string(const std::string &handle_str) {
	for(size_t i = 0; i < __devices.size(); i ++) {
		std::stringstream ss;
		ss << ((void *)__devices[i]);
		if(handle_str == ss.str())
			return __devices[i];
	}
	return NULL;
}

conv_device_h get_device_handle_by_name(const std::string &name) {
	for(size_t i = 0; i < __devices.size(); i ++) {
		char *cur_name = NULL;
		conv_device_get_property_string(__devices[i], CONV_DEVICE_NAME, &cur_name);
		if(cur_name && name == cur_name) {
			free(cur_name);
			return __devices[i];
		}

		if(cur_name)
			free(cur_name);
	}
	return NULL;
}

// commands:
// device <dhandle1 | TizenA> services
// device <dhandle1 | TizenA> id
// device <dhandle1 | TizenA> name
// device <dhandle1 | TizenA> type
static void process_device(const std::vector<std::string> &cmd) {
	/*printf("  BOOOM: process_device >> ");
	for(size_t i = 0; i < cmd.size(); i ++)
		printf("{%s} ", cmd[i].c_str());
	printf("\n");*/


	if(cmd.size() != 3) {
		printf("Incorrect format of discovery command: no start or stop instruction\n");
		return;
	}

	// Assuming the device is specified with handle
	conv_device_h device = get_device_handle_by_handle_string(cmd[1]);
	if(!device)
		// Assuming the device is specified with name
		device = get_device_handle_by_name(cmd[1]);

	if(!device) {
		printf("Incorrect handle or name of the device\n");
		return; // No device handle, can not continue
	}

	if(cmd[2] == "services") {
		const int error = conv_device_foreach_service(device,
				__conv_service_foreach_cb, NULL);
		print_conv_error(error);
	} else if(cmd[2] == "id") {
		char *id = NULL;
		const int error = conv_device_get_property_string(device,
				CONV_DEVICE_ID, &id);
		print_conv_error(error);
		if(error == CONV_ERROR_NONE) {
			printf("%s\n", id);
			free(id);
		}
	} else if(cmd[2] == "name") {
		char *name = NULL;
		const int error = conv_device_get_property_string(device,
				CONV_DEVICE_NAME, &name);
		print_conv_error(error);
		if(error == CONV_ERROR_NONE) {
			printf("%s\n", name);
			free(name);
		}
	} else if(cmd[2] == "type") {
		char *type = NULL;
		const int error = conv_device_get_property_string(device,
				CONV_DEVICE_TYPE, &type);
		print_conv_error(error);
		if(error == CONV_ERROR_NONE) {
			printf("%s\n", type);
			free(type);
		}
	}

}

static void process_service(const std::vector<std::string> &cmd) {
	printf("  BOOOM: process_service >> ");
	for(size_t i = 0; i < cmd.size(); i ++)
		printf("{%s} ", cmd[i].c_str());
	printf("\n");
}


static void pre_parse_command(const std::string &input, std::vector<std::string> *cmd) {

	std::stringstream ss(input);
	std::string word;
	while (ss >> word) {
		cmd->push_back(word);
	}
}
