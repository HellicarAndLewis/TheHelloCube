//
//  Branch.h
//  emptyExample
//
//  Created by Todd Vanderlin on 2/22/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//


#pragma once
#include "ofMain.h"
#include "Utils.h"

class Branch {
public:
    
    ofVec2f     p,v,f;      // particle pts
    ofVec3f     sp,         // start point
                fp;         // finish point
    
    float      t;   
    ofPolyline pts;
    bool       hasBranched;
    bool       bCanBranch;
    int        maxPts;
    
    
    float      seed;        // a uniquiness to the branch
    float      amp;         // how fast am i
    float      drag;        // eath has some drag
    
    bool       grabFinish;  // grab the finish point when done
    
    
    
    Branch(float x, float y) {
        
        // seed used as a uniqueness
        seed = ofRandom(-50000, 50000) / 100000.0;
    
        // some draggin on earth
        drag = ofRandom(0.78, 0.98);	

        
        amp = ofRandom(0.02, 0.2);
        p.set(x, y);
        hasBranched = false;
        bCanBranch  = true;//(int)ofRandom(0, 10)>3?false:true;
        maxPts      = (int)ofRandom(10, 30);
        sp          = p;
        t           = 0;
        
        // toggles
        grabFinish  = false;
        
    }
    void update() {
        if(pts.size() < maxPts) {
            
            //f.x = cos(t) * amp;
            //f.y = sin(t) * -0.2;
            
            static float div = 0.001;
            float time = ofGetElapsedTimef() * 0.01;
            f.x = ofSignedNoise(seed, p.y * div, time);
            f.y = ofSignedNoise(seed, p.x * div, time);
            
            f.y -= 0.4;
            
            v += f * 0.4;
            v *= drag;
            p += v;
            pts.addVertex(p);
            
            t += 0.2;
        }
        else if(pts.size() == maxPts && !grabFinish) {
            fp = p;
            grabFinish = true;
        }
        
        // just live man
        else {
            
            v += f * 0.4;
            v *= drag;
            p += v;
            
        }
        
    }
    void draw() {
        ofSetColor(255);
        drawDot(sp, 2);
        
        ofSetColor(200);
        pts.draw();
    }
};