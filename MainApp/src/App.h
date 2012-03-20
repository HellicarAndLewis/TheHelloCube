#pragma once


#include "ofMain.h"
#include "AppAssets.h"
#include "CellScene.h"
#include "DrawnScene.h"
#include "TextureScene.h"
#include "SpotsScene.h"
#include "TwitterManager.h"
#include "Effects.h"
#include "ofxQTKitVideoGrabber.h"
#include "AudioManager.h"

#include "ofxGui.h"
#include "physics/rxSpring.h"
#include "physics/rxParticles.h"
#include "physics/rxParticle.h"

class App : public ofBaseApp{
    
public:
        
    enum {
        SCENE_CELL    = 0,
        SCENE_DRAWN   = 1,
        SCENE_TEXTURE = 2,
        SCENE_SPOTS   = 3,
    };
    
    void setup();
    void update();
    void draw();
    void changeScene(int scene);
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(float * input, int bufferSize, int nChannels); 
    
    
    int                 sceneIndex;
    
					
    // scenes
    vector <BaseScene*>  scenes;
    BaseScene *          currentScene;
    CellScene            cellSC;
    DrawnScene           drawnSC;
    TextureScene         textureSC;
    SpotsScene            spotsSC;
	
    // twitter & FX
	float				 command_timeout;	
	TwitterCommand		 command;
    TwitterManager		 twitter;
    Effects				 fx;
	int					 take_screenshot_on;
	bool				draw_gui;
	ofxPanel 			gui;
	ofxIntSlider		fx_duration;
	ofxIntSlider		delay_between_fx_and_screenshot;
	ofxFloatSlider		fx_pixelate_x;
	ofxFloatSlider		fx_pixelate_y;
	ofxFloatSlider		fx_wave_displace;
	ofxFloatSlider		fx_wave_speed;
	ofxFloatSlider		fx_wave_num;
	ofxFloatSlider		fx_shake_number;
	ofxFloatSlider		fx_shake_amplitude;
	ofxFloatSlider		fx_shake_duration;
	ofxFloatSlider		fx_swirl_radius;
	ofxFloatSlider		fx_swirl_angle;
	ofxButton			fx_test_ripple;
	ofxButton			fx_test_shake;
	ofxToggle			fx_toggle_pixelate;
	ofxToggle			fx_toggle_swirl;
	ofxToggle			fx_toggle_wave;
	ofxToggle			fx_toggle_mirror;
	ofxToggle			fx_toggle_reflect;
	ofxToggle			fx_toggle_invert;
	ofxToggle			fx_toggle_posterize;
	ofxToggle			fx_toggle_flip;
	ofxToggle			fx_reset_automatically;
	
	void setupEffectsGui();
	void onGuiUpdateSettings(bool& on);
	void onGuiTogglePixelate(bool& on);
	void onGuiTestShake(bool& on);
	void onGuiTestRipple(bool& on);
	void onGuiPixelateX(float& v);
	void onGuiPixelateY(float& v);	
	void onGuiToggleWave(bool& on);
	void onGuiWaveSpeed(float& v);
	void onGuiWaveNum(float& v);
	void onGuiWaveDisplace(float& v);
	void onGuiToggleSwirl(bool& on);
	void onGuiSwirlRadius(float& v);
	void onGuiSwirlAngle(float& v);
	void onGuiInvert(bool& on);
	void onGuiMirror(bool& on);
	void onGuiReflect(bool& on);
	void onGuiPosterize(bool& on);
	void onGuiFlip(bool& on);
	
    // Audio
    AudioManager         audioManager;
    bool                 drawAudio;
    
    // Video
	ofxQTKitVideoGrabber vidGrabber;	
	int 				 camWidth;
	int 				 camHeight;  
	
	// Particles test
#ifdef USE_SWIRPS
	rxParticles particles;
	float repel_effect;
	float follow_effect;
#endif
    
    // Video FX
    //LUT bits from openFrameworks/examples/graphics/InstagramLikeImageFilters example 
    void                 loadLUT(string path);
	void                 applyLUT(ofPixelsRef pix);
	bool                 doLUT;
	int                  dirLoadIndex;
	ofDirectory          dir;
	bool                 LUTloaded;
	ofVec3f              lut[32][32][32];
	ofImage              lutImg;
};
