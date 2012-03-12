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
        
        int nStart = (int)ofRandom(3, 5);
        
        // first we add just a few circles
        for (int i=0; i<nStart; i++) {
            
            ofxBox2dCircle circle;
            circle.setPhysics(3.0, 0.53, 0.1);
            circle.setup(box2d.getWorld(), x, y+(i*20), 3);
            circles.push_back(circle);
        
        }
        
        // now connect each circle with a joint
        for (int i=0; i<circles.size(); i++) {
            
            ofxBox2dJoint joint;
            
            // if this is the first point connect to the top anchor.
            if(i == 0) {
                joint.setup(box2d.getWorld(), anchor.body, circles[i].body, BOX2D_DEFAULT_FREQ, BOX2D_DEFAULT_DAMPING, false);		
            }
            else {
                joint.setup(box2d.getWorld(), circles[i-1].body, circles[i].body, BOX2D_DEFAULT_FREQ, BOX2D_DEFAULT_DAMPING, false);
            }
          
            joint.setLength(25);
            joints.push_back(joint);
        }
    }
    
    
    // -------------------------------------------------------
    void add(ofxBox2d &box2d) {
        // add a new circle
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), 
                     circles.back().getPosition().x+ofRandom(-30, 30), 
                     circles.back().getPosition().y-30, 2);
        
		circles.push_back(circle);
        
		// get this cirlce and the prev cirlce
		int a = (int)circles.size()-2;
		int b = (int)circles.size()-1; 
        
		// now connect the new circle with a joint
		ofxBox2dJoint joint;
		joint.setup(box2d.getWorld(), circles[a].body, circles[b].body);
		joint.setLength(5);
		joints.push_back(joint);
    }
    
    // -------------------------------------------------------
    ofxBox2dCircle& getHead() {
        return circles.back();
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
    
    
    // -------------------------------------------------------
    void drawAsVine() {
        
        vector <ofVec2f> verts;
        ofVec2f firstPt = anchor.getPosition();
        
        verts.push_back(ofVec2f(firstPt.x-20, firstPt.y));
        verts.push_back(ofVec2f(firstPt.x+20, firstPt.y));
        
     
        
        for(int i=1; i<circles.size(); i++) {
            ofVec2f a   = circles[i].getPosition();
            ofVec2f b   = circles[i-1].getPosition();
            ofVec2f vec = a - b;
            ofVec2f perp = vec.perpendicular();
           
            float r = ofMap(i, 1, circles.size()-1, 10, 4);
            perp *= r;
            
            ofVec2f pa = b + perp;
            ofVec2f pb = b - perp;
            
            
            verts.push_back(pa);
            verts.push_back(pb);
            //            verts.push_back(pb);

           // ofLine(a, a+(perp*10));
           // ofLine(a, a-(perp*10));
        }
        
        
        for(int i=1; i<circles.size(); i++) {
            ofVec2f a   = circles[i].getPosition();
            ofVec2f b   = circles[i-1].getPosition();
            ofSetColor(100, 0, 0);
            ofLine(a, b);
        }
        
        ofSetColor(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &verts[0]);
        glDrawArrays(GL_QUAD_STRIP, 0, (int)verts.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        
        
        return;
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