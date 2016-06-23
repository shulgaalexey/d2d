// http://www.math.utah.edu/docs/info/rlman_2.html#SEC36
// http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <string>
#include <vector>
#include <sstream>

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
//	printf("\n\t\t\t   ...my_completion params: [%s], %d, %d\n", text, start, end);
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

	static const int n = int(sizeof(d2d_cmd) / sizeof(d2d_cmd[0]));
	while ((list_index < n) && d2d_cmd[list_index]) {
		const char *name = d2d_cmd[list_index];
		list_index++;

		if (strncmp (name, text, len) == 0) {
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
