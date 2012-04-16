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

class Wiggler {
    
public:
    
    enum {
        NORMAL,
        LAND_WIGGLE
    };
    
	vector <ofVec2f>	pts;
	vector <Hair>		hair;
    ofVec2f             root, dir;
    int                 nHairs;
    bool                bMade;
    float               girth;
    float               seperation;  
    float               unique;
    float               div;
    ofTexture  *        txt;
    ofTexture  *        spikeTex;
    float               soundAmp;
    ofColor             color, colorD;
    int                 resampleCount;
    
    int                 type;
    float               thickness;
    
    //--------------------------------------------------------------
    Wiggler() {
        soundAmp = 0.1;
        txt      = NULL;
        spikeTex = NULL;
        bMade    = false;
        unique   = ofRandomuf();
        div      = ofRandom(200, 900);

    }
    //--------------------------------------------------------------
    ~Wiggler() {
        
    }
    
    //--------------------------------------------------------------
    void make(float x, float y, int type) {
        
        this->type = type;
        root.set(x, y+20);
        nHairs = 22;
        
        int nPts = ofRandom(20, 30);
        girth    = ofRandom(MAX(3, nPts-5), nPts);
        
        // -------------------------
        // land wiggle
        // -------------------------
        if(type == LAND_WIGGLE) {
            nPts  = ofRandom(5, 10);
            girth = ofRandom(4, 8);
            thickness = ofRandom(0.3, 2);
            resampleCount = 20;
        }
        
        // add points
        for (int i=0; i<nPts; i++) {
            pts.push_back(ofVec2f(x, y-i*2));
        }
        
        // -------------------------
        // Normal
        // -------------------------
        if(type == NORMAL) {
            resampleCount = 30;
            nHairs = ofRandom(50, 100);
            for (int i=0; i<nHairs; i++) {
                hair.push_back(Hair());
                hair[i].init(ofRandom(5, 10));
            }
            
            thickness = 2;
        }
            
        bMade = true;
        seperation = ofRandom(10, 20);
    }
    
    //--------------------------------------------------------------
    void update() {
        if(!bMade) return;
       
        float rnd = 0.2;
        float t   = ofGetElapsedTimef() * 0.3;
        pts[0].set(ofGetMouseX(), ofGetMouseY());
        
        if(type == NORMAL) {
            pts[0].set(root.x, root.y+seperation+4);
        }
        else if(type == LAND_WIGGLE) {
            pts[0].set(root.x, root.y);

        }
        
        ofVec2f noiseFrc;
        noiseFrc.x = ofSignedNoise(unique, t, pts[1].y/div);
        noiseFrc.y = ofSignedNoise(unique, pts[1].x/div, t);
        noiseFrc *= 0.3;
        
        pts[1] = pts[0] + noiseFrc;
        
        
        for (int i=2; i<pts.size(); i++) {
            
            ofVec3f v = pts[i] - pts[i-2];
            float   d = v.length();
            pts[i]    = pts[i-1] + (v * seperation) / d;
            
            if(type == NORMAL) {
                pts[i].y -= 0.3;
            }
            else if(type == LAND_WIGGLE) {
                pts[i] += dir;
            }
        }
        
        
        
        
        color.r += (colorD.r - color.r) * 0.2;
        color.g += (colorD.g - color.g) * 0.2;
        color.b += (colorD.b - color.b) * 0.2;
        color.a = 255;
    }
    
    
    //--------------------------------------------------------------
    void draw() {
        if(!bMade) return;
        
        // resample to get better points
        ofPolyline resampled;
        for(int i=0; i<pts.size(); i++) {
            resampled.addVertex(pts[i].x, pts[i].y);
        }
        resampled = resampled.getResampledByCount(resampleCount);
        ofEnableAlphaBlending();
        
       
        // -----------------
        // repeating tex
        // -----------------
        glEnable(txt->texData.textureTarget);
        glBindTexture(txt->texData.textureTarget, txt->texData.textureID);
        glTexParameterf(txt->texData.textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(txt->texData.textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(txt->texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(txt->texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        ofSetColor(color);
        
        ofVec2f v, p, a, b;
        vector <ofVec2f> perps;
        
        glBegin (GL_QUAD_STRIP);
        for(int i=0; i<resampled.size()-1; i++) {
            
            a = resampled[i];
            b = resampled[i+1];
            
            v = a - b;
            p = v.perpendicular();
            
            float r = ofMap(i, 0, resampled.size()-2, girth, 2); 
            p *= r;
            
            float w = (girth*2) / 64.0;
            float per = ofMap(i, 0, resampled.size()-2, 0, seperation*(resampled.size()-2), true) / 64.0;
            glTexCoord2f(0.0, per);  glVertex2f(a.x - p.x, a.y - p.y);
            glTexCoord2f(w, per);    glVertex2f(a.x + p.x, a.y + p.y);
            
            
            perps.push_back(ofVec2f(a.x - p.x, a.y - p.y));
            perps.push_back(ofVec2f(a.x + p.x, a.y + p.y));            
            
        }
        glEnd ();
        glDisable(txt->texData.textureTarget);
        // -----------------
        // end repeat tex
        // -----------------
        
        
        // -----------------
        // line strip
        // -----------------
        ofPolyline linestrip;
        for(int i=0; i<perps.size(); i++) {
            if(i%2==0) linestrip.addVertex(perps[i]);   
        }
        for(int i=perps.size()-1; i>=0; i--) {
            if(i%2==1) linestrip.addVertex(perps[i]);   
        }
        linestrip.addVertex(perps[0]);
        linestrip = linestrip.getResampledByCount(nHairs);
      
        // draw fast
        ofSetColor(20);
        glLineWidth(thickness);
        linestrip.draw();
        /*
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &linestrip.getVertices()[0]);
        glDrawArrays(GL_LINE_STRIP, 0, (int)linestrip.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        
        */
        glLineWidth(1);
        
        if(type == NORMAL) {
            // find the general perp
            for(int i=1; i<linestrip.size(); i++) {
                v = linestrip[i] - linestrip[i-1];
                p = v.perpendicular();
                hair[i].dir = p;
            }
            
            for(int i=0; i<hair.size(); i++) {
                int hairIndex = MIN(i, hair.size());
                hair[hairIndex].rnd = soundAmp;
                hair[hairIndex].draw(linestrip[i], 3);
            }
        }
        
                
    }
    
    
    //--------------------------------------------------------------
    /*void draw() {
        if(!bMade) return;
        
         for (int i=1; i<pts.size(); i++) {
         ofSetHexColor(0x363328);
         ofLine(pts[i-1], pts[i]);
         }
         
         ofNoFill();
         for (int i=0; i<pts.size(); i++) {
         ofSetHexColor(0x363328);
         ofCircle(pts[i], 4);
         ofCircle(pts[i], 1);
         }
        
       // drawThick();
        
    }
    */
};
