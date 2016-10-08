#ifndef __D2D_CONV_CONSOLE_H__
#define __D2D_CONV_CONSOLE_H__


#include <string>
#include <vector>
#include <map>
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

		int process_service_create(const std::vector<std::string> &cmd);
		int process_service_id(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_version(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_name(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_property(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_type(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_constate(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_destroy(conv_service_h service);
		int process_service_connect(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_disconnect(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_start(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_stop(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_send(conv_service_h service,
				const std::vector<std::string> &cmd);
		int process_service_read(conv_service_h service,
				const std::vector<std::string> &cmd);

		conv_device_h get_device_handle_by_handle_string(
				const std::string &handle_str) const;
		conv_device_h get_device_handle_by_name(
				const std::string &name) const;

		conv_service_h get_service_handle_by_handle_string(
				const std::string &handle_str) const;

		static void __conv_discovery_cb(conv_device_h device_handle,
			conv_discovery_result_e result, void* user_data);
		static void __conv_service_foreach_cb(
				conv_service_h service_handle, void* user_data);


		static conv_service_e str_to_service_type(
				const std::string str);
		static std::string service_type_to_str(
				const conv_service_e type);
		static std::string service_constate_to_str(
				const conv_service_connection_state_e state);

		void store_local_service(conv_service_h service);

	public:
		static const int INCORRECT_COMMAND = CONV_ERROR_NO_DATA + 1000;
	private:
		conv_h convergence_manager;
		std::vector<conv_device_h> devices; // Discovered devices
							// TODO use map for this
		std::map<conv_service_h, conv_service_h> local_services;

};

#endif /* __D2D_CONV_CONSOLE_H__ */
