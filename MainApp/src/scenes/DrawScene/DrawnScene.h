//
//  DrawScene.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "BaseScene.h"
#include "ofxBox2d.h"
#include "Viner.h"
#include "Particle.h"
#include "VectorField.h"
#include "Wiggler.h"
#include "TinyVine.h"


#include "rxSpring.h"
#include "rxParticles.h"
#include "rxParticle.h"

// Particles test
#ifdef USE_SWIRPS
rxParticles particles;
float repel_effect;
float follow_effect;
#endif

#define MAX_CHASERS 100
#define MAX_POOP    700
//#define MAKE_LAND

// ----------------------------------------------------
class Chaser : public Particle {
public:
    Chaser() {
        radius       = ofRandom(10, 40);
        nConnections = 0;
        rotation = 0;
    }
    int nConnections;
    
    float rotation;
    
};

// ----------------------------------------------------
// Land mass
// ----------------------------------------------------
class LandMass : public ofPolyline {
  public:
    vector <ofVec2f> perps;
    vector <float>   circleVariance;
    int              texId;
};

// ----------------------------------------------------
class VinePoop : public ofxBox2dCircle {
public:
    VinePoop() {
        dotSize = 1;
        dotSizeD = 1;
        img = NULL;
        birthdate = ofGetElapsedTimef();
        bFade =false;
        bGetBig = false;
        alpha = 255;
        newPopSize = 10;
    }
    
    void updateColor() {
        color.r += (colorD.r - color.r) * 0.2;
        color.g += (colorD.g - color.g) * 0.2;
        color.b += (colorD.b - color.b) * 0.2;
        
        if(bGetBig) {
            float r = getRadius();
            r += (newPopSize-r) * 0.2;
            setRadius(r);
        }
    }
  
    bool bGetBig;
    bool bFade;
    float alpha;
    float birthdate;
    float dotSize, dotSizeD, newPopSize;
    ofImage * img;
    ofColor color, colorD;    
};


// ----------------------------------------------------
class DrawnScene : public BaseScene {
    
private:
protected:
public:
    
    ofxBox2d        box2d;
    
    ofxFloatSlider  damping;
    ofxFloatSlider  frequency;
    
    void setup();
    void update();
    void draw(); 
	virtual void drawGui();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);

	void handleCommands(TwitterCommand& cmd, Effects& fx);   
    void respondToNewComplimentaryColours();
    void enterScene();
    
    VectorField field;
    
    // chasers and the stuff that comes out of them
    void makePoop();
    void addChasers(float x=ofRandom(CUBE_SCREEN_WIDTH), float y = ofRandom(-300, 300) + CUBE_SCREEN_HEIGHT/2);
    void addBush(float startX);
    void makeWiggler();
    void makeTinyVine();
    void makePoopFromTinyVines();
    
    vector <Chaser>     chasers;
    vector <VinePoop>   poop;
    vector <Viner>      vines;
    vector <Viner*>     poopVines;
    
    vector <Wiggler>    wigglers;
    vector <TinyVine>   tinyVines;
    
    // ---------------------
    // land wiggles
    // ---------------------
    vector <Wiggler>    landWiggles;

    // ---------------------
    // land masses
    // ---------------------
    vector <LandMass>   land;
    vector <Hair>       landHair;
    float               landHairSep;
    
    // ---------------------
    // images
    // ---------------------
    vector <ofImage> tris;
    vector <ofImage> dots;
    ofImage          eyeA;
    float            eyeRatio;
    ofImage          dotRepeatImg;
    
    // ---------------------
    // modes
    // ---------------------
    int              bBigMode;
    int              tinyVinesPoopAmt;
    bool             bTinyPoop;
    
    // Particles test
#ifdef USE_SWIRPS
	rxParticles particles;
	float repel_effect;
	float follow_effect;
#endif
  
};