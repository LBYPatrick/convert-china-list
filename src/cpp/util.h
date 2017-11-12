#pragma once
#include <string>

using std::string;

namespace util {
	void visualProgress(string message, double currentProgress, double progressGoal);
	void reportError(string content);
	bool containIgnoreCase(string str, string key);
	string toUpperString(string str);
};