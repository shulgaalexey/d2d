#ifndef _D2D_SCOPE_LOGGER_H
#define _D2D_SCOPE_LOGGER_H

#include <stdio.h>
#include <string>
#include "common.h"

#ifndef __SCOPE_LOGGER_MUTE
#define __SCOPE_LOGGER_MUTE
#endif

// TODO use common logger settings
// TODO move this class to common.h
class ScopeLogger {
	private:
		const std::string _file;
		const std::string _function;
		const int _line;
	public:
		ScopeLogger(const std::string &file,
				const std::string &function,
				const int line) :
			_file(file), _function(function), _line(line) {
			#ifdef __SCOPE_LOGGER_MUTE
				return;
			#endif
			/*printf("%c[%d;%dm", 0x1B, 0, 34);
			printf("SCOPE %s:%d entered %s\n",
					_file.c_str(), _line, _function.c_str());
			printf("%c[%d;%dm", 0x1B, 0, 32);*/
			DBG("SCOPE %s:%d entered %s",
					_file.c_str(), _line, _function.c_str());
		}
		~ScopeLogger() {
			#ifdef __SCOPE_LOGGER_MUTE
				return;
			#endif
			/*printf("%c[%d;%dm", 0x1B, 0, 34);
			printf("SCOPE %s:%d exited %s\n",
					_file.c_str(), _line, _function.c_str());
			printf("%c[%d;%dm", 0x1B, 0, 32);*/
			DBG("SCOPE %s:%d exited %s",
					_file.c_str(), _line, _function.c_str());
		}

	public:
		// TODO remove following two static members
		static void debug(const std::string &str) {
			#ifdef __SCOPE_LOGGER_MUTE
				return;
			#endif
			/*printf("%c[%d;%dm", 0x1B, 0, 34);
			printf("D: %s\n", str.c_str());
			printf("%c[%d;%dm", 0x1B, 0, 32);*/
			DBG("D: %s", str.c_str());
		}
		static void error(const std::string &str) {
			#ifdef __SCOPE_LOGGER_MUTE
				return;
			#endif
			/*printf("%c[%d;%dm", 0x1B, 0, 31);
			printf("E: %s\n", str.c_str());
			printf("%c[%d;%dm", 0x1B, 0, 32);*/
			ERR("E: %s", str.c_str());
		}
};


#define ScopeLogger() const ScopeLogger __sl__(__FILE__, __func__, __LINE__)

#define _D(str) ScopeLogger::debug(str)

#define _E(str) ScopeLogger::error(str)


#endif /*_D2D_SCOPE_LOGGER_H*/
