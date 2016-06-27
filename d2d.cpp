// http://www.math.utah.edu/docs/info/rlman_2.html#SEC36
// http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <string>
#include <vector>
#include <sstream>

#define arr_size(a) (int(sizeof(a)/sizeof(a[0])))

static char** my_completion(const char*, int ,int);
char* my_generator(const char*,int);
char * dupstr (const char*);
void *xmalloc (int);

/*const char* cmd [] = {
	"discovery",
	"device",
	"service",
	"services",
	"channel",
	"payload",
	"connect",
	"disconnect",
	"start",
	"stop",
	"send",
	"read",
	"listen",
	"unlisten",
	"quit",
	" "
};*/

/*
 * DISCOVERY: discovery
 *                      start _timeout_
 *                      stop
 *
 * DEVICE: device _handle_
 *                         services
 *                         id
 *                         name
 *                         type
 *
 * SERVICE: service _handle_
 *                            connect
 *                            disconnect
 *                            start _channel_ _payload_
 *                            stop _channel_ _payload_
 *                            read _channel_ _payload_
 *                            send _channel_ _payload_
 *                            listen
 *                            stoplisten
 *                            id
 *                            version
 */
const char* d2d_cmd [] = {
	"discovery",
	"device",
	"service",
	"channel",
	"payload",
	"quit",
	" "
};

const char* discovery_cmd [] = {
	"start",
	"stop",
};

const char* device_cmd [] = {
	"services",
	"id",
	"name",
	"type"
};

const char* service_cmd [] = {
	"connect",
	"disconnect",
	"start",
	"stop",
	"read",
	"send",
	"listen",
	"stoplisten",
	"type",
	"id",
	"properties"
};

const char* discovery_timeout_cmd [] = {
	"0",
	"15",
	"30"
};


// This is a fake array, it should be generated dynamically
const char* device_handle_cmd [] = {
	"d1",
	"d2",
	"d3"
};

// This is a fake array, it should be generated dynamically
const char* service_handle_cmd [] = {
	"s1",
	"s2",
	"s3"
};

// This is a fake array, it should be generated dynamically
const char* channel_handle_cmd [] = {
	"c1",
	"c2",
	"c3"
};

// This is a fake array, it should be generated dynamically
const char* payload_handle_cmd [] = {
	"p1",
	"p2",
	"p3"
};


// Current command
std::vector<std::string> __command;

static std::string trim(const char *buf) {
	std::string str(buf);
	std::stringstream trimmer;
	trimmer << str;
	str.clear();
	trimmer >> str;
	return str;
}

int main()
{
	printf("\n\n");
	printf("D2D Convergence Console started\n");
	printf("enter your commands...\n");

	rl_attempted_completion_function = my_completion;

	char *buf = NULL;
	while((buf = readline(" D2D >> "))!=NULL) {
		const std::string command = trim(buf);
		free(buf);

		if(command == "quit")
			break;
		if(!command.empty())
			add_history(command.c_str());
	}

	return 0;
}


static char** my_completion( const char * text , int start,  int end)
{
	//printf("\n\t\t\t   ...my_completion params: [%s], %d, %d\n", text, start, end);
	return rl_completion_matches ((char*)text, &my_generator);
}

char* my_generator(const char* text, int state)
{
	//printf("\n\t\t\t   ...my_generator params: [%s], %d\n", text, state);
	static int list_index = 0, len;

	if (!state) {
		list_index = 0;
		len = strlen (text);
	}

	/*std::stringstream ss(text);
	std::string word;
	__command.clear();
	while (ss >> word)
		__command.push_back(word);
	for(size_t i = 0; i < __command.size(); i ++)
		printf("command part [%s]\n", __command[i].c_str());

	if(__command.size() >= 1) {
		bool command_started = false;
		for(int i = 0; i < arr_size(d2d_cmd); i ++) {
			if(__command[0] == std::string(d2d_cmd[i])) {
				command_started = true;
				printf("command started [%s]\n", d2d_cmd[i]);
				break;
			}
		}
		if(!command_started)
			__command.clear();
	}*/

	/*const char** cmd_array = NULL;
	int n = 0;

	switch(__command.size()) {
		case 0:
			cmd_array = d2d_cmd;
			n = arr_size(d2d_cmd);
			break;
		case 1:
			if(__command[0] == "discovery") {
				cmd_array = discovery_cmd;
				n = arr_size(discovery_cmd);
			} else if(__command[0] == "device") {
				cmd_array = device_cmd;
				n = arr_size(device_cmd);
			} else if(__command[0] == "service") {
				cmd_array = service_cmd;
				n = arr_size(service_cmd);
			} else if(__command[0] == "channel") {
				// TODO
			} else if(__command[0] == "payload") {
				// TODO
			} else {
				// Unsupported state or 'quite'
			}
			break;
		case 2:
			if((__command[0] == "discovery") && (__command[1] == "start")) {
				cmd_array = discovery_timeout_cmd;
				n = arr_size(discovery_timeout_cmd);
			} else if(__command[0] == "device") {
				cmd_array = device_handle_cmd;
				n = arr_size(device_handle_cmd);
			} else if(__command[0] == "service") {
				cmd_array = service_handle_cmd;
				n = arr_size(service_handle_cmd);
			} else if(__command[0] == "channel") {
				// TODO
			} else if(__command[0] == "payload") {
				// TODO
			} else {
				// Unsupported state or 'quite'
			}
			break;
			break;
		default:
			return ((char *)NULL);
	}*/

	const char** cmd_array = d2d_cmd;
	const int n = int(sizeof(d2d_cmd) / sizeof(d2d_cmd[0]));
	while ((list_index < n) && cmd_array[list_index]) {
		const char *name = cmd_array[list_index];
		list_index++;

		if (strncmp (name, text, len) == 0) {
			__command.push_back((dupstr(name)));
			return (dupstr(name));
		}
	}

	/* If no names matched, then return NULL. */
	return ((char *)NULL);

}

char * dupstr (const char* s) {
	char *r;

	r = (char*) xmalloc ((strlen (s) + 1));
	strcpy (r, s);
	return (r);
}

void * xmalloc (int size)
{
	void *buf;

	buf = malloc (size);
	if (!buf) {
		fprintf (stderr, "Error: Out of memory. Exiting.'n");
		exit (1);
	}

	return buf;
}
