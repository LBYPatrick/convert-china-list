#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

#include "util.h"
#include "ContentProcessor.h"

using std::string;
using std::cin;
using std::cout;
using std::ofstream;
using std::ifstream;
using std::endl;


int checkAccess(string i, string o) {
	
	ifstream tempReader;
	ofstream tempWriter;
	bool	 readAccess = false;
	bool	 writeAccess = false;


	tempReader.open(i.c_str());
	if (tempReader.is_open()) readAccess = true;
	tempReader.close();

	tempWriter.open(o.c_str());
	if (tempWriter.is_open()) writeAccess = true;
	tempWriter.close();
	
	if (!readAccess) return 1;
	else if (!writeAccess) return 2;
	else return 0;

}

void readFileContent(string i, string & o, int inputMode) {
	
	ifstream reader;
	string readBuffer;

	reader.open(i.c_str());

	if(inputMode == 1) while (getline(reader, readBuffer)) { o += readBuffer; }
	else while (getline(reader, readBuffer)) { o += readBuffer; o += "\n";} 
	reader.close();
}


int main(int argc, char*const argv[]) {

	ContentProcessor	executeObj;
	string				inputFileName = "";
	string				outputFileName = "";
	string				preferredDNS = "";
	string				preferredProxy = "";
	string				buf4Parameters = "";
	string				rawFileContent = "";
	int					inputMode = -1;
	int					outputMode = -1;
	int					fileAccessStat = 0;
	bool				result = false;
	ofstream			writer;
	bool				isCommandLineMode = false;

	//Console "Interface" mode START
	if (argc == 1) {
		isCommandLineMode = false;
		system("cls");
		printf("\n	1. dnsmasq \n	2. GFWList\n");
		printf("\nPlease enter type of the source file:");
		cin >> buf4Parameters;

		if (util::containIgnoreCase(buf4Parameters, "dnsmasq") || buf4Parameters == "1") inputMode = 0;
		else if (util::containIgnoreCase(buf4Parameters, "gfwlist") || buf4Parameters == "2") inputMode = 1;
		else {
			util::reportError("Unknown Mode.\n\nFailed to execute.");
			system("pause");
			return 1;
		}

		//====================================================
		//Collect Information
		//====================================================

		system("cls");
		printf("\n	1. Shadowrocket \n	2. Bind\n	3. Shadowsocks-windows PAC\n	4. SwitchyOmega PAC\n");
		printf("\nPlease enter running mode(function number):");
		cin >> buf4Parameters;

		if (util::containIgnoreCase(buf4Parameters, "shadowrocket") || buf4Parameters == "1") { outputMode = 0; }
		else if (util::containIgnoreCase(buf4Parameters, "bind") || buf4Parameters == "2") { outputMode = 1; }
		else if (util::containIgnoreCase(buf4Parameters, "shadowsocks-windows") || buf4Parameters == "3") { outputMode = 2; }
		else if (util::containIgnoreCase(buf4Parameters, "switchyomega") || buf4Parameters == "4") { outputMode = 3; }
		else {
			util::reportError("Unknown Mode.\n\nFailed to execute");
			system("pause");
			return 1;
		}

		//====================================================
		//Specify Input/Output file
		//====================================================
		system("cls");
		printf("Please Enter origin file name:");
		cin >> inputFileName;
		printf("\nPlease Enter output file name:");
		cin >> outputFileName;
	}
	//Console "Interface" END



	//Show help or show error message
	else if (argc == 2) {
		isCommandLineMode = true;
		buf4Parameters = argv[1];
		if (buf4Parameters == "--help" || buf4Parameters == "/?") {
			printf("convert-china-list by LBYPatrick\n");
			printf("\nHere are parameters available:\n");
			printf("    -s  or --source-type         : specify type of source file (dnsmasq (0)? GFWList (1)?). \n");
			printf("    -c  or --convert-to          : specify type of output file (shadowrocket (0)? bind (1)? shadowsocks-windows (2)? SwitchyOmega (3)?). \n");
			printf("    -i  or --input-file          : specify input file.\n");
			printf("    -i  or --input-file          : specify input file.\n");
			printf("    -o  or --output-file         : specify output file. \n");
			printf("    -d  or --dns                 : specify preferred DNS (only works in bind mode).\n");
			printf("    -p  or --proxy               : specify preferred proxy (only works in SwitchyOmega PAC Mode).\n");
			return 0;
		}
		else { printf("[ERROR]Unknown parameter. Please check your spell or read the manual by using --help.\n"); return 1; }
	}

	//====================================================
	//Collect Information
	//====================================================
	else if (argc > 2) {
		isCommandLineMode = true;
		for (int i = 1; i < argc; ++i) {
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

				else if (buf4Parameters == "-i" || buf4Parameters == "--input-file") { inputFileName = argv[i + 1]; }
				else if (buf4Parameters == "-o" || buf4Parameters == "--output-file") { outputFileName = argv[i + 1]; }
				else if (buf4Parameters == "-d" || buf4Parameters == "--dns") { preferredDNS = argv[i + 1]; }
				else if (buf4Parameters == "-p" || buf4Parameters == "--proxy") { preferredProxy = argv[i + 1]; }
			}
		}
	}
	
	//Check parameters
	if (inputMode == -1) {util::reportError("Need to specify input file type.");return 1;}
	if (outputMode == -1) outputMode = 2; //Convert to Shadowsocks-windows by default
	if (inputFileName == "") {util::reportError("Need to specify output file type.");return 1;}
	if (outputFileName == "") outputFileName = inputFileName + ".converted"; // Write to [FILENAME].converted by default

	//Check file access
	fileAccessStat = checkAccess(inputFileName, outputFileName);

	if (fileAccessStat != 0) {
		util::reportError((fileAccessStat == 1) ? "No Access to the input file." : "No Access to the output file");
		util::reportError("Failed to execute.");
		return 1;
	}

	//Read file to memory
	readFileContent(inputFileName, rawFileContent,inputMode);
	
	//Initialize
	executeObj.init(rawFileContent, ContentProcessor::InputType(inputMode), ContentProcessor::OutputType(outputMode));
	executeObj.customDNS = preferredDNS;
	executeObj.customProxy = preferredProxy;

	//Convert
	executeObj.getRawList();
	executeObj.convert();

    //Write to the target file
	writer.open(outputFileName.c_str());
	writer << executeObj.outputContent;
	writer.close();

	printf("Success.\n");

	if(!isCommandLineMode) system("pause");
	return 0;

}