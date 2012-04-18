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

class Hair {
public:
	ofPolyline pts;
    ofVec2f    dir;
    bool       bMade;
    float      rnd;
    int        landHairIndex[2];
    Hair() {
        landHairIndex[0] = 0;
        landHairIndex[1] = 0;
        bMade = false;
        rnd   = 0.92;
    }
    void init(int n) {
        for (int i=0; i<n; i++) {
            pts.addVertex(ofRandomf(), ofRandomf());
        }
        bMade = true;
    }
    int size() {
        return (int)pts.size();
    }
    void draw(ofVec2f base, float seperation) {
        if(!bMade) return;
        if(pts.size()>3) {
            pts[0] = base;
            pts[1] = base;
            pts[1].x += ofRandom(-rnd, rnd);
            pts[1].y += ofRandom(-rnd, rnd);
            for (int i=2; i<pts.size(); i++) {
                ofVec3f v = pts[i] - pts[i-2];
                float   d = v.length();
                pts[i] = pts[i-1] + (v * seperation) / d;
                pts[i] += dir;
            }
            pts.draw();
        }
    }
};
