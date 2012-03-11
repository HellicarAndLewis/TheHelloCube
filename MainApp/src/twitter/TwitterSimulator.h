#ifndef TWITTER_SIMULATORH
#define TWITTER_SIMULATORH

#include "Twitter.h"
#include "OSCGui.h"
#include "Effects.h"
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
	void setEffects(Effects& f);
	
	string fake_tweet;	
	bool take_screenshot;
	float col[4];
	bool fx_mirror;
	bool fx_flip;
	bool fx_pixelate;
	bool fx_shake;
	bool fx_invert;
	float fx_pixelate_x;
	float fx_pixelate_y;

	float fx_shake_displace;
	float fx_shake_speed;
	float fx_shake_waves;

private:
	string settings_file;
	TwitterManager& manager;
	OSCGui gui;
	Effects* fx;
	
};
#endif