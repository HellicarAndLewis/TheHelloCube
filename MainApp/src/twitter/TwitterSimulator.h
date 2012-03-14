/**
 * Simulator, used to debug and test effects, tweet parsing etc..
 * 
 * 
 * 
 * @date	2012.03.xx
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 */
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
	void setVerbose(bool v);
	
	string fake_tweet;	
	bool take_screenshot;
	float col[4];
	bool fx_mirror;
	bool fx_flip;
	bool fx_pixelate;
	bool fx_wave;
	bool fx_invert;
	bool fx_swirl;
	bool fx_posterize;
	bool fx_reflect;
	
	float fx_pixelate_x;
	float fx_pixelate_y;

	float fx_wave_displace;
	float fx_wave_speed;
	float fx_wave_num;
	float fx_shake_number;
	float fx_shake_amplitude;
	float fx_shake_duration;
	float fx_swirl_radius;
	float fx_swirl_angle;


private:
	bool verbose;
	string settings_file;
	TwitterManager& manager;
	OSCGui gui;
	Effects* fx;
	
};

inline void TwitterSimulator::setVerbose(bool v) {
	verbose = v;	
}


#endif