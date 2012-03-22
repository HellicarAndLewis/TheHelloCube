#pragma once
#include "ofMain.h"

#define CUBE_SCREEN_WIDTH 1024
#define CUBE_SCREEN_HEIGHT 768
#define CAMERA_PROJECTION_SCREEN_WIDTH 1280
#define CAMERA_PROJECTION_SCREEN_HEIGHT 1024

#define CAM_WIDTH 960
#define CAM_HEIGHT 720
#define USE_FX

#define NUM_AUDIO_CHANNELS 6
//#define USE_SWIRPS
//#define USE_VEINS

// Use this to switch code between prouction and development
// production running at the Turbine Hall
#define RUN_MODE_DEVELOPMENT 	1
#define RUN_MODE_PRODUCTION		2
//#define RUN_MODE				RUN_MODE_PRODUCTION
#define RUN_MODE				RUN_MODE_DEVELOPMENT

#if RUN_MODE == RUN_MODE_DEVELOPMENT
	#define USE_SMALL_APP
#else

#endif
#define USE_MOUSE_HACK      // use the mouse to damp the mic....

class AppAssets {
    
private:
    static AppAssets * instance;
public:
    ~AppAssets();
    static AppAssets * inst();
    ofTrueTypeFont     appFont;
};
