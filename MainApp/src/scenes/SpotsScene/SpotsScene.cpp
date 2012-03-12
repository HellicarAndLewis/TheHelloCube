#include "SpotsScene.h"


// ----------------------------------------------------
void SpotScene::setup() {
    name = "spots scene";
}

// ----------------------------------------------------
void SpotScene::handleCommands(TwitterCommand& cmd, Effects& fx) {
	// handle commands.
	set<string>::const_iterator it = cmd.tokens.begin();
	while(it != cmd.tokens.end()) {
		const string& c = (*it);
		if(c == "bigger") {
		}
		else if(c == "smaller") {
		}
		fx.applyEffect(c);
		++it;
	}
	
	// handle colour
	map<string, ofColor>::const_iterator cit = cmd.colours.begin();
	while(cit != cmd.colours.end()) {	
		//bgColorTarget = cit->second;
		break;
	}
}