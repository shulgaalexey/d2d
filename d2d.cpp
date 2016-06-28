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
	"help",
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


static const std::string __error_cmd = "ERR";

// Current command
std::vector<std::string> __command;


bool is_cmd(const std::string &word, const char **table, int size) {
	for(int i = 0; i < size; i ++) {
		std::string s = table[i];
		if(word == s)
			return true;
	}
	return false;
}

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
	while((buf = readline(" D2D >> ")) != NULL) {
		const std::string command = trim(buf);
		free(buf);

		if(command == "quit")
			break;
		if(!command.empty())
			add_history(command.c_str());
	}

	return 0;
}


static char** my_completion( const char *text , int start,  int end)
{
	//printf(">\n\t\t\t   ...my_completion params: [%s], %d, %d\n", text, start, end);
	return rl_completion_matches ((char*)text, &my_generator);
}

void pre_parse_command() {
	//printf(">preparse: %s\n", rl_line_buffer);
	std::stringstream ss(rl_line_buffer);
	std::string word;
	__command.clear();
	while (ss >> word) {
		bool is_correct_cmd = false;
		switch(__command.size()) {
			case 0:
				is_correct_cmd = is_cmd(word, d2d_cmd, arr_size(d2d_cmd));
				break;
			case 1:
				//is_correct_cmd = true;
				is_correct_cmd = is_cmd(word, discovery_cmd, arr_size(discovery_cmd));
				break;
			default:
				is_correct_cmd = true;
				break;
		}
		if(is_correct_cmd)
			__command.push_back(word);
		else
			__command.push_back(__error_cmd);

	}
	if((!__command.empty()) && (__command.back() == __error_cmd)) {
		// We keep adding first word of the command:
		// should ommit this part
		__command.pop_back();
	}

	//for(size_t i = 0; i < __command.size(); i ++)
	//	printf("\n...[%s]\n", __command[i].c_str());
}

char* my_generator_table(const char* text, const char **table, int size, int *index, const char *tab_name) {
	//printf(">my_generator_table %s: %s, %d\n", tab_name, text, *index);
	const int len = strlen (text);
	while (((*index) < size) && table[*index]) {
		const char *name = table[*index];
		(*index)++;
		if (strncmp (name, text, len) == 0) {
			//printf(">suggest: %s\n", name);
			return (dupstr(name));
		}
	}

	/* If no names matched, then return NULL. */
	//printf(">my_generator_table: %s, %d HALTED\n", text, *index);
	return ((char *)NULL);
}

// http://www.math.utah.edu/docs/info/rlman_2.html#SEC36
char* my_generator(const char* text, int state)
{
	static int list_index = 0;

	if (state == 0) { // First attempt to complete current word
		list_index = 0;
		pre_parse_command();
	}

	if (__command.empty()) { // The initial state: no command data entered
		return my_generator_table(text, d2d_cmd, arr_size(d2d_cmd), &list_index, "d2d_cmd1");
	} else if(__command.size() == 1) {
		if(__command[0] == "discovery") {
			//printf("yooooo\n");
			return my_generator_table(text, discovery_cmd, arr_size(discovery_cmd), &list_index, "discovery_cmd");
		}
		if(__command[0] == "device")
			return my_generator_table(text, device_cmd, arr_size(device_cmd), &list_index, "device_cmd");
		if(__command[0] == "service")
			return my_generator_table(text, service_cmd, arr_size(service_cmd), &list_index, "service_cmd");
		else
			return my_generator_table(text, d2d_cmd, arr_size(d2d_cmd), &list_index, "d2d_cmd2");
	} else if(__command.size() == 2) {
//for(size_t i = 0; i < __command.size(); i ++)
//	printf("\n...{%s}\n", __command[i].c_str());
		return my_generator_table(text, d2d_cmd, arr_size(d2d_cmd), &list_index, "d2d_cmd3");
	} else {
//for(size_t i = 0; i < __command.size(); i ++)
//	printf("\n...{%s}\n", __command[i].c_str());
		return my_generator_table(text, d2d_cmd, arr_size(d2d_cmd), &list_index, "d2d_cmd4");
	}

	return NULL;


	#if 0
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
#endif

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
