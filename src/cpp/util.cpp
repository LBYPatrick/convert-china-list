#include "util.h"
#include "main.h"

string failureReason;

bool util::convertToBind() {

#pragma region Check Read Access

	Reader.open(inputFile.c_str());
	if (Reader.is_open()) {

#pragma endregion

#pragma region Check Write Access
		Writer.open(outputFile.c_str());
		if (!Writer.is_open()) {
			failureReason += "\nProgram cannot output to the target file.";
			return 1;
		}
#pragma endregion

#pragma region Format (Main)
		
		while (getline(Reader, originLine)) {

			Writer << "zone " << "\"" << getRawDomain(originLine) << "\"" << " {" << endl;
			Writer << "	type forward;" << endl;
			Writer << "	forwarders {" << preferredDNS << ";};" << endl;
			Writer << "};\n\n";
			}

#pragma endregion

#pragma region Close streams 
		
		Writer.close();
		Reader.close();
	}

#pragma endregion

	else {
		failureReason +=  "\nProgram cannot find the origin file.";
		return 1;
		}
	return 0;
}

void util::visualProgress(string message, double currentProgress, double progressGoal) {
	system("cls");
	
	printf("%s\n[",message.c_str());
	currentProgress++;
	for (int i = 0; i <= (currentProgress); i++) {printf(">");} 
	
	for (int i = 0; i < (progressGoal - currentProgress); i++) {printf(" ");}

	if (currentProgress + (progressGoal - currentProgress) < progressGoal) {printf(" ");}

	printf("]%ls%s%f",currentProgress,R"(\)",progressGoal);

	if (currentProgress == progressGoal) {printf("\n\n\n");}
};

bool util::convertToShadowrocket() {

#pragma region Check Read Access
	Reader.open(inputFile.c_str());
	if (Reader.is_open()) {
#pragma endregion

#pragma region Check Write Access
		Writer.open(outputFile.c_str());
		if (!Writer.is_open()) {
			failureReason += "\nProgram cannot output to the target file.";
			return 1;
		}
#pragma endregion

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
			Writer << "DOMAIN-SUFFIX,"
				<< getRawDomain(originLine)
				<< ",Direct\n";
		}
#pragma endregion

#pragma region End Message + Close streams

		Writer << "\n\n\n"
			<< "FINAL,Proxy";
		Reader.close();
		Writer.close();
	}

#pragma endregion

	else {
		failureReason += "\nProgram cannot find the origin file.";
		return 1;
	}
	return 0;
}

bool util::convertToShadowsocksWindows() {
	bool firstLineStat = 1;
#pragma region Check Read Access
	Reader.open(inputFile.c_str());
	if (Reader.is_open()) {
#pragma endregion

#pragma region Check Write Access
		Writer.open(outputFile.c_str());
		if (!Writer.is_open()) {
			failureReason += "\nProgram cannot output to the target file.";
			return 1;
		}
#pragma endregion

#pragma region Init Message
		Writer << R"(var domains = {)"<<"\n";
#pragma endregion

#pragma region Format (Main)
		while (getline(Reader, originLine)) {
			if (!firstLineStat) {
				Writer << ",\n";
			}
			Writer << R"(")" 
				<< getRawDomain(originLine) 
				<< R"(" : 1)";
			firstLineStat = 0;
		}
#pragma endregion

#pragma region End Message + Close streams

		Writer << "\n\n};\n\n\n"
			<< R"(var proxy = "__PROXY__";)" << "\n"
			<< R"(var direct = 'DIRECT;';)"
			<< "\n"
			<< R"(var hasOwnProperty = Object.hasOwnProperty;)"
			<< "\n\n"
			<< R"(function FindProxyForURL(url, host) {)" << "\n\n"
			<< R"(var suffix;)" << "\n"
			<< R"(    var pos = host.lastIndexOf('.');)" << "\n"
			<< R"(    pos = host.lastIndexOf('.', pos - 1);)"<<"\n\n"
			<< R"(    while(1) {)" << "\n"
			<< R"(	if (pos <= 0) {)" << "\n"
			<< R"(		if (hasOwnProperty.call(domains, host)){ return direct;})" << "\n"
			<< R"(		else {return proxy;}})" << "\n\n"
			<< R"( suffix = host.substring(pos + 1);)" << "\n"
			<< R"(if (hasOwnProperty.call(domains, suffix)) {return direct;})" << "\n"
			<< R"(        pos = host.lastIndexOf('.', pos - 1);)" << "\n"
			<< "	}\n}";

		    
		Reader.close();
		Writer.close();
	}

#pragma endregion

	else {
		failureReason += "\nProgram cannot find the origin file.";
		return 1;
	}
	return 0;
}

string util::getRawDomain(string originLine) {
	string returnBuf;
	for (int counter = 8; counter < (originLine.length() - 16); counter++) {
		returnBuf += originLine[counter];
	}
	return returnBuf;
}

void util::reportError(string content) {
	printf("[ERROR]%s\n", content.c_str());
}

string util::toUpperString(string str) {
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		*it = (char)toupper(*it);
	return str;
}

bool util::containIgnoreCase(string str, string key) {
	return toUpperString(str).find(toUpperString(key)) != string::npos;
}