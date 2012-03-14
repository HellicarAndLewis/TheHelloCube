/**
 * Parsing commands from twitter
 * 
 * 
 * 
 * @date	2012.03.xx
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 */
#ifndef TWITTER_COMMANDSH
#define TWITTER_COMMANDSH

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include <fstream>
#include <vector>
#include <string>
#include <set>

using std::set;
using std::string;
using std::ifstream;
using std::vector;

struct ColourNameAndValue{
	string name;
	ofColor value;
};


class TwitterCommands {
public:
	TwitterCommands();
	~TwitterCommands();
	void setup(const string& commandsFilePath, const string& coloursFilePath);
	void load();
	bool filterCommands(
		 set<string>& unfiltered
		,set<string>& commandsResult
		,map<string, ofColor>& coloursResult
		,set<string>& scenesResult
	);
	void setVerbose(bool v);	
private:
	void loadCommands(const string& filePath);
	void loadColours(const string& filePath);
	bool verbose;
	string commands_filepath;
	string colours_filepath;
	set<string> commands;
	set<string> colour_names;
	map<string, ofColor> colours;
	map<string, string> aliases;
	set<string> aliased_words;
	set<string> scenes;
};

inline void TwitterCommands::setVerbose(bool v) {
	verbose = v;
}
#endif