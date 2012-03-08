#ifndef TWITTER_SIMULATORH
#define TWITTER_SIMULATORH

#include "Twitter.h"
#include "OSCGui.h"

class TwitterManager;

class TwitterSimulator : public roxlu::GuiCallback{
public:
	TwitterSimulator(TwitterManager& manager);
	~TwitterSimulator();
	void loadSettings();
	void saveSettings();
	void setup(const string& settingsFile);
	void update();
	void guiCallback(int t);
	string fake_tweet;	
	bool take_screenshot;
	float col[4];
private:
	string settings_file;
	TwitterManager& manager;
	OSCGui gui;
	
};
#endif