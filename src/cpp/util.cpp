#include "util.h"
#include "DnsmasqToBind.h"

string failureReason;

bool util::convertToBind(string fileName, string preferredDNS) {
	ifstream Reader;
	ofstream Writer;
	char sprintfBuffer[128];
	string InputFile;
	string OutputFile;
	
	sprintf(sprintfBuffer, "%s.conf", fileName.c_str());
	InputFile = sprintfBuffer;
	sprintf(sprintfBuffer, "named.conf.%s", fileName.c_str());
	OutputFile = sprintfBuffer;
	
	Reader.open(InputFile.c_str());
	
	string buffer;
	string domain;

	if (Reader.is_open()) {
		Writer.open(OutputFile.c_str());
		if (!Writer.is_open()) {
			failureReason += "\nProgram cannot output to the target file.";
			return 1;
		}

		while (getline(Reader, buffer)) {
			for (int counter = 8; counter < (buffer.length() - 16); counter++) {
				domain += buffer[counter];
				}
			Writer << "zone " << "\"" << domain << "\"" << " {" << endl;
			Writer << "	type forward;" << endl;
			Writer << "	forwarders {" << preferredDNS << ";};" << endl;
			Writer << "};\n\n";
			domain = "";
			buffer = "";

			}
		Writer.close();
		Reader.close();
	}
	else {
		failureReason +=  "\nProgram cannot find the origin file.";
		return 1;
		}
	return 0;
}

void util::visualProgress(string message, double currentProgress, double progressGoal) {
	system("cls");
	cout << message << "\n[";
	currentProgress++;
	for (int i = 0; i <= (currentProgress); i++) {
		cout << ">";
	} for (int i = 0; i < (progressGoal - currentProgress); i++) {
		cout << " ";
	}
	if (currentProgress + (progressGoal - currentProgress) < progressGoal) {
		cout << " ";
	}
	cout << "] " << currentProgress << "/" << progressGoal;
	if (currentProgress == progressGoal) {
		cout << "\n\n\n";
	}
};