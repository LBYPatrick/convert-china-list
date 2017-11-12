#include "util.h"

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
