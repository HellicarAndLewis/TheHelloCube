//
//  BaseScenes.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "AppAssets.h"
#include "ofxGui.h"
#include "../../../addons/Tools/Utils/Utils.h"
#include "TwitterTypes.h"
#include "Effects.h"
#include "AudioManager.h"

class BaseScene {
    
private:
protected:
public:
    
    BaseScene();
    ~BaseScene();
    
    enum {
        TOP    = 0,
        BOTTOM = 1,
        LEFT   = 2,
        RIGHT  = 3  
    };
    
    // ----------------------------------------------------
    string   name;
    ofxPanel gui;
    
    ofColor         bgColor;
    ofColor         bgColorTarget;
	static ofColor	twitterColour;
    AudioManager *  audioPtr;
    vector<ofColor> complimentaryColours;
	bool 			mustDrawGui;
    
    // ----------------------------------------------------
    virtual void exitScene() {
		bgColor = BaseScene::twitterColour;
        ofUnregisterMouseEvents(&gui);
    }
    
    virtual void enterScene() {
        generateComplimentaryColours();
        ofRegisterMouseEvents(&gui);
    }

    // ----------------------------------------------------
	virtual void handleCommands(TwitterCommand& cmd, Effects& fx) { }
	
    // ----------------------------------------------------
    virtual void drawBackground();
	virtual void drawGui();
    
    // ----------------------------------------------------
    virtual void setup() = 0;
    virtual void update() {};
    virtual void draw();

    // ----------------------------------------------------
    virtual void keyPressed(int key){};
    virtual void keyReleased(int key){}
    virtual void mouseMoved(int x, int y){};
    virtual void mouseDragged(int x, int y, int button){};
    virtual void mousePressed(int x, int y, int button){};
    virtual void mouseReleased(int x, int y, int button){};
    
    virtual void generateComplimentaryColours(); //generate complimentaryColours vector of colours that are close to twitterColour
    
    virtual ofVec2f getCentreCubeScreen(){ return ofVec2f(CUBE_SCREEN_WIDTH /2.f, CUBE_SCREEN_HEIGHT/2.f);};
};