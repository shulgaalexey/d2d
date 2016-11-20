#ifndef __D2D_CONV_VOICE_CONTROL_H__
#define __D2D_CONV_VOICE_CONTROL_H__

#include <string>
#include <vector>

class d2d_voice_control {
	public:
		d2d_voice_control();
		~d2d_voice_control();
	public:
		std::vector<std::string> process_mic();
		void say(const std::vector<std::string> &text);
};


#endif /*__D2D_CONV_VOICE_CONTROL_H__*/


