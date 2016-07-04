// http://www.math.utah.edu/docs/info/rlman_2.html#SEC36
// http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/

#include "d2d_conv_manager_fake.h"
#include "scope_logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <sstream>
#include <map>



static char** my_completion(const char*, int ,int);
char* my_generator(const char*,int);
char * dupstr (const char*);
void *xmalloc (int);
static bool pre_parse_command(std::vector<std::string> *cmd, std::string *strange_word);

static void process_discovery(const std::vector<std::string> &cmd);
static void process_device(const std::vector<std::string> &cmd);
static void process_service(const std::vector<std::string> &cmd);

static conv_h __convergence_manager = NULL;
static std::vector<conv_device_h> __devices;


//-----------------------------------------------------------------------------

class cmd_node {
	public:
		std::string name;
		std::map<std::string, cmd_node *> children;
	public:
		cmd_node(const std::string &n);
		~cmd_node();
	public:
		void add_child(cmd_node *node);
		void trace();
};

cmd_node::cmd_node(const std::string &n) : name(n) {
}

cmd_node::~cmd_node() {
	for(std::map<std::string, cmd_node *>::iterator it = children.begin();
			it != children.end(); ++it) {
		cmd_node *node = it->second;
		delete node;
	}
	children.clear();
}

void cmd_node::add_child(cmd_node *node) {
	children[node->name] = node;
}

void cmd_node::trace() {

	printf("{%s}: ", name.c_str());
	for(std::map<std::string, cmd_node *>::iterator it = children.begin();
			it != children.end(); ++it) {
		printf("<%s> ", it->first.c_str());
	}
	printf("\n");
}
//-----------------------------------------------------------------------------

cmd_node __cmd_root("D2D");
cmd_node *__selected_cmd = &__cmd_root;
cmd_node *__device_handle_cmd = NULL;
cmd_node *__service_handle_cmd = NULL;
bool __error_cmd = false;

static void init_cmd_root() {
	// Discovery
	__cmd_root.add_child(new cmd_node("discovery"));
	__cmd_root.children["discovery"]->add_child(new cmd_node("stop"));
	__cmd_root.children["discovery"]->add_child(new cmd_node("start"));
	__cmd_root.children["discovery"]->children["start"]->add_child(new cmd_node("0"));
	__cmd_root.children["discovery"]->children["start"]->add_child(new cmd_node("15"));
	__cmd_root.children["discovery"]->children["start"]->add_child(new cmd_node("30"));

	// Device
	__cmd_root.add_child(new cmd_node("device"));
	__cmd_root.children["device"]->add_child(new cmd_node("<dhandle>"));
	__device_handle_cmd = __cmd_root.children["device"]->children["<dhandle>"]; // Pointer on device handle part of command
	__cmd_root.children["device"]->children["<dhandle>"]->add_child(new cmd_node("services"));
	__cmd_root.children["device"]->children["<dhandle>"]->add_child(new cmd_node("id"));
	__cmd_root.children["device"]->children["<dhandle>"]->add_child(new cmd_node("name"));
	__cmd_root.children["device"]->children["<dhandle>"]->add_child(new cmd_node("type"));

	// Service
	__cmd_root.add_child(new cmd_node("service"));
	__cmd_root.children["service"]->add_child(new cmd_node("<shandle>"));
	__device_handle_cmd = __cmd_root.children["service"]->children["<shandle>"]; // Pointer on service handle part of command
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("type"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("id"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("properties"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("create"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("destroy"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("connect"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("disconnect"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("start"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("stop"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("read"));
	__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("send"));
	// Always listening
	//__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("listen"));
	//__cmd_root.children["service"]->children["<shandle>"]->add_child(new cmd_node("stoplisten"));

	// Channel, Payload, Help, Quit
	//__cmd_root.add_child(new cmd_node("channel"));
	//__cmd_root.add_child(new cmd_node("payload"));
	__cmd_root.add_child(new cmd_node("help"));
	__cmd_root.add_child(new cmd_node("quit"));
}

static void __reset_cmd_device_handles() {

	std::map<std::string, cmd_node *> &dhandles =
		__cmd_root.children["device"]->children;

	for(std::map<std::string, cmd_node *>::iterator it = dhandles.begin();
			it != dhandles.end(); ++it) {
		if(it->first != "<dhandle>") {
			dhandles.erase(it);
		}
	}
}
//-----------------------------------------------------------------------------


static std::string trim(const char *buf) {
	std::string str(buf);
	std::stringstream trimmer;
	trimmer << str;
	str.clear();
	trimmer >> str;
	return str;
}

static void show_intro() {
	const unsigned int quick_pause = 30 * 1000; // ms
	const unsigned int long_pause = 50 * 1000; // ms

	printf("\n");

	std::string title = "D2D Convergence CLI 0.0.1 console";

	std::string space;
	for(int i = 0; i < 34; i ++)
		space += " ";
	const std::string trace = space + title + " ";

	for(int i = 0; i < int(space.length() + 1); i ++) {
		printf("\r%s", trace.substr(i, 60).c_str());
		fflush(stdout);
		usleep(long_pause);
	}

	printf("\r");
	fflush(stdout);
	usleep(long_pause);

	for(size_t i = 0; i < title.length(); i ++) {
		printf("%c", title[i]);
		fflush(stdout);
		usleep(quick_pause);
	}

	printf("\n");

	for(int i = 0; i < 34; i ++) {
		printf("*");
		fflush(stdout);
		usleep(quick_pause);
	}

	printf("\n");

	const std::string invite = "Enter your convergence commands...";
	for(size_t i = 0; i < invite.length(); i ++) {
		printf("%c", invite[i]);
		fflush(stdout);
		usleep(quick_pause);
	}

	printf("\n");
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

static void show_launch_keys() {
	printf("\n");
	printf("D2D Convergence CLI 0.0.1\n\n");
	printf("usage:\n");
	printf(" -sdi                                               - skip dramatic intro\n");
	printf(" -h                                                 - help, print instruction\n");
	printf("\n");
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

// TODO disable file browsing for autocompletion


int main(int argc, char *argv[])
{
	ConsoleColorSetter ccs;

	// Analyze CLI app arguments
	bool skip_dramatic_intro = false;
	bool introduce_usage = false;
	for(int i = 1; i < argc; i ++) {
		if(std::string(argv[i]) == "-sdi")
			skip_dramatic_intro = true;
		else if(std::string(argv[i]) == "-h")
			introduce_usage = true;
		else {
			printf("Unknown argument [%s]\n", argv[i]);
			show_launch_keys();
			return 0;
		}
	}

	// Show (or not) the dramatic CLI intro
	if(!skip_dramatic_intro)
		show_intro();

	if(introduce_usage)
		show_usage();

	// Initializa D2D CLI behavioral tree
	init_cmd_root();

	// Initialize auto-completion core
	rl_attempted_completion_function = my_completion;

	// Create convergence manager
	int error = conv_create(&__convergence_manager);
	print_conv_error(error);


	// Continuously process user input
	char *buf = NULL;
	while((buf = readline(" D2D >> ")) != NULL) {
		const std::string origin_input = buf;
		const std::string command = trim(buf);
		free(buf);

		if(command.empty()) {
			continue;
		}

		add_history(origin_input.c_str());

		std::vector<std::string> cmd;
		std::string strange_word;
		if(!pre_parse_command(&cmd, &strange_word) || __error_cmd) {
			if(!strange_word.empty())
				printf("Incorrect command, strange word: %s\n",
						strange_word.c_str());
			else
				printf("Incorrect command\n");
			continue;
		}

		if(cmd.empty()) {
			show_usage();
			continue;
		}

		const std::string cmd_type = cmd[0];

		// TODO: link functions to the nodes of behavioural graph
		if(cmd_type == "quit") {
			__reset_cmd_device_handles();
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
			// Stor discovered device in the global storage
			__devices.push_back(device_handle);

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

			// Add new device name to the behavioral tree
			if(name) {
				__cmd_root.children["device"]->add_child(new cmd_node(name));
				__cmd_root.children["device"]->children[name]->children =
					__cmd_root.children["device"]->children["<dhandle>"]->children;

				std::stringstream ss;
				ss << ((void *)device_handle);
				const std::string handle_str = ss.str();
				__cmd_root.children["device"]->add_child(new cmd_node(handle_str));
				__cmd_root.children["device"]->children[handle_str]->children =
					__cmd_root.children["device"]->children["<dhandle>"]->children;

			}

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

	printf(" D2D >> ");
	fflush(stdout);
}

// commands:
// discovery start 15
// discovery stop
static void process_discovery(const std::vector<std::string> &cmd) {
	/*printf("  BOOOM: process_discovery >> ");
	for(size_t i = 0; i < cmd.size(); i ++)
		printf("{%s} ", cmd[i].c_str());
	printf("\n");*/

	if(cmd.size() < 1) {
		printf("Incorrect format of discovery command: no start or stop instruction\n");
		return;
	}

	if(cmd[1] == "start") {

		// Reset device list
		__devices.clear();

		// Reset behaviour tree
		__reset_cmd_device_handles();

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
			printf("\tid: %s", id);
			free(id);
		} else {
			printf("\tid: NULL");
		}
	}

	{ // Printing service version
		char *version = NULL;
		int error = conv_service_get_property_string(service_handle,
				CONV_SERVICE_VERSION, &version);
		print_conv_error(error);
		if(error == CONV_ERROR_NONE) {
			printf("\tversion: %s", version);
			free(version);
		} else {
			printf("\tversion: NULL");
		}
	}
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

static char** my_completion( const char *text , int start,  int end)
{
	return rl_completion_matches ((char*)text, &my_generator);
}

static bool pre_parse_command(std::vector<std::string> *cmd, std::string *strange_word) {

	// Start command parsing from the beginning of the line
	__selected_cmd = &__cmd_root;

	std::stringstream ss(rl_line_buffer);
	std::string word;

	__error_cmd = false;
	while (ss >> word) {
		if(__error_cmd) {
			// Con not continue: somewhere in the middle of the sentence
			// there is an error of command syntax
			return false;
		}

		// Is this word among children?
		if(__selected_cmd->children.count(word) > 0) {

			// OK: have a part of the command
			__selected_cmd = __selected_cmd->children[word];

			if(cmd)
				cmd->push_back(__selected_cmd->name);

		} else {
			// Something strange is happening
			// If it is a final word, than the user might keep typing
			// If it is in the middle of the sentence, than it is an incorrect command
			__error_cmd = true;
			if(strange_word)
				*strange_word = word;
		}
	}
	return true;
}

// http://www.math.utah.edu/docs/info/rlman_2.html#SEC36
char* my_generator(const char* text, int state)
{
	static std::map<std::string, cmd_node *>::iterator it = __selected_cmd->children.begin();

	if (state == 0) { // First attempt to complete current word
		if(!pre_parse_command(NULL, NULL)) {
			return NULL;
		}
		it = __selected_cmd->children.begin();

		if((__selected_cmd == __device_handle_cmd) || (__selected_cmd == __service_handle_cmd)) {
			if(__selected_cmd->children.size() > 1)
				++it; // First item is a prototype
		}
	}

	if(__selected_cmd->children.empty()) {
		// This is the lates component of the command
		return NULL;
	}


	const int len = strlen (text);
	while(it != __selected_cmd->children.end()) {
		const char *name = it->first.c_str();
		++it;

		if (strncmp (name, text, len) == 0)
			return (dupstr(name));
	}

	return ((char *)NULL); // If no names matched, then return NULL.
}

char * dupstr(const char *s) {
	char *r = (char *) xmalloc ((strlen (s) + 1));
	strcpy(r, s);
	return r;
}

void *xmalloc(int size)
{
	void *buf = malloc(size);
	if (!buf) {
		fprintf (stderr, "Error: Out of memory. Exiting.'n");
		exit (1);
	}

	return buf;
}
