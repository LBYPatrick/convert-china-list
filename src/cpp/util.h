#pragma once
#include "main.h"
class util {
public:
	bool convertToBind();
	void visualProgress(string message, double currentProgress, double progressGoal);
	bool convertToShadowrocket();
	void reportError(string content);
	bool containIgnoreCase(string str, string key);
	string toUpperString(string str);
private:
	string getRawDomain(string originLine);
	ifstream Reader;
	ofstream Writer;
	string originLine;
};