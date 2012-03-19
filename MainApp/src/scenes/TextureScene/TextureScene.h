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
class TextureScene : public BaseScene {
    
private:
protected:
public:
    
    void setup();
    void update();
    void draw();
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
   
    bool     circleFrcFlip;
    ofVec2f circleFrc;
    ofxFloatSlider releaseRate;
	ofxFloatSlider repulsionForce;
	ofxFloatSlider attractionForce;
	ofxFloatSlider shapeBounceForce;
	ofxFloatSlider radialForce;
    ofxIntSlider   maxShapesOnScreen;
    
};