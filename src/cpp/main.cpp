#include "util.h"
#include "main.h"

extern string failureReason;
util utils;

string inputFile;
string outputFile;
string preferredDNS;

int main(int argc, char*const argv[]) {

#pragma region Init Variables (buffer for arguments, Mode Selection (Int) ,bool for selecting modes)

	string buf4Arguments;
	int mode;
	bool result;

#pragma endregion

	//Console "Interface" mode
	if (argc == 1) {

#pragma region  Select Running Mode

		system("cls");
		printf("	1. Shadowrocket \n	2. Bind\n	3. Shadowsocks-windows PAC\n	4. SwitchyOmega PAC\n");
		printf("\nPlease enter running mode(function number):");
		cin >> buf4Arguments;

		if (utils.containIgnoreCase(buf4Arguments, "shadowrocket") || buf4Arguments == "1") {mode = 0;}
		else if (utils.containIgnoreCase(buf4Arguments, "bind") || buf4Arguments == "2") { mode = 1; }
		else if (utils.containIgnoreCase(buf4Arguments, "shadowsocks-windows") || buf4Arguments == "3") { mode = 2; }
		else if (utils.containIgnoreCase(buf4Arguments, "switchyomega") || buf4Arguments == "4") { mode = 3; }
		else { utils.reportError("Unknown Mode."); system("pause"); return 1; }

#pragma endregion

#pragma region Collect Information
		system("cls");
		printf("Please Enter origin file name:");
		cin >> utils.inputFile;
		printf("\nPlease Enter output file name:");
		cin >> utils.outputFile;
		
#pragma endregion

#pragma region Main Step

		cout << "------------------\n";
		cout << "Converting...";

		switch (mode) {
		case 0: 
			
			result = utils.convertToShadowrocket();
			break;
		case 1:
			printf("\nPlease Enter Your preferred DNS:");
			cin >> utils.preferredDNS;
			result = utils.convertToBind();
			break;
		case 2:
			result = utils.convertToShadowsocksWindows();
			break;
		case 3:
			result = utils.convertToSwitchyOmega();
			break;
		default:
			utils.reportError("Unkown Mode");
			return 1;
			break;
		}
#pragma endregion

#pragma region Post Run
		
		if (result) {
			cout << "Failed.\n";
			cout << "------Errors------\n";
			cout << failureReason;
			cout << "\n------------------\n";
		}
		else {
			cout << "Success.\n";
		}
		system("pause");
		return 0;
	}

#pragma endregion

	//Show help or show error message
	if (argc == 2) {
		buf4Arguments = argv[1];
		if (buf4Arguments == "--help") {
			printf("convert-china-list by LBYPatrick\n");
			printf("\nHere are parameters available:\n");
			printf("    -s  or --shadowrocket        : convert specified file to a shadowrocket-compatible file.\n");
			printf("    -b  or --bind                : convert specified file to a bind9-compatible file.\n");
			printf("    -sw or --shadowsocks-windows : convert specified file to a PAC file for shadowsocks-windows.\n");
			printf("    -so or --switchyomega        : convert specified file to a PAC file for SwitchyOmega.\n");
			printf("    -i  or --input-file          : specify input file.\n");
			printf("    -o  or --output-file         : specify output file. \n");
			printf("    -d  or --dns                 : specify preferred DNS (only required in bind mode).\n");
			
		}
		else { printf("[ERROR]Unknown parameter. Please check your spell or read the manual by using --help.\n"); return 1; }
	}

	// My Favorite -- commandline mode (Just Like Linux!)
	if (argc > 2) {
#pragma region Collect Arguments
		for (unsigned int i = 1; i < argc; i++) {
			if (argv[i]) {

				buf4Arguments = argv[i];
				if (buf4Arguments == "-s" || buf4Arguments == "--shadowrocket") { mode = 0; }
				if (buf4Arguments == "-b" || buf4Arguments == "--bind") { mode = 1; }
				if (buf4Arguments == "-sw" || utils.containIgnoreCase(buf4Arguments, "--shadowsocks-windows")) { mode = 2; }
				if (buf4Arguments == "-so" || utils.containIgnoreCase(buf4Arguments, "--switchyomega")) { mode = 3; }
				if (buf4Arguments == "-i" || buf4Arguments == "--input-file") { utils.inputFile = argv[i + 1]; }
				if (buf4Arguments == "-o" || buf4Arguments == "--output-file") { utils.outputFile = argv[i + 1]; }
				if (buf4Arguments == "-d" || buf4Arguments == "--dns") { utils.preferredDNS = argv[i + 1]; }
			}
			else { break; }
		}
#pragma endregion

#pragma region Select Running Mode & Run
		
		if (utils.outputFile == "") { utils.reportError("Need to specify output file name."); return 1; }
		if (utils.inputFile == "") { utils.reportError("Need to specify input file name."); return 1; }
		
		printf("Converting...");

		switch (mode) {
		case 0:
			result = utils.convertToShadowrocket();
			break;
		case 1:
			if (preferredDNS == "") { printf("Failed.\n"); utils.reportError("Need to specify preferred DNS."); return 1; }
			else { result = utils.convertToBind(); }
			break;
		case 2:
			result = utils.convertToShadowsocksWindows();
			break;
		case 3:
			result = utils.convertToSwitchyOmega();
			break;
		default:
			utils.reportError("Need to specify running mode (Shadowrocket?Bind 9?Shadowsocks-windows?Raw file?)"); 
			return 1;
			break;
		}

#pragma endregion

#pragma region Post Run
		if (result) {
			cout << "Failed.\n";
			cout << "------Errors------\n";
			cout << failureReason;
			cout << "\n------------------\n";
			return 0;
		}
		if (!result) {
			printf("Done!\n");
			return 0;
		}
#pragma endregion
	}
	
}