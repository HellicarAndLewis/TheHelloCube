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
#include "Hair.h"

class Massive {
    
public:
    
	vector <ofVec2f>	pts;
	vector <Hair>		hair;
    ofVec2f             root;
    int                 nHairs;
    bool                bMade;
    float               girth;
    float               seperation;  
    float               unique;
    
    //--------------------------------------------------------------
    Massive() {
        bMade = false;
        unique = ofRandomuf();
    }
    //--------------------------------------------------------------
    ~Massive() {
        
    }
    
    //--------------------------------------------------------------
    void make(float x, float y) {
        int nPts = ofRandom(10, 20);
        girth    = ofRandom(MAX(3, nPts-5), nPts);
        root.set(x, y);
        
        for (int i=0; i<nPts; i++) {
            pts.push_back(ofVec2f(x, y-i*2));
        }
        
        nHairs = ofRandom(50, 160);
        for (int i=0; i<nHairs; i++) {
            hair.push_back(Hair());
            hair[i].init(ofRandom(5, 10));
        }
        bMade = true;
        
        seperation = ofRandom(10, 20);
    }
    
    //--------------------------------------------------------------
    void update() {
        if(!bMade) return;
       
        float rnd = 0.2;
        float div = 500.0;
        float t   = ofGetElapsedTimef()*0.3;
        pts[0].set(ofGetMouseX(), ofGetMouseY());
        pts[0].set(root.x, root.y+seperation+4);
        
        
        ofVec2f noiseFrc;
        noiseFrc.x = ofSignedNoise(unique, t, pts[1].y/div);
        noiseFrc.y = ofSignedNoise(unique, pts[1].x/div, t);
        noiseFrc *= 0.3;
        
        pts[1] = pts[0] + noiseFrc;
        
        
        for (int i=2; i<pts.size(); i++) {
            
            ofVec3f v = pts[i] - pts[i-2];
            float   d = v.length();
            pts[i] = pts[i-1] + (v * seperation) / d;
            
            pts[i].y -= 0.3;
        }
        
        
    }
    
    
    //--------------------------------------------------------------
    void drawThick() {
        if(!bMade) return;
        vector <ofVec2f> perps;
        ofVec2f v, p;
        for(int i=2; i<pts.size(); i++) {
            
            v = pts[i] - pts[i-1];
            p = v.perpendicular();
            
            float r = ofMap(i, 2, pts.size()-1, girth, 2);
            p *= r * 2;
            
            perps.push_back(ofVec2f(pts[i] + p));
            perps.push_back(ofVec2f(pts[i] - p));            
            
        }
        ofPolyline linestrip;
        for(int i=0; i<perps.size(); i++) {
            if(i%2==0) linestrip.addVertex(perps[i]);   
        }
        for(int i=perps.size()-1; i>=0; i--) {
            if(i%2==1) linestrip.addVertex(perps[i]);   
        }
        linestrip.addVertex(perps[0]);
        
        linestrip = linestrip.getResampledByCount(nHairs);
        
        
        // find the general perp
        for(int i=1; i<linestrip.size(); i++) {
            v = linestrip[i] - linestrip[i-1];
            p = v.perpendicular();
            hair[i].dir = p;
        }
        
        for(int i=0; i<linestrip.size(); i++) {
            ofCircle(linestrip[i], 1);
            int hairIndex = MIN(i, hair.size()-1);
            hair[hairIndex].draw(linestrip[i], 3);
        }
        
        
        ofSetColor(20);
        glLineWidth(2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &linestrip.getVertices()[0]);
        glDrawArrays(GL_LINE_STRIP, 0, (int)linestrip.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        
        glLineWidth(1);
        
        
    }
    
    
    //--------------------------------------------------------------
    void draw() {
        if(!bMade) return;
        /*
         for (int i=1; i<pts.size(); i++) {
         ofSetHexColor(0x363328);
         ofLine(pts[i-1], pts[i]);
         }
         
         ofNoFill();
         for (int i=0; i<pts.size(); i++) {
         ofSetHexColor(0x363328);
         ofCircle(pts[i], 4);
         ofCircle(pts[i], 1);
         }*/
        
        drawThick();
        
    }
    
};
