//
//  TextureScene.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
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
    
    vector <ofTexture> textures;
    
    ofxBox2d box2d;
    vector <TexturedShape> shapes;
    vector <ofVec2f>       pts;
    vector <TriangleShape> tris;
    int index;
};