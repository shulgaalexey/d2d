#ifndef __D2D_CONV_CONSOLE_H__
#define __D2D_CONV_CONSOLE_H__


#include <string>
#include <vector>
#include <d2d_conv_manager.h>

class d2d_conv_console {
	public:
		d2d_conv_console();
		~d2d_conv_console();
	public:
		int start();
		int stop();
		int process(std::vector<std::string> &cmd);
	private:
		conv_h convergence_manager;
		std::vector<conv_device_h> devices; // Discovered devices
							// TODO use map for this

};

#endif
