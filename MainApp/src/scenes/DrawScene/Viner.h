//
//  Viner.h
//  MainApp
//
//  Created by Todd Vanderlin on 3/11/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "BaseScene.h"
#include "ofxBox2d.h"

class Viner {
    
public:
    
    // -------------------------------------------------------
    ofxBox2dCircle					anchor;			  
    vector		<ofxBox2dCircle>	circles;		  
    vector		<ofxBox2dJoint>		joints;
    
    // -------------------------------------------------------
    void make(float x, float y, ofxBox2d &box2d) {
        
        anchor.setup(box2d.getWorld(), x, y, 4);
        
        // first we add just a few circles
        for (int i=0; i<3; i++) {
            ofxBox2dCircle circle;
            circle.setPhysics(3.0, 0.53, 0.1);
            circle.setup(box2d.getWorld(), x, y+(i*20), 8);
            circles.push_back(circle);
        }
        
        // now connect each circle with a joint
        for (int i=0; i<circles.size(); i++) {
            
            ofxBox2dJoint joint;
            
            // if this is the first point connect to the top anchor.
            if(i == 0) {
                joint.setup(box2d.getWorld(), anchor.body, circles[i].body);		
            }
            else {
                joint.setup(box2d.getWorld(), circles[i-1].body, circles[i].body);
            }
            
            joint.setLength(25);
            joints.push_back(joint);
        }
    }
    
    
    // -------------------------------------------------------
    void draw() {
        ofSetHexColor(0xf2ab01);
        anchor.draw();
        
        for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
            circles[i].draw();
        }
        
        for(int i=0; i<joints.size(); i++) {
            ofSetHexColor(0x444342);
            joints[i].draw();
        }
    }
};