#include "util.h"
#include "DnsmasqToBind.h"

extern string failureReason;

int main() {
	string DNS;
	string fileName;
	cout << "Please Enter file name:";
	cin >> fileName;
	cout << "Please Enter Your preferred DNS:";
	cin >> DNS;
	cout << "------------------\n";
	cout << "Processing...";
	bool result = util::convertToBind(fileName, DNS);
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