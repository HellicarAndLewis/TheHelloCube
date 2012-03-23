#pragma once
#include "ofMain.h"

#define CUBE_SCREEN_WIDTH 1024
#define CUBE_SCREEN_HEIGHT 768
#define CAMERA_PROJECTION_SCREEN_WIDTH 1280
#define CAMERA_PROJECTION_SCREEN_HEIGHT 1024

#define CAM_WIDTH 960
#define CAM_HEIGHT 	720
#define USE_FX

#define NUM_AUDIO_CHANNELS  6

// Use this to switch code between prouction and development production running at the Turbine Hall
#define RUN_MODE_DEVELOPMENT 1
#define RUN_MODE_PRODUCTION	2
#define RUN_MODE RUN_MODE_DEVELOPMENT

#if RUN_MODE == RUN_MODE_DEVELOPMENT

	#define USE_SMALL_APP
	#define FETCH_MENTION_DELAY_MILLIS 25000
	#define USE_TEST_TWITTERACCOUNT

#else
	#define USE_TEST_TWITTERACCOUNT
	#define FETCH_MENTION_DELAY_MILLIS 11000

#endif

#define USE_MOUSE_HACK      // use the mouse to damp the mic....


//#define USE_SWIRPS
//#define USE_VEINS

class AppAssets {
    
private:
    static AppAssets * instance;
public:
    ~AppAssets();
    static AppAssets * inst();
    ofTrueTypeFont     appFont;
};
