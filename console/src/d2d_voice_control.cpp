#include "d2d_voice_control.h"
using namespace std;

d2d_voice_control::d2d_voice_control() {
}

d2d_voice_control::~d2d_voice_control() {
}


std::vector<std::string> d2d_voice_control::process_mic() {
	// 1. Get signal from mic

	// 2. Send it to the voice server

	// 3. Receive recognized text
	vector<string> cmd;

	// just for test
	cmd.push_back("discovery");
	cmd.push_back("start");
	cmd.push_back("60");

	return cmd;
}
