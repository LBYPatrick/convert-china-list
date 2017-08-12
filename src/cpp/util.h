#pragma once
#include "main.h"
namespace util {
	void visualProgress(string message, double currentProgress, double progressGoal);
	void reportError(string content);
	bool containIgnoreCase(string str, string key);
	string toUpperString(string str);
	void sysExecute(string cmd);
};