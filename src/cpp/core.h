#pragma once
#include <string>
#include "main.h"

using namespace std;

extern string originLine;


class core {
public:	
	bool convertToSwitchyOmega();
	bool convertToShadowsocksWindows();
	bool convertToShadowrocket();
	bool convertToBind();
	bool convertToRawList();
	bool decodeGFWList();
	string inputFile;
	string outputFile;
	string preferredDNS;
	int inputMode; // 0 ==> dnsmasq, 1 ==> GFWList
	bool dnsMode = 0; // 0 ==> No customize DNS, 1 ==> Customized DNS
private:
	
	bool checkAccess(string inputFileName, string outputFileName);
	void closeStream();
	void cleanup();
};

namespace dnsmasqList {
	string getRawDomain();
	string getRawDNS();
}

