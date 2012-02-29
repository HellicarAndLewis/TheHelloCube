//
//  Plant.cpp
//  emptyExample
//
//  Created by Todd Vanderlin on 2/22/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#include "Plant.h"

float t = 0;
int maxPts      = 20;
int maxBranches = 100;
// ---------------------------------------------------
Plant::Plant() {
}

// ---------------------------------------------------
void Plant::grow(float x, float y) {
    
    bSplit = false;
    startPt.set(x, y);  
    
    p    = startPt;    
    t    = 0;
    v.set(0, -ofRandom(1.0, 2.0));
    maxBranches = (int)ofRandom(10, 100);
    branches.clear();
    pts.clear();
    
    printf("Max Branches %i\n", maxBranches);
}

// ---------------------------------------------------
void Plant::update() {
    
    if(pts.size() < maxPts) {
        
        //p.x = sin( (t * 0.03) ) * (cos(t*0.002) * 30.0);
        //p.y = (-t + cos(t * 0.02) * 20.0);
        
        p += v;
        //v *= 0.6;
        
        t ++;
        
        pts.addVertex(p);
        
    }
    else {
        if(!bSplit) {
            
            Branch br(p.x, p.y);
            br.maxPts = (int)ofRandom(10, 30);
            br.v      = v;
            br.bCanBranch = true;
            branches.push_back(br);
            
            bSplit = true;
            printf("Split\n");
        }
    }
    
    // the branches
        
    for(int i=0; i<branches.size(); i++) {
        
        branches[i].update();
        
        if(branches.size() < maxBranches && branches[i].bCanBranch) {
            if(!branches[i].hasBranched && branches[i].pts.size() >= branches[i].maxPts) {
                branches[i].hasBranched = true;

                Branch brA(branches[i].p.x, branches[i].p.y);
                //brA.maxPts = branches[i].maxPts - 1;
                brA.v      = branches[i].v;
                brA.v.x   -= ofRandom(0.4, 2.0);
               
                Branch brB(branches[i].p.x, branches[i].p.y);
                //brB.maxPts = branches[i].maxPts - 1;
                brB.v      = branches[i].v;
                brB.v.x   += ofRandom(0.4, 2.0);
               
                
                branches.push_back(brA);
                branches.push_back(brB);
                
                // maxBranches --;
                printf("Branch %i : %i\n", i, maxBranches);
            }
        }
        
        
    }
}

// ---------------------------------------------------
void Plant::draw() {
 
    ofSetColor(255);
    drawDot(startPt);
    
    ofSetColor(200);
    pts.draw();
    
    // the branches
    for(int i=0; i<branches.size(); i++) {
        branches[i].draw();
    }
    
}