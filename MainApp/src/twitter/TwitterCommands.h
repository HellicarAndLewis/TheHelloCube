#ifndef TWITTER_COMMANDSH
#define TWITTER_COMMANDSH

#include <fstream>
#include <vector>
#include <string>
#include <set>

using std::set;
using std::string;
using std::ifstream;
using std::vector;


class TwitterCommands {
public:
	TwitterCommands();
	~TwitterCommands();
	void setup(const string& filePath);
	void load();
	void filterCommands(set<string>& unfiltered, set<string>& result);

private:
	string filepath;
	set<string> commands;
};
#endif