#include "util.h"
#include "main.h"

extern string failureReason;
util utils;

string inputFile;
string outputFile;
string preferredDNS;

int main(int argc, char*const argv[]) {

#pragma region Init Variables (available arguments, buffer for arguments, bool for selecting modes)

	const unsigned int availableArguments = 5;
	string buf4Arguments;
	bool shadowrocketMode = 0;
	bool bindMode = 0;
	bool result;

#pragma endregion

	//Console "Interface" mode
	if (argc == 1) {

#pragma region  Select Running Mode

		system("cls");
		printf("1. Shadowrocket \n2. Bind\n\n");
		printf("Please enter running mode(number or name):");
		cin >> buf4Arguments;

		if (utils.containIgnoreCase(buf4Arguments, "shadowrocket") || buf4Arguments == "1") {shadowrocketMode = 1;}
		if (utils.containIgnoreCase(buf4Arguments, "bind") || buf4Arguments == "2") { bindMode = 1; }
		else { utils.reportError("Unknown Mode."); system("pause"); return 1; }

#pragma endregion

#pragma region Collect Information
		system("cls");
		printf("Please Enter origin file name:");
		cin >> inputFile;
		printf("\nPlease Enter output file name:");
		cin >> outputFile;
		
		if (bindMode) {
			printf("\nPlease Enter Your preferred DNS:");
			cin >> preferredDNS;
		}
#pragma endregion

#pragma region Main Step

		cout << "------------------\n";
		cout << "Converting...";

		if (bindMode) { result = utils.convertToBind(); }
		if (shadowrocketMode) { result = utils.convertToShadowrocket(); }
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
			printf("    -s or --shadowrocket : convert specified file to a shadowrocket-compatible file.\n");
			printf("    -b or --bind         : convert specified file to a bind9-compatible file.\n");
			printf("    -i or --input-file   : specify input file.\n");
			printf("    -o or --output-file  : specify output file. \n");
			printf("    -d or --dns          : specify preferred DNS (only required in bind mode).\n");

		}
		else { printf("[ERROR]Unknown parameter. Please check your spell or read the manual by using --help.\n"); return 0; }
	}

	// My Favorite -- commandline mode (Just Like Linux!)
	if (argc > 2) {
#pragma region Collect Arguments
		for (unsigned int i = 1; i < argc; i++) {
			if (argv[i]) {

				buf4Arguments = argv[i];
				if (buf4Arguments == "-s" || buf4Arguments == "--shadowrocket") {shadowrocketMode = 1;}
				if (buf4Arguments == "-b" || buf4Arguments == "--bind") {bindMode = 1;}
				if (buf4Arguments == "-i" || buf4Arguments == "--input-file") { inputFile = argv[i + 1]; }
				if (buf4Arguments == "-o" || buf4Arguments == "--output-file") { outputFile = argv[i + 1]; }
				if (buf4Arguments == "-d" || buf4Arguments == "--dns") { preferredDNS = argv[i + 1]; }
			}
			else { break; }
		}
#pragma endregion

#pragma region Select Running Mode & Run
		
		if (outputFile == "") { utils.reportError("Need to specify output file name."); return 1; }
		if (inputFile == "") { utils.reportError("Need to specify input file name."); return 1; }
		
		printf("Converting...");
		
		if (shadowrocketMode) { result = utils.convertToShadowrocket(); }
		
		if (bindMode) { 
			if (preferredDNS == "") { printf("Failed.\n"); utils.reportError("Need to specify preferred DNS."); return 1; }
			else { result = utils.convertToBind(); }
		}
		if (!bindMode && !shadowrocketMode) { utils.reportError("Need to specify running mode (Shadowrocket?Bind 9?)"); return 1; }
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