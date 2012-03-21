#include "TwitterCommands.h"

TwitterCommands::TwitterCommands()
	:commands_filepath("")
	,colours_filepath("")
	,verbose(false)
{
	scenes.insert("cell");
	scenes.insert("drawn");
	scenes.insert("texture");
	scenes.insert("spots");
}

TwitterCommands::~TwitterCommands() {
}

void TwitterCommands::setup(const string& commandsFilePath, const string& coloursFilePath) {
	commands_filepath = commandsFilePath;
	colours_filepath = coloursFilePath;
}

void TwitterCommands::load() {
	if(!commands_filepath.size() || !colours_filepath.size()) {
		printf("Warning: call setup first before loading commands.\n");
		return;
	}
	
	loadCommands(commands_filepath);
	loadColours(colours_filepath);
}

void TwitterCommands::loadCommands(const string& filePath) {
	if(!commands_filepath.size()) {
		printf("Warning: no file path given to load commands.");
		return;
	}
	ifstream ifs(commands_filepath.c_str());
	if(!ifs.is_open()) {
		printf("Warnin: cannot open commands file");
		return;
	}
	commands.clear();
	string word;
	string tok;
	string cmd;
	while(std::getline(ifs, word)) {
	
		// keep track of aliases
		stringstream ss(word);
		if(word.find(",") != std::string::npos) {
			int i = 0;
			while(std::getline(ss, tok, ',')) {
				aliased_words.insert(tok);
				if(i == 0) {
					word = tok;
					++i;
					commands.insert(tok);
				}
				else {
					aliases[tok] = word;
				}
			}
		}
		else {
			commands.insert(word);
		}
	}
	
	ifs.close();
}

void TwitterCommands::loadColours(const string& filePath) {
	ofxXmlSettings xml;
	if(!xml.loadFile(filePath) ){
		printf("Warning: unable to load: '%s'\n", filePath.c_str());
		return;
	}
	
	int num = xml.getNumTags("colourlist");
	if(num <= 0) {
		printf("Warning: no colour list found.\n");
		return;
	}

	xml.pushTag("colourlist", num-1);
	int num_tags = xml.getNumTags("colour");
	if(num_tags <= 0) {
		printf("Warning: No colours found.\n");
		return;
	}
	
	int num_to_read = num_tags;
	for(int i = 0; i < num_to_read; i++){
		string name = xml.getValue("colour:name", "transparent",i);
		int r = xml.getValue("colour:red", 0, i);
		int g = xml.getValue("colour:green", 0, i);
		int b = xml.getValue("colour:blue", 0, i);
		ofColor col(r,g,b);
		colours.insert(std::pair<string, ofColor>(name, col));
		colour_names.insert(name);
	}
	xml.popTag();
}

void TwitterCommands::testCommandString(string msg) {
	std::stringstream ss(msg);
	string word;
	set<string> unfiltered_tokens;
	while(std::getline(ss, word, ' ')) {
		printf("-- '%s'\n", word.c_str());
		unfiltered_tokens.insert(word);
	}
	
	set<string> command_result;
	map<string, ofColor> color_result;
	set<string> scene_result;
	
	// filter, and print result.
	filterCommands(unfiltered_tokens, command_result, color_result, scene_result);
	set<string>::iterator cit = command_result.begin();
	while(cit != command_result.end()) {
		printf("Found command: '%s'\n", (*cit).c_str());
		++cit;
	}
}


// returns true when we found something we need handle
bool TwitterCommands::filterCommands(
			 set<string>& unfiltered
			,set<string>& commandsResult
			,map<string, ofColor>& coloursResult
			,set<string>& scenesResult
) 
{
	
	// first, replace aliases with the aliased word.
	set<string> aliased;
	std::set_intersection(
		unfiltered.begin(), unfiltered.end()
		,aliased_words.begin(), aliased_words.end()
		,std::inserter(aliased, aliased.end())
	);
	set<string>::iterator sit = aliased.begin();
	while(sit != aliased.end()) {
		commandsResult.insert(aliases[(*sit)]);
		++sit;
	}

	// now find which commands are valid.	
	std::set_intersection(
		 unfiltered.begin(), unfiltered.end()
		,commands.begin(), commands.end()
		,std::inserter(commandsResult, commandsResult.end())
	);

	// and find the colours.
	set<string> found_colours;
	std::set_intersection(
		unfiltered.begin(), unfiltered.end()
		,colour_names.begin(), colour_names.end()
		,std::inserter(found_colours, found_colours.end())
	);
	
	set<string>::iterator it = found_colours.begin();
	while(it != found_colours.end()) {
		string name = (*it);
		coloursResult.insert(std::pair<string,ofColor>(name, colours[name]));
		++it;
	}
	
	// find scene
	std::set_intersection(
		commandsResult.begin(), commandsResult.end()
		,scenes.begin(), scenes.end()
		,std::inserter(scenesResult, scenesResult.end())
	);
	
	return commandsResult.size() || coloursResult.size() || scenesResult.size();
}

void TwitterCommands::print() {
	set<string>::iterator cit = commands.begin();
	while(cit != commands.end()) {
		printf("Command: %s\n", (*cit).c_str());
		++cit;
	}
	map<string, string>::iterator it = aliases.begin();
	while(it != aliases.end()) {
		printf("Alias: %s = %s\n", (it->first.c_str()), (it->second.c_str()));
		++it;
	}
	
	set<string>::iterator aw = aliased_words.begin();
	while(aw != aliased_words.begin()) {
		printf("Aliased word: %s\n", (*aw).c_str());
		++aw;
	}
}

