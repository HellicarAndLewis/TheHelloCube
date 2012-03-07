#include "TwitterCommands.h"

TwitterCommands::TwitterCommands()
	:filepath("")
{
}

TwitterCommands::~TwitterCommands() {
}

void TwitterCommands::setup(const string& filePath) {
	filepath = filePath;
}

void TwitterCommands::load() {
	if(!filepath.size()) {
		printf("No file path given to load commands.");
		return;
	}
	ifstream ifs(filepath.c_str());
	if(!ifs.is_open()) {
		printf("Cannot open commands file");
		return;
	}
	commands.clear();
	string word;
	while(std::getline(ifs, word)) {
		commands.insert(word);
		//printf("Command: %s\n", word.c_str());
	}
	
	ifs.close();
}



void TwitterCommands::filterCommands(set<string>& unfiltered, set<string>& result) {
	std::set_intersection(
		 unfiltered.begin(), unfiltered.end()
		,commands.begin(), commands.end()
		,std::inserter(result, result.end())
	);
}


