#include "TwitterSimulator.h"
#include "TWitterManager.h"
#include "ofMain.h"

TwitterSimulator::TwitterSimulator(TwitterManager& manager) 
	:manager(manager)
	,gui(4000)
	,take_screenshot(false)
{
	gui.addString("fake_tweet", fake_tweet);
	gui.addCallback("simulate_tweet", 1, this);
	gui.addCallback("reload_commands", 2, this);
	gui.addCallback("take_screenshot", 3, this);
	gui.addCallback("reload_badwords", 4, this);
	gui.addColor("color", col);
//	gui.addColor("martin", col);
}

TwitterSimulator::~TwitterSimulator() {
}

void TwitterSimulator::guiCallback(int v) {
	string settings_file = ofToDataPath("twitter/simulator.ini",true);
	//gui.save(settings_file);
	gui.load(settings_file);
	return;	
	if(v == 1 && fake_tweet.length())  {
		printf("> fake tweet\n");
		rtt::Tweet tweet;
		tweet.setText(fake_tweet);
		manager.parseTweet(tweet);		
	}
	else if(v == 2) {
		printf("> reload commands\n");
		manager.getCommands().load();
	}
	else if(v == 3) {
		take_screenshot = true;
	}
	else if(v == 4) {
		printf("> reload bad words\n");
		manager.reloadBadWords();
	}
}

void TwitterSimulator::update() {
	gui.update();
	//printf("r:%f, g:%f\n", col[0], col[1]);
}