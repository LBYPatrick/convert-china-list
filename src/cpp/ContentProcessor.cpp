#include <string>
#include <fstream>
#include "base64.h"
#include "ContentProcessor.h"

using std::string;

	void ContentProcessor::init(string & rawContent, InputType inputType, OutputType outputType) {
		sourceType = inputType;
		targetType = outputType;
		rawFileContent = (inputType == DNSMASQ) ? rawContent : base64::decode(rawContent);
	}

	void ContentProcessor::getRawList() {

		readBuffer = ""; //cleanup
		string writeBuffer = "";

		for (int i = 0; i < (int)rawFileContent.size(); ++i) {

			readBuffer += rawFileContent[i];
			
			if (rawFileContent[i+1] == '\n') {	
					
				if (sourceType == 0) { //dnsmasq
					dnsListCache += dnsmasq.getRawDNS(readBuffer) + '\n';
					domainListCache += dnsmasq.getRawDomain(readBuffer) + '\n';
				}
				else if (sourceType == 1) { //GFWList
					writeBuffer = gfwlist.getRawDomain(readBuffer);
					if (writeBuffer != "") {
						domainListCache += writeBuffer + "\n";
					}
				}
				readBuffer = "";
				i += 1;
			}
		}
	}

	void ContentProcessor::convert() {

		
			  int			rotatorOne			= 0;
			  int			rotatorTwo			= 0;
			  string		readDNSBuffer		= "";
			  string		readDomainBuffer	= "";
		const bool			isDnsmasqAndBind	= (sourceType == DNSMASQ && targetType == BIND);
			  bool			isFirstLine			= true;
			  bool			isDNSCustomed		= customDNS != "";
			  bool			isProxyCustomed		= customProxy != "";

		readBuffer = "";

		//Header
		switch (targetType) {
		case SHADOWROCKET:
			outputContent += "[General]\n\n";
			outputContent += "bypass-system = true";
			outputContent += "\nskip - proxy = 192.168.0.0 / 16, 10.0.0.0 / 8, 172.16.0.0 / 12, localhost, *.local, e.crashlynatics.com, captive.apple.com";
			outputContent += "\nbypass-tun = 10.0.0.0/8,100.64.0.0/10,127.0.0.0/8,169.254.0.0/16,172.16.0.0/12,192.0.0.0/24,192.0.2.0/24,192.88.99.0/24,192.168.0.0/16,198.18.0.0/15,198.51.100.0/24,203.0.113.0/24,224.0.0.0/4,255.255.255.255/32";
			outputContent += "\n\n\n\n";
			outputContent += R"([Rule])";
			outputContent += "\n";
			break;
		case BIND:break;
		case SSWINDOWS:
		case SWITCHYOMEGA:
			outputContent += "var domains = {\n";
			break;
		}

		//Read and process output content line by line 
		while (rotatorTwo < domainListCache.size()) {
			
			if (isDnsmasqAndBind) { //Get DNS if sourceType is DNSMASQ and targetType is bind
				while(true) { //Get DNS
					readDNSBuffer += dnsListCache[rotatorOne];
					if (dnsListCache[rotatorOne + 1] == '\n') { rotatorOne += 2; break; }
					rotatorOne++;
				}
			}
			
			while(true) { //Get Domain
				readDomainBuffer += domainListCache[rotatorTwo];
				if (domainListCache[rotatorTwo + 1] == '\n') { rotatorTwo += 2; break; }
				rotatorTwo++;
			}

			//Format
			switch (targetType) {
				case SHADOWROCKET: 
					outputContent += "\tDOMAIN-KEYWORD,";
					outputContent += readDomainBuffer;
					if(sourceType == 1) outputContent += ",Proxy\n";
					else outputContent += ",Direct\n";
					break;
				
				case BIND:
					outputContent += "zone \"";
					outputContent += readDomainBuffer;
					outputContent += "\" {\n\t type forward;\n\t forwarders {";
					
					if (isDnsmasqAndBind) { 
						if (isDNSCustomed) outputContent += customDNS;
						else			   outputContent += readDNSBuffer; 
					}
					else{
						if(isDNSCustomed) outputContent += customDNS;
						else if(sourceType == 1) outputContent += defaultDNS;
					}				   

					outputContent += ";};\n};\n\n";
					break;
				
				case SSWINDOWS :
				case SWITCHYOMEGA :
					if (!isFirstLine) outputContent += ",\n";
					else			  isFirstLine = false;

					outputContent += "\t\"";
					outputContent += readDomainBuffer;
					outputContent += "\" : 1";
					break;
			}
			readDNSBuffer = "";
			readDomainBuffer = "";
		}

		//End Message
		switch (targetType) {
		
		case SHADOWROCKET : 
			if(sourceType == DNSMASQ) outputContent += "\n\n\nFINAL,Proxy\n"; 
			else outputContent += "\n\n\nFINAL,Direct";
			break;
		case BIND : break;
		case SSWINDOWS : 
		case SWITCHYOMEGA :
			outputContent += "\n\n};\n\n\n";
			outputContent += "var proxy = \"";
			outputContent += (targetType == 2)? "__PROXY__" : (isProxyCustomed? customProxy : defaultProxy);
			outputContent += "\";\n";
			outputContent += "var direct = 'DIRECT;';\n";
			outputContent += "var hasOwnProperty = Object.hasOwnProperty;\n\n";
			outputContent += "function FindProxyForURL(url, host) {\n\n";
			outputContent += "\tvar suffix;\n";
			outputContent += "\tvar pos = host.lastIndexOf('.');\n";
			outputContent += "\tpos = host.lastIndexOf('.', pos - 1);\n\n";
			outputContent += "\twhile(1) {\n";
			outputContent += "\t\tif (pos <= 0) {\n";
			
			outputContent += "\t\t\tif (hasOwnProperty.call(domains, host)) ";
			if(sourceType == 0) outputContent += "return direct;\n";
			else outputContent += "return proxy;\n";
			outputContent += "\t\t\telse ";
			if(sourceType == 0) outputContent += "return proxy;\n";
			else outputContent += "return direct;\n";
			outputContent += "\t\t}\n\n";
			
			outputContent += "\tsuffix = host.substring(pos + 1);\n";

			outputContent += "\tif (hasOwnProperty.call(domains, suffix))";
			if(sourceType == 0) outputContent += "return direct;\n";
			else outputContent += "return proxy;\n";

			outputContent += "\tpos = host.lastIndexOf('.', pos - 1);\n\t}\n}";
			break;
		}
	}

	string inline ContentProcessor::dnsmasqProcessor::getRawDomain(string originLine) {
		string returnBuf = "";
		int startPosition = 8;
		int endPosition = originLine.find_last_of("/");
		
		//for( int counter = originLine.size()-1; counter > (int) originLine.find_last_of("/"); --counter) originLine.erase(counter);
		for (int counter = startPosition; counter < endPosition; counter++) {
			returnBuf += originLine[counter];
		}

		return returnBuf; 
		//return originLine;
	}      

	string inline ContentProcessor::dnsmasqProcessor::getRawDNS(string originLine) {
		
		string returnBuf = "";
		int startPosition = originLine.find_last_of("/") + 1;
		int endPosition = originLine.size();

		for (int counter = startPosition; counter < endPosition; counter++) { 
			returnBuf += originLine[counter]; 
		}

		return returnBuf;
	}

	string ContentProcessor::gfwlistProcessor::getRawDomain(string originLine) {

		int startPosition = 0;
		int endPosition = originLine.size();
		string returnBuffer;

		bool skipRule1 = originLine.find("[") != string::npos; // [Auto xxxx...
		bool skipRule2 = originLine.find("!") != string::npos; // Comments
		bool skipRule3 = originLine.find("@") != string::npos; // Non-proxy Lines
		bool skipRule4 = originLine.find("*") != string::npos;
		bool passRule1 = originLine.find("|") != string::npos; // Proxy Lines
		bool passRule2 = originLine.find(".") != string::npos; // Link-Contained Lines

		if(originLine[endPosition] == '\n') 				    endPosition -= 1;
		if(originLine.find("http://") != string::npos) 			startPosition += 8;
		else if(originLine.find("https://") != string::npos) 	startPosition += 9;

		if (skipRule1 || skipRule2 || skipRule3 || skipRule4) return ""; // Skip unrelated lines

		else if (passRule2) {
			if (passRule1) { startPosition += originLine.find_last_of("|") + 1; }
			
			if (originLine[startPosition] == '\n') startPosition += 1;
			
			for (int i = startPosition; i < endPosition; ++i) { returnBuffer += originLine[i]; }
		}

		//Return
		return returnBuffer;

	}