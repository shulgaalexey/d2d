// http://www.math.utah.edu/docs/info/rlman_2.html#SEC36
// http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <string>
#include <vector>
#include <sstream>
#include <map>

#define arr_size(a) (int(sizeof(a)/sizeof(a[0])))

static char** my_completion(const char*, int ,int);
char* my_generator(const char*,int);
char * dupstr (const char*);
void *xmalloc (int);

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
	__cmd_root.children["device"]->add_child(new cmd_node("dhandle"));
	__cmd_root.children["device"]->children["dhandle"]->add_child(new cmd_node("services"));
	__cmd_root.children["device"]->children["dhandle"]->add_child(new cmd_node("id"));
	__cmd_root.children["device"]->children["dhandle"]->add_child(new cmd_node("name"));
	__cmd_root.children["device"]->children["dhandle"]->add_child(new cmd_node("type"));

	// Service
	__cmd_root.add_child(new cmd_node("service"));
	__cmd_root.children["service"]->add_child(new cmd_node("shandle"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("type"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("id"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("properties"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("connect"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("disconnect"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("start"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("stop"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("read"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("send"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("listen"));
	__cmd_root.children["service"]->children["shandle"]->add_child(new cmd_node("stoplisten"));

	// Channel, Payload, Help, Quit
	__cmd_root.add_child(new cmd_node("channel"));
	__cmd_root.add_child(new cmd_node("payload"));
	__cmd_root.add_child(new cmd_node("help"));
	__cmd_root.add_child(new cmd_node("quit"));
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

int main()
{
	printf("\n\n");
	printf("D2D Convergence Console started\n");
	printf("enter your commands...\n");

	init_cmd_root();

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
	return rl_completion_matches ((char*)text, &my_generator);
}

bool pre_parse_command() {

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

		} else {
			// Something strange is happening
			// If it is a final word, than the user might keep typing
			// If it is in the middle of the sentence, than it is an incorrect command
			__error_cmd = true;
		}
	}
	return true;
}

// http://www.math.utah.edu/docs/info/rlman_2.html#SEC36
char* my_generator(const char* text, int state)
{
	static std::map<std::string, cmd_node *>::iterator it = __selected_cmd->children.begin();

	if (state == 0) { // First attempt to complete current word
		if(!pre_parse_command()) {
			return NULL;
		}
		it = __selected_cmd->children.begin();
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
