#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//#include <dlog.h>
//#include <glib.h>

#include "common.h"

#include "d2d_conv_console.h"

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

static void tokenize_command(const std::string &input, std::vector<std::string> *cmd) {

	std::stringstream ss(input);
	std::string word;
	while (ss >> word) {
		cmd->push_back(word);
	}
}

#if 0
static GMainLoop* gMainLoop = NULL;
gboolean timeout_func_cb(gpointer data)
{
	if(gMainLoop)
	{
		ALOGD("Main loop will be terminated.");
		g_main_loop_quit((GMainLoop*)data);
	}
	return FALSE;
}
#endif

int main(int argc, char *argv[])
{
#if 0
	// Initialize a GTK main loop
	gMainLoop = g_main_loop_new(NULL, FALSE);
	ERR("D2D Convergence Console Sevice started\n");
	printf("D2D Convergence Sevice started\n");

	// Add callbacks to main loop
	g_timeout_add(3, timeout_func_cb, gMainLoop); // Timeout callback: it will be called after 3000ms.

	// Start the main loop of service
	g_main_loop_run(gMainLoop);



	ERR("D2D Convergence Console Serice is terminated successfully\n");
	printf("D2D Convergence Console Serice is terminated successfully\n");
#endif

	d2d_conv_console cc;

	// Create convergence manager
	const int error = cc.start();
	if (error)
		return error;


	// Continuously process user input
	std::string input;
	while(true) {
		getline(std::cin, input);
		const std::string command = trim(input);

		if(command.empty()) {
			continue;
		}

		std::vector<std::string> cmd;
		tokenize_command(command, &cmd);

		if(cmd.empty()) {
			show_usage();
			continue;
		}

		// Process the command
		cc.process(cmd);
	}

	// Destroy convergence manager
	return cc.stop();
}

//! End of a file
