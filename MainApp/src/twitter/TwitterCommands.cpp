#include "TwitterCommands.h"

TwitterCommands::TwitterCommands()
	:commands_filepath("")
	,colours_filepath("")
{
}

TwitterCommands::~TwitterCommands() {
}

void TwitterCommands::setup(const string& commandsFilePath, const string& coloursFilePath) {
	commands_filepath = commandsFilePath;
	colours_filepath = coloursFilePath;
}

void TwitterCommands::load() {
	if(!commands_filepath.size() || !colours_filepath.size()) {
		printf("Call setup first before loading commands.\n");
		return;
	}
	
	loadCommands(commands_filepath);
	loadColours(colours_filepath);
}

void TwitterCommands::loadCommands(const string& filePath) {
	if(!commands_filepath.size()) {
		printf("No file path given to load commands.");
		return;
	}
	ifstream ifs(commands_filepath.c_str());
	if(!ifs.is_open()) {
		printf("Cannot open commands file");
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
		printf("Unable to load: '%s'\n", filePath.c_str());
		return;
	}
	
	int num = xml.getNumTags("colourlist");
	if(num <= 0) {
		printf("No colour list found.\n");
		return;
	}

	xml.pushTag("colourlist", num-1);
	int num_tags = xml.getNumTags("colour");
	if(num_tags <= 0) {
		printf("No colours found.\n");
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



void TwitterCommands::filterCommands(
			 set<string>& unfiltered
			,set<string>& commandsResult
			,map<string, ofColor>& coloursResult
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
}


