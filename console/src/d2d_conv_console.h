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
	public:
		static void print_conv_error(const int error);
	private:
		int process_discovery(const std::vector<std::string> &cmd);
		int process_device(const std::vector<std::string> &cmd);
		int process_service(const std::vector<std::string> &cmd);
	public:
		static const int INCORRECT_COMMAND = CONV_ERROR_NO_DATA + 1000;
	private:
		conv_h convergence_manager;
		std::vector<conv_device_h> devices; // Discovered devices
							// TODO use map for this

};

#endif /* __D2D_CONV_CONSOLE_H__ */
