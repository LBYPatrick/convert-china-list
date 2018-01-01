#include <string>
#include <fstream>

using std::string;
using std::ifstream;
using std::ofstream;

class ContentProcessor final {
public:
	enum InputType {
		DNSMASQ,
		GFWLIST
	};
	enum OutputType {
		SHADOWROCKET,
		BIND,
		SSWINDOWS,
		SWITCHYOMEGA
	};
	string		rawFileContent = "";
	string		outputContent = "";
	string		customDNS = "";
	string		customProxy = "";

	InputType sourceType = InputType(0);
	OutputType targetType = OutputType(0);

	ContentProcessor() = default;
	void init(string & rawContent, InputType inputType, OutputType outputType);
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