#ifndef TWITTER_SIMULATORH
#define TWITTER_SIMULATORH

#include "Twitter.h"
#include "OSCGui.h"

class TwitterManager;

class TwitterSimulator : public roxlu::GuiCallback{
public:
	TwitterSimulator(TwitterManager& manager);
	~TwitterSimulator();
	void update();
	void guiCallback(int t);
	string fake_tweet;	
	bool take_screenshot;
private:

	TwitterManager& manager;
	OSCGui gui;
};
#endif