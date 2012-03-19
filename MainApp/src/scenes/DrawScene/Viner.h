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
    
    enum {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
    };
    
    enum {
        LINER,
        THICK
    };
    
    // -------------------------------------------------------
    ofxBox2dCircle					anchor;			  
    vector		<ofxBox2dCircle>	circles;		  
    vector		<ofxBox2dJoint>		joints;
    ofVec2f                         pullForce;
    int                             location;
    int                             type;
    ofColor                         color, colorDes;
    ofImage * dotImg;
    ofImage * triImg;
    float dotSize;
    float dotSizeD;
    bool  bPop;
    bool  bAddPopForceThisFrame;

    Viner() {
        
        dotSizeD = ofRandom(.03, 0.2);
        dotSize  = 0;
        bPop     = false;
        
        //default
        type = THICK;
        dotImg = NULL;
        triImg = NULL;
        bAddPopForceThisFrame = false;
        

    }
    
    
    // -------------------------------------------------------
    void make(float x, float y, int loc, ofxBox2d &box2d) {
        
        location = loc;
        anchor.setup(box2d.getWorld(), x, y, 4);
        
        int nStart = (int)ofRandom(3, 15);
        
        if(type == LINER) {
            nStart = 4;
        }
        
        if(loc == BOTTOM) {
            nStart = ofRandom(10, 15);
        }
        
        // first we add just a few circles
        for (int i=0; i<nStart; i++) {
            
            ofxBox2dCircle circle;
            circle.setPhysics(3.0, 0.53, 0.1);
            
            float ypos = 0;
            if(location == TOP) {
                ypos = y+(i*20);
            }
            else if(location == BOTTOM) {
                ypos = y-(i*20);
            }
            else if(location == LEFT) {
                
            }
            else if(location == RIGHT) {
                
            }
            
            circle.setup(box2d.getWorld(), x, ypos, 3);
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
            
            joint.setLength(20);
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
		joint.setLength(20);
		joints.push_back(joint);
    }
    
    // -------------------------------------------------------
    ofxBox2dCircle * getHead() {
        return &circles.back();
    }
    
    // -------------------------------------------------------
    void update() {
        
        dotSize += (dotSizeD-dotSize) * 0.1;
        
        color.r += (colorDes.r - color.r) * 0.2;
        color.g += (colorDes.g - color.g) * 0.2;
        color.b += (colorDes.b - color.b) * 0.2;
        color.a = 255;
        // ofXeno(dotSize, dotSizeD, 0.2);
        
        /*
        vine->dotSize  = 0;
        vine->dotSizeD = ofRandom(.03, 0.2);
        vine->dotImg   = &dots[(int)ofRandomIndex(dots)];
        
        if(bPop && abs(dotSize-dotSizeD) < 0.1) {
            bPop = false;
        }*/
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
        
        if(type == THICK) {
            vector <ofVec2f> verts;
            ofVec2f firstPt = anchor.getPosition();
            
            
            //verts.push_back(ofVec2f(firstPt.x-20, firstPt.y));
            //verts.push_back(ofVec2f(firstPt.x+20, firstPt.y));
            
            ofVec2f a, b;
            
            for(int i=0; i<circles.size(); i++) {
                
                
                a   = circles[i].getPosition();
                if(i == 0) {
                    b = anchor.getPosition();
                }
                else {
                    b = circles[i-1].getPosition();
                }
                
                ofVec2f vec = a - b;
                ofVec2f perp = vec.perpendicular();
                
                float r = ofMap(i, 1, circles.size()-1, 10, 4);
                perp *= r*2;
                
                ofVec2f pa = b + perp;
                ofVec2f pb = b - perp;
                
                if(location == TOP) {
                    pa.y -= 20;
                    pb.y -= 20;
                }
                else if(location == BOTTOM) {
                    pa.y += 20;
                    pb.y += 20;
                }
                
                verts.push_back(pa);
                verts.push_back(pb);
                //            verts.push_back(pb);
                
                // ofLine(a, a+(perp*10));
                // ofLine(a, a-(perp*10));
                
                if(triImg != NULL) {
                    ofSetColor(color);
                        
                    if(location == BOTTOM) {
                        float triScale = ofMap(i, 1, circles.size()-1, 0.5, 0.2);
                        float angle = ofRadToDeg( atan2(perp.y, perp.x) );
                        float offx  = -(((triImg->getWidth()*triScale)/2) + r);
                        float offy  = (triImg->getHeight()*triScale)/2;
                        
                        triImg->setAnchorPoint(offx, offy);
                        ofPushMatrix();
                        ofTranslate(b.x, b.y+20);
                        ofRotate(angle);
                        ofPushStyle();
                        ofSetRectMode(OF_RECTMODE_CENTER);
                        triImg->draw(0, offy+r, triImg->getWidth()*triScale, triImg->getHeight()*triScale);
                        ofPopStyle();
                        ofPopMatrix();
                        triImg->resetAnchor();
                        
                        
                        triImg->setAnchorPoint(offx, offy);
                        ofPushMatrix();
                        ofTranslate(b.x, b.y+20);
                        ofRotate(angle+180);
                        ofPushStyle();
                        ofSetRectMode(OF_RECTMODE_CENTER);
                        triImg->draw(0, 0, triImg->getWidth()*triScale, triImg->getHeight()*triScale);
                        ofPopStyle();
                        ofPopMatrix();
                        triImg->resetAnchor();
                        
                    
                    
                    }
                }
            }
            
            if(location==TOP) {
                vector <ofVec2f> linestrip;
                for(int i=0; i<verts.size(); i++) {
                    if(i%2==0) linestrip.push_back(verts[i]);   
                }
                for(int i=verts.size()-1; i>=0; i--) {
                    if(i%2==1) linestrip.push_back(verts[i]);   
                }
                
                
                ofSetColor(color);
                glLineWidth(2);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(2, GL_FLOAT, 0, &linestrip[0]);
                glDrawArrays(GL_LINE_STRIP, 0, (int)linestrip.size());
                glDisableClientState(GL_VERTEX_ARRAY);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        
                glLineWidth(1);
            }
            else {
                ofSetColor(color);
                glLineWidth(2);
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(2, GL_FLOAT, 0, &verts[0]);
                glDrawArrays(GL_QUAD_STRIP, 0, (int)verts.size());
                glDisableClientState(GL_VERTEX_ARRAY);
                glLineWidth(1);
            }
        }
        
        else if(type == LINER) {
            
            ofSetColor(color);
            ofNoFill();
            ofBeginShape();
            ofPoint anc = anchor.getPosition();
            ofVertex(anc);
            for(int i=0; i<circles.size(); i++) {
                ofPoint a   = circles[i].getPosition();
                ofVertex(a);
            }
            ofEndShape();
            
            if(dotImg!=NULL) {
                ofPushStyle();
                ofSetColor(color);
                ofEnableAlphaBlending();
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofPoint dp = getHead()->getPosition();
                dotImg->draw(dp, dotImg->getWidth()*dotSize, dotImg->getHeight()*dotSize);
                ofPopStyle();
            }
            
        }
    }
    
};