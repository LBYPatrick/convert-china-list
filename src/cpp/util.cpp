#include "util.h"
#include "main.h"
#include "windows.h"

string failureReason;

void util::visualProgress(string message, double currentProgress, double progressGoal) {
	system("cls");
	
	printf("%s\n[",message.c_str());
	currentProgress++;
	for (int i = 0; i <= (currentProgress); i++) {printf(">");} 
	
	for (int i = 0; i < (progressGoal - currentProgress); i++) {printf(" ");}

	if (currentProgress + (progressGoal - currentProgress) < progressGoal) {printf(" ");}

	printf("]%f%s%f",currentProgress,R"(\)",progressGoal);

	if (currentProgress == progressGoal) {printf("\n\n\n");}
};

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

void util::sysExecute(string cmd) {

	cmd = R"(cmd /q /c ")" + cmd + R"(")";


	TCHAR *commandInTCHAR = new TCHAR[cmd.size() + 1];
	commandInTCHAR[cmd.size()] = 0;
	std::copy(cmd.begin(), cmd.end(), commandInTCHAR);


	STARTUPINFO si;
	PROCESS_INFORMATION pi;



	si.lpReserved = NULL;
	si.lpDesktop = NULL;
	si.lpTitle = NULL;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.cbReserved2 = NULL;
	si.lpReserved2 = NULL;

	si.cb = sizeof(si);
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));


	bool result = CreateProcess(NULL,   // No module name (use command line)
		commandInTCHAR,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		;

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

}