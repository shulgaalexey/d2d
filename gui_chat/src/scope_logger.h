#ifndef _D2D_SCOPE_LOGGER_H
#define _D2D_SCOPE_LOGGER_H

#include <stdio.h>
#include <string>
#include "d2d-chat.h"


#ifndef __SCOPE_LOGGER_MUTE
//#define __SCOPE_LOGGER_MUTE
#endif

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
			DBG("SCOPE %s:%d entered %s",
					_file.c_str(), _line, _function.c_str());
		}
		~ScopeLogger() {
			#ifdef __SCOPE_LOGGER_MUTE
				return;
			#endif
			DBG("SCOPE %s:%d exited %s",
					_file.c_str(), _line, _function.c_str());
		}
};


#define ScopeLogger() const ScopeLogger __sl__(__FILE__, __func__, __LINE__)

#endif /*_D2D_SCOPE_LOGGER_H*/
