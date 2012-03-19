#pragma once

//#define USE_SWIRPS

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
    
    
    int                  sceneIndex;
    
    // scenes
    vector <BaseScene*>  scenes;
    BaseScene *          currentScene;
    CellScene            cellSC;
    DrawnScene           drawnSC;
    TextureScene         textureSC;
    SpotsScene            spotsSC;
	
    // twitter
	float				 command_timeout;	
	TwitterCommand		 command;
    TwitterManager		 twitter;
    Effects				 fx;
	float				 fx_duration;
	
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
