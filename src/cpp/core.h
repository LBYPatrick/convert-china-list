#include <string>
#include "main.h"

using namespace std;

string getRawDomain();

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
private:
	
	ifstream Reader;
	ofstream Writer;
	bool checkAccess(string inputFileName, string outputFileName);
	void closeStream();
	void cleanup();
};