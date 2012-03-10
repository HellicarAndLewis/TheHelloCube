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
#define CAM_WIDTH 960
#define CAM_HEIGHT 720
//#define USE_FX

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
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    bool                 bExportPDF;
    int                  sceneIndex;
    
    // scenes
    vector <BaseScene*>  scenes;
    BaseScene *          currentScene;
    CellScene            cellSC;
    DrawnScene           drawnSC;
    TextureScene         textureSC;
    SpotScene            spotsSC;
	
    // twitter
    TwitterManager		 twitter;
#ifdef USE_FX
    Effects				 fx;
#endif
    
    // Video
	ofxQTKitVideoGrabber vidGrabber;	
	int 				 camWidth;
	int 				 camHeight;  
    
    // Video FX
    //LUT bits from openFrameworks/examples/graphics/InstagramLikeImageFilters example 
    void                 loadLUT(string path);
	void                 applyLUT(ofPixelsRef pix);
	bool                 doLUT;
	int                  dirLoadIndex;
	ofDirectory          dir;
	ofPoint              lutPos;
	ofPoint              thumbPos;
	bool                 LUTloaded;
	ofVec3f              lut[32][32][32];
	ofImage              lutImg;
};
