//
//  Viner.h
//  MainApp
//
//  Created by Todd Vanderlin on 3/11/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "BaseScene.h"
#include "Hair.h"



class TinyVine {
    
public:
    
	vector <ofVec2f>	pts;
	vector <Hair>		hair;
    ofVec2f             root;
    int                 bMade;
    float               unique;
    ofImage *           headImg;
    float               headSizePct;
    ofColor             color, colorD;
    int                 maxLength;
    bool                bShrinking;
    int                 thickness;
    int                 side;
    
    //--------------------------------------------------------------
    TinyVine() {
        bMade       = false;
        unique      = ofRandomuf();
        headSizePct = ofRandom(0.1, 0.2);
        headImg     = NULL;
        bShrinking  = false;
        maxLength   = ofRandom(6, 20);
        thickness   = (int)ofRandom(1, 3);
    }
    
    //--------------------------------------------------------------
    ~TinyVine() {
        
    }
    
    //--------------------------------------------------------------
    void make(float x, float y, int nPts) {
        root.set(x, y);
        for (int i=0; i<nPts; i++) {
            pts.push_back(ofVec2f(x, y-i*2));
        }
        bMade = true;
    }
    
    //--------------------------------------------------------------

    void grow() {
        if(pts.size() < maxLength && !bShrinking) {
            pts.push_back(pts.back());
        }else {
            bShrinking = true;
            
        }
    }
    
    //--------------------------------------------------------------
    void update() {
        
        if(!bMade) return;
        
        color.r += (colorD.r - color.r) * 0.2;
        color.g += (colorD.g - color.g) * 0.2;
        color.b += (colorD.b - color.b) * 0.2;
        color.a = 255;
        
        float seperation    = 10;
        float rnd           = 0.2;
        float div           = 500.0;
        float t             = ofGetElapsedTimef()*0.3;
        float offset        = 0;
        
        if(side == BaseScene::TOP) {
            offset = -(seperation+4);
        }
       
        pts[0].set(root);
        pts[0].set(root.x, root.y+offset);
        ofVec2f noiseFrc;
        noiseFrc.x = ofSignedNoise(unique, t, pts[1].y/div);
        noiseFrc.y = ofSignedNoise(unique, pts[1].x/div, t);
        noiseFrc *= 0.53;
        
        pts[1] = pts[0] + noiseFrc;
        
        for (int i=2; i<pts.size(); i++) {
            
            ofVec3f v = pts[i] - pts[i-2];
            float   d = v.length();
            pts[i] = pts[i-1] + (v * seperation) / d;
            
            pts[i].y += 1;
            
        }
        
        
        if(bShrinking && pts.size() > 3 && (ofGetFrameNum()%4==0)) {
            pts.erase(pts.begin());
            if(pts.size()<=3) {
                bShrinking = false;
                if(side == BaseScene::TOP) {
                    root.x = ofRandom(50, CUBE_SCREEN_WIDTH-50);
                     maxLength   = ofRandom(6, 20);
                }
            }
        }
    }
   
    //--------------------------------------------------------------
    void draw() {
        if(!bMade) return;
        ofSetColor(20);
        glLineWidth(thickness);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &pts[0]);
        glDrawArrays(GL_LINE_STRIP, 0, (int)pts.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        glLineWidth(1);
        
        ofVec2f a = pts.back();
        ofVec2f b = pts[(int)pts.size()-1];
        float rotation = a.angle(b);
        float sz       = ofMap(pts.size(), 0, maxLength, 0.01, headSizePct, true);
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(color);
        if(headImg!=NULL) {
            ofPushMatrix();
            ofTranslate(pts.back());
            ofRotate(rotation);
            ofSetRectMode(OF_RECTMODE_CENTER);
            headImg->draw(0, 0, headImg->getWidth()*sz, headImg->getHeight()*sz);
            ofPopMatrix();
        }
        ofPopStyle();
    }
    
};
