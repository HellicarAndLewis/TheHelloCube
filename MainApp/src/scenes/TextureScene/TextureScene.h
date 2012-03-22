//
//  TextureScene.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "BaseScene.h"
#include "ofxBox2d.h"
#include "TexturedShape.h"
#include "ofxBox2dPolygonUtils.h"
#include "GeometryUtils.h"

class Attractor {
public:
    Attractor() {
        amp  = 1;
        ampD = 0;
    }
    ofVec2f pos;
    float   amp; // 0 - 1;
    float   ampD;
    int     side;
    
    float getAmp() {
        return ofClamp(amp, 0, 1);
    }
    void setAmp(float v) {
        ampD = v;
    }
    
    void update() {
        amp += (ampD-amp) * 0.7;
    }
    
    void draw() {
        
        ofSetColor(255);
        ofCircle(pos, 25);
        ofSetColor(255, 0, 255);
        ofCircle(pos, 5+(20*getAmp()));
        
        ofSetColor(0, 250, 250);
        ofDrawBitmapString(AudioManager::getSideName(side), pos);
 
    }
};

class TextureScene : public BaseScene {
    
private:
protected:
public:
    
    void setup();
    void update();
    void draw();
	virtual void drawGui();
    void keyPressed(int key);
    void addPoints();
    void makeObstacles();
    void addShape();
    void respondToNewComplimentaryColours();
    void enterScene() {
        generateComplimentaryColours();
        respondToNewComplimentaryColours();
        ofRegisterMouseEvents(&gui);
    }
    void exitScene();
    
    
	void handleCommands(TwitterCommand& cmd, Effects& fx);
	
    vector <ofTexture> textures;
    
    ofxBox2d box2d;
    vector <ofxBox2dCircle> obsticals;
    vector <TexturedShape>   shapes;
    vector <ofVec2f>         pts;
    vector <TriangleShape>   tris;

    vector <Attractor>      attractors;

    
    bool     circleFrcFlip;
    ofVec2f circleFrc;
    ofxFloatSlider releaseRate;
	ofxFloatSlider repulsionForce;
	ofxFloatSlider attractionForce;
	ofxFloatSlider shapeBounceForce;
	ofxFloatSlider radialForce;
	ofxFloatSlider peakValue;
    ofxFloatSlider attractionDamping;

    ofxIntSlider   maxShapesOnScreen;
    
};