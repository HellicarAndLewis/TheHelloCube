#include "TwitterSimulator.h"
#include "TWitterManager.h"
#include "ofMain.h"

TwitterSimulator::TwitterSimulator(TwitterManager& manager) 
	:manager(manager)
	,gui()
	,verbose(false)
	,take_screenshot(false)
	,fx_mirror(false)
	,fx_flip(false)
	,fx_pixelate(false)
	,fx_wave(false)
	,fx_invert(false)
	,fx_swirl(false)
	,fx_posterize(false)
	,fx_reflect(false)
	,fx_crack(false)
    ,fx_pixelate_x(5.0f)
    ,fx_pixelate_y(5.0f)
	,fx_wave_displace(0.01f)
	,fx_wave_speed(1.001f)
	,fx_wave_num(5.0f)
	,fx_shake_number(6.0f)
	,fx_shake_amplitude(0.4f)
	,fx_shake_duration(3.5f)
{
	
	gui.addString("fake_tweet", fake_tweet);
	gui.addCallback("simulate_tweet", 1, this);
	gui.addCallback("reload_commands", 2, this);
	gui.addCallback("take_screenshot", 3, this);
	gui.addCallback("reload_badwords", 4, this);
	gui.addCallback("save_settings", 5, this);
	gui.addCallback("shake_fx", 6, this); 
	gui.addCallback("ripple_fx", 7, this);
}

TwitterSimulator::~TwitterSimulator() {
}

void TwitterSimulator::saveSettings() {
	gui.save(settings_file);
}

void TwitterSimulator::loadSettings() {
	gui.load(settings_file);
}

void TwitterSimulator::setup(const string& settingsFile) {
	settings_file = settingsFile;
}

void TwitterSimulator::guiCallback(int v) {
	if(v == 1 && fake_tweet.length())  {
		if(verbose) {
			printf("> fake tweet\n");
		}
		rtt::Tweet tweet;
		tweet.setText(fake_tweet);
		manager.parseTweet(tweet, true);		
	}
	else if(v == 2) {
		if(verbose) {
			printf("> reload commands\n");
		}
		manager.getCommands().load();
	}
	else if(v == 3) {
		take_screenshot = true;
	}
	else if(v == 4) {
		if(verbose) {
			printf("> reload bad words\n");
		}
		manager.reloadBadWords();
	}
	else if(v == 5) {
		if(verbose) {
			printf("> save settings.\n");
		}
		saveSettings();
	}
	else if(v == 6) {	
		if(verbose) {
			printf("> bounce: duration: %f seconds, %f bounces, %f amplitude.\n", fx_shake_duration, fx_shake_number, fx_shake_amplitude);
		}
		fx->shake(true, fx_shake_duration, fx_shake_number, fx_shake_amplitude);
	}
	else if(v == 7) {
		if(verbose) {
			printf("> ripple!\n");
		}
		fx->ripple(true, 3.5);
	}
}

void TwitterSimulator::update() {
	gui.update();
	
	if(gui.didValueChange()) {
		fx->mirror(fx_mirror);
		fx->flip(fx_flip);
		fx->pixelate(fx_pixelate, fx_pixelate_x, fx_pixelate_y);
		fx->wave(fx_wave, fx_wave_speed, fx_wave_displace, fx_wave_num);
		fx->invert(fx_invert);
		fx->swirl(fx_swirl, fx_swirl_radius, fx_swirl_angle);
		fx->posterize(fx_posterize);
		fx->reflect(fx_reflect);
		fx->crack(fx_crack);
	}
	
}

void TwitterSimulator::setEffects(Effects& f) {
	fx = &f;
	gui.addBool("fx_mirror", fx_mirror);
	gui.addBool("fx_flip", fx_flip);
	gui.addBool("fx_wave", fx_wave);
	gui.addBool("fx_pixelate", fx_pixelate);
	gui.addBool("fx_invert", fx_invert);
	gui.addBool("fx_swirl", fx_swirl);
	gui.addBool("fx_reflect", fx_reflect);
	gui.addBool("fx_posterize", fx_posterize);
	gui.addBool("fx_crack", fx_crack);
	gui.addFloat("fx_pixelate_x", fx_pixelate_x);
	gui.addFloat("fx_pixelate_y", fx_pixelate_y);
	gui.addFloat("fx_wave_displace", fx_wave_displace);
	gui.addFloat("fx_wave_speed", fx_wave_speed);
	gui.addFloat("fx_wave_num", fx_wave_num);
	gui.addFloat("fx_shake_duration", fx_shake_duration);
	gui.addFloat("fx_shake_number", fx_shake_number);
	gui.addFloat("fx_shake_amplitude", fx_shake_amplitude);
	gui.addFloat("fx_swirl_angle", fx_swirl_angle);
	gui.addFloat("fx_swirl_radius", fx_swirl_radius);

}