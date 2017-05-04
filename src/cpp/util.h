#pragma once
#include "DnsmasqToBind.h"
namespace util {
	bool convertToBind(string fileName, string preferredDNS);
	void visualProgress(string message, double currentProgress, double progressGoal);
}