#include "core.h"
#include "base64.h"
#include "util.h"


extern string failureReason;
string lastTempFile;
string originLine;

ifstream Reader;
ifstream DNSListReader;
ofstream Writer;
ofstream DNSListWriter;

bool core::convertToShadowsocksWindows() {
	
	bool firstLineStat = 1;
	if (checkAccess("rawList.temp", outputFile)) { return 1; } // Check Read/Write Access
		
	
	Writer << R"(var domains = {)" << "\n"; //Init Message

		//Format (Main) START
		while (getline(Reader, originLine)) {
			if (!firstLineStat) {
				Writer << ",\n";
			}
			Writer << R"(")"
				<< originLine
				<< R"(" : 1)";
			firstLineStat = 0;
		}
		//Format (Main) END

		//End Message START
		Writer << "\n\n};\n\n\n"
			<< R"(var proxy = "__PROXY__";)" << "\n"
			<< R"(var direct = 'DIRECT;';)"
			<< "\n"
			<< R"(var hasOwnProperty = Object.hasOwnProperty;)"
			<< "\n\n"
			<< R"(function FindProxyForURL(url, host) {)" << "\n\n"
			<< R"(var suffix;)" << "\n"
			<< R"(    var pos = host.lastIndexOf('.');)" << "\n"
			<< R"(    pos = host.lastIndexOf('.', pos - 1);)" << "\n\n"
			<< R"(    while(1) {)" << "\n"
			<< R"(	if (pos <= 0) {)" << "\n"
			<< R"(		if (hasOwnProperty.call(domains, host)){ return direct;})" << "\n"
			<< R"(		else {return proxy;}})" << "\n\n"
			<< R"( suffix = host.substring(pos + 1);)" << "\n"
			<< R"(if (hasOwnProperty.call(domains, suffix)) {return direct;})" << "\n"
			<< R"(        pos = host.lastIndexOf('.', pos - 1);)" << "\n"
			<< "	}\n}";

		closeStream();
		cleanup();

	return 0;
//End Message END

}

bool core::convertToSwitchyOmega() {


	bool firstLineStat = 1; //A buffer for dealing with semicolon

	if (checkAccess("rawList.temp",outputFile)) { return 1; } //Check Read/Write Access

#pragma region Init Message
		Writer << R"(var domains = {)" << "\n";
#pragma endregion

#pragma region Format (Main)
		while (getline(Reader, originLine)) {
			if (!firstLineStat) {
				Writer << ",\n";
			}
			Writer << R"(")"
				<< originLine
				<< R"(" : 1)";
			firstLineStat = 0;
		}
#pragma endregion

#pragma region End Message

		Writer << "\n\n};\n\n\n"
			<< R"(var proxy = "SOCKS5 127.0.0.1:1080; SOCKS 127.0.0.1:1080";)" << "\n"
			<< R"(var direct = 'DIRECT';)"
			<< "\n"
			<< R"(var hasOwnProperty = Object.hasOwnProperty;)"
			<< "\n\n"
			<< R"(function FindProxyForURL(url, host) {)" << "\n\n"
			<< R"(var suffix;)" << "\n"
			<< R"(    var pos = host.lastIndexOf('.');)" << "\n"
			<< R"(    pos = host.lastIndexOf('.', pos - 1);)" << "\n\n"
			<< R"(    while(1) {)" << "\n"
			<< R"(	if (pos <= 0) {)" << "\n"
			<< R"(		if (hasOwnProperty.call(domains, host)){ return direct;})" << "\n"
			<< R"(		else {return proxy;}})" << "\n\n"
			<< R"( suffix = host.substring(pos + 1);)" << "\n"
			<< R"(if (hasOwnProperty.call(domains, suffix)) {return direct;})" << "\n"
			<< R"(        pos = host.lastIndexOf('.', pos - 1);)" << "\n"
			<< "	}\n}";
		closeStream(); // Close file read/write streams
		cleanup();
	return 0;

#pragma endregion

}


bool core::convertToShadowrocket() {

	if (checkAccess("rawList.temp",outputFile)) { return 1; } // Check Read/Write Access

#pragma region Init Message
		Writer << R"([General])"
			<< "\n\n"
			<< R"(bypass-system = true)"
			<< "\n"
			<< R"(skip - proxy = 192.168.0.0 / 16, 10.0.0.0 / 8, 172.16.0.0 / 12, localhost, *.local, e.crashlynatics.com, captive.apple.com)"
			<< "\n"
			<< R"(bypass-tun = 10.0.0.0/8,100.64.0.0/10,127.0.0.0/8,169.254.0.0/16,172.16.0.0/12,192.0.0.0/24,192.0.2.0/24,192.88.99.0/24,192.168.0.0/16,198.18.0.0/15,198.51.100.0/24,203.0.113.0/24,224.0.0.0/4,255.255.255.255/32)"
			<< "\n\n\n\n"
			<< R"([Rule])"
			<< "\n";
#pragma endregion

#pragma region Format (Main)

		while (getline(Reader, originLine)) {
			Writer << "DOMAIN-KEYWORD,"
				<< originLine
				<< ",Direct\n";
		}
#pragma endregion

#pragma region End Message

		Writer << "\n\n\n"
			<< "FINAL,Proxy";

#pragma endregion
		closeStream(); // Close file read/write streams
		cleanup();
	return 0;
}

bool core::convertToBind() {

	string * DNScache = new string;

	if (!dnsMode) DNSListReader.open("DNSList.temp");

	if (checkAccess("rawList.temp", outputFile)) { return 1; } // Check Read/Write Access

	while (getline(Reader, originLine)) {
		
		if(!dnsMode) getline(DNSListReader, *DNScache);

		Writer << "zone " << "\"" << originLine << "\"" << " {" << endl;
		Writer << "	type forward;" << endl;
		Writer << "	forwarders {";
		
		if (dnsMode) Writer << preferredDNS;
		else Writer << *DNScache;

		Writer << ";};" << endl;
		Writer << "};\n\n";
	}

	closeStream();
	
	if (!dnsMode) DNSListReader.close();

	cleanup();
	return 0;
}

bool core::convertToRawList() {

	bool GFWListMode = inputMode == 1 ? 1 : 0;

	dnsMode = (!GFWListMode && preferredDNS == "") ? 0 : 1;

	if (inputMode == 1) {
		decodeGFWList();
	}

	bool filterBuffer0, filterBuffer1, filterBuffer2, filterBuffer3, filterBuffer4, filterBuffer5; //buffer for filters

	string fileToRead = GFWListMode ? "gfwlist.temp" : inputFile;

	if (checkAccess(fileToRead, "rawList.temp")) return 1;
	
	if (!GFWListMode && preferredDNS == "") {
		DNSListWriter.open("DNSList.temp");
	}
	
	//Convert (GFWList ==> Raw List) START
	if (GFWListMode) {

		while (getline(Reader, originLine)) {
			filterBuffer0 = originLine.find("[") != string::npos; // [Auto xxxx...
			filterBuffer1 = originLine.find("!") != string::npos; // Comments
			filterBuffer2 = originLine.find("@") != string::npos; // Non-proxy Lines
			filterBuffer3 = originLine.find("|") != string::npos; // Proxy Lines
			filterBuffer4 = originLine.find(".") != string::npos; // Link-Contained Lines
			filterBuffer5 = originLine.find("*") != string::npos;

			if (filterBuffer0 || filterBuffer1 || filterBuffer2 || filterBuffer5 ) {
				continue; 
			} // Skip unrelated lines
			
			else if (filterBuffer4) {
				if (filterBuffer3) { 
					for (int i = (originLine.find_last_of("|") + 1); i < originLine.size(); ++i) {
						Writer << originLine[i];
					}
					Writer << "\n";
					continue;
				}
				else {
					for (int i = 0; i < originLine.size(); ++i) {Writer << originLine[i];}
					Writer << "\n";
					continue;
				}
			}
		}
	}
	//Convert (GFWList ==> Raw List) END
	
	else { //dnsmasq-china-list mode
		while (getline(Reader, originLine)) {
			Writer << dnsmasqList::getRawDomain() << "\n";
			if (!dnsMode) {
				DNSListWriter << dnsmasqList::getRawDNS() << "\n";
			}
		}
		if (!dnsMode) {
			DNSListWriter.close();
		}
	}

	closeStream();
	return 0;
}; 

bool core::decodeGFWList() { //Decode GFWList to ASCII, output to "rawlist.temp"
	
	if (checkAccess(inputFile,"gfwlist.temp")) return 1; //Check write access

	while (getline(Reader, originLine)) {
		originLine = base64::decode(originLine); //Decode
		Writer << originLine;
	}

	closeStream();
	return 0;
}

bool core::checkAccess(string inputFileName, string outputFileName) {
	
	Reader.open(inputFileName.c_str());
	Writer.open(outputFileName.c_str());

	if (!Reader.is_open()) { failureReason += "\nProgram cannot find the origin file."; return 1; }
	if (!Writer.is_open()) { failureReason += "\nProgram cannot output to the target file."; return 1; }
	return 0;
}

void core::closeStream() {
	Writer.close();
	Reader.close();
}

void core::cleanup() {

	if (inputMode == 1) {
		util::sysExecute("del gfwlist.temp");
	}
	if (!dnsMode) {
		util::sysExecute("del DNSList.temp");
	}

	util::sysExecute("del rawList.temp");

}

string dnsmasqList::getRawDomain() {

	string returnBuf;
	for (int counter = 8; counter < originLine.find_last_of("/"); counter++) {
		returnBuf += originLine[counter];
	}
	return returnBuf;

}

string dnsmasqList::getRawDNS() {
	
	string returnBuf;
	for (int counter = originLine.find_last_of("/")+1; counter < originLine.size(); counter++) {
		returnBuf += originLine[counter];
	}
	return returnBuf;
}