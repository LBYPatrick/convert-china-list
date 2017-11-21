#include <string>
#include <fstream>

using std::string;
using std::ifstream;
using std::ofstream;

class ContentProcessor final {
public:
	string		rawFileContent = "";
	string		outputContent = "";
	string		customDNS = "";
	string		customProxy = "";

	int sourceType = 0;
	//sourceType 0 : dnsmasq
	//sourceType 1 : GFWList
	int targetType = 0;

	//0 : convertToShadowrocket();
	//1 : convertToBind();
	//2 : convertToShadowsocksWindows();
	//3 : convertToSwitchyOmega();
	ContentProcessor() = default;
	void init(string & rawContent, int inputType, int outputType);
	void getRawList();
	void convert();

private:
	struct dnsmasqProcessor final{
		string getRawDomain(string originLine);
		string getRawDNS(string originLine);
	};

	struct gfwlistProcessor final{
		string getRawDomain(string originLine);
	};

	const string				defaultProxy		= "SOCKS5 127.0.0.1:1080; SOCKS 127.0.0.1:1080";
	const string				defaultDNS			= "8.8.8.8";
		  string				domainListCache		= "";
		  string				dnsListCache		= "";
		  string				readBuffer			= "";
		  string				content				= "";
	      ifstream				reader;
	      ofstream				writer;
		  dnsmasqProcessor		dnsmasq;
		  gfwlistProcessor		gfwlist;
};