#include "TwitterSimulator.h"
#include "TWitterManager.h"

TwitterSimulator::TwitterSimulator(TwitterManager& manager) 
	:manager(manager)
	,gui(4000)
	,take_screenshot(false)
{
	gui.addString("fake_tweet", fake_tweet);
	gui.addCallback("simulate_tweet", 1, this);
	gui.addCallback("reload_commands", 2, this);
	gui.addCallback("take_screenshot", 3, this);
}

TwitterSimulator::~TwitterSimulator() {
}

void TwitterSimulator::guiCallback(int v) {
	if(v == 1 && fake_tweet.length())  {
		rtt::Tweet tweet;
		tweet.setText(fake_tweet);
		manager.parseTweet(tweet);		
	}
	else if(v == 2) {
		manager.getCommands().load();
	}
	else if(v == 3) {
		take_screenshot = true;
	}

}

void TwitterSimulator::update() {
	gui.update();
}