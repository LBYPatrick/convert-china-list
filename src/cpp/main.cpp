#include "util.h"
#include "main.h"
#include "core.h"

extern string failureReason;
core coreExecute;

string inputFile;
string outputFile;
string preferredDNS;

int main(int argc, char*const argv[]) {

#pragma region Init Variables (buffer for Parameters, Mode Selection (Int) ,bool for selecting modes)

	string buf4Parameters;
	int inputMode;
	int outputMode;
	bool result;

#pragma endregion

	//Console "Interface" mode
	if (argc == 1) {

#pragma region  Select Running Mode

		system("cls");
		printf("\n	1. dnsmasq \n	2. GFWList\n");
		printf("\nPlease enter type of the source file:");
		cin >> buf4Parameters;

		if (util::containIgnoreCase(buf4Parameters, "dnsmasq") || buf4Parameters == "1") { inputMode = 0; }
		else if (util::containIgnoreCase(buf4Parameters, "gfwlist") || buf4Parameters == "2") { inputMode = 1; }
		else {
			util::reportError("Unknown Mode.");
			system("pause");
			return 1;
		}


		system("cls");
		printf("\n	1. Shadowrocket \n	2. Bind\n	3. Shadowsocks-windows PAC\n	4. SwitchyOmega PAC\n");
		printf("\nPlease enter running mode(function number):");
		cin >> buf4Parameters;

		if (util::containIgnoreCase(buf4Parameters, "shadowrocket")              || buf4Parameters == "1") { outputMode = 0; }
		else if (util::containIgnoreCase(buf4Parameters, "bind")                 || buf4Parameters == "2") { outputMode = 1; }
		else if (util::containIgnoreCase(buf4Parameters, "shadowsocks-windows")  || buf4Parameters == "3") { outputMode = 2; }
		else if (util::containIgnoreCase(buf4Parameters, "switchyomega")         || buf4Parameters == "4") { outputMode = 3; }
		else { 
			util::reportError("Unknown Mode."); 
			system("pause");
			return 1; 
		}		

#pragma endregion

#pragma region Collect Information
		system("cls");
		printf("Please Enter origin file name:");
		cin >> coreExecute.inputFile;
		printf("\nPlease Enter output file name:");
		cin >> coreExecute.outputFile;
		
#pragma endregion

#pragma region Main Step

		cout << "------------------\n";
		cout << "Converting...";

		coreExecute.inputMode = inputMode;
		coreExecute.convertToRawList();

		switch (outputMode) {
		case 0: 
			
			result = coreExecute.convertToShadowrocket();
			break;
		case 1:
			printf("\nPlease Enter Your preferred DNS:");
			cin >> coreExecute.preferredDNS;
			result = coreExecute.convertToBind();
			break;
		case 2:
			result = coreExecute.convertToShadowsocksWindows();
			break;
		case 3:
			result = coreExecute.convertToSwitchyOmega();
			break;
		default:
			util::reportError("Unkown Mode");
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
		buf4Parameters = argv[1];
		if (buf4Parameters == "--help") {
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
#pragma region Collect Parameters
		for (unsigned int i = 1; i < argc; i++) {
			if (argv[i]) {

				buf4Parameters = argv[i];
				
				if (buf4Parameters == "-c" || buf4Parameters == "--convert-to") {
					buf4Parameters = argv[i + 1];
					if (util::containIgnoreCase(buf4Parameters, "shadowrocket")              || buf4Parameters == "0") { outputMode = 0; }
					else if (util::containIgnoreCase(buf4Parameters, "bind")                 || buf4Parameters == "1") { outputMode = 1; }
					else if (util::containIgnoreCase(buf4Parameters, "shadowsocks-windows")  || buf4Parameters == "2") { outputMode = 2; }
					else if (util::containIgnoreCase(buf4Parameters, "switchyomega")         || buf4Parameters == "3") { outputMode = 3; }
					else { util::reportError("Sorry, we haven't finished the support for the output type you specified."); return 1; }
				}
				
				else if (buf4Parameters == "-s" || buf4Parameters == "--source-type") {
					buf4Parameters = argv[i + 1];
					if (util::containIgnoreCase(buf4Parameters, "dnsmasq")                   || buf4Parameters == "0") { inputMode = 0; }
					else if (util::containIgnoreCase(buf4Parameters, "gfwlist")              || buf4Parameters == "1") { inputMode = 1; }
					else { util::reportError("Sorry, we haven't finished the support for the input type you specified."); return 1; }
				}

				else if (buf4Parameters == "-i" || buf4Parameters == "--input-file") { coreExecute.inputFile = argv[i + 1]; }
				else if (buf4Parameters == "-o" || buf4Parameters == "--output-file") { coreExecute.outputFile = argv[i + 1]; }
				else if (buf4Parameters == "-d" || buf4Parameters == "--dns") { coreExecute.preferredDNS = argv[i + 1]; }
			}
		}
#pragma endregion

#pragma region Select Running Mode & Run
		
		if (coreExecute.outputFile == "") { util::reportError("Need to specify output file name."); return 1; }
		if (coreExecute.inputFile == "") { util::reportError("Need to specify input file name."); return 1; }

		printf("Converting...");

		coreExecute.inputMode = inputMode;
		coreExecute.convertToRawList();

		switch (outputMode) {
		case 0:
			result = coreExecute.convertToShadowrocket();
			break;
		case 1:
			if (coreExecute.preferredDNS == "") { printf("Failed.\n"); util::reportError("Need to specify preferred DNS."); return 1; }
			else { result = coreExecute.convertToBind(); }
			break;
		case 2:
			result = coreExecute.convertToShadowsocksWindows();
			break;
		case 3:
			result = coreExecute.convertToSwitchyOmega();
			break;
		default:
			util::reportError("Need to specify output mode (Shadowrocket? Bind 9? Shadowsocks-windows? )"); 
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