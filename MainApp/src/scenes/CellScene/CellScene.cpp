#include "CellScene.h"

// ----------------------------------------------------
void CellScene::setup() {
    name = "cell scene";
    
    string safeName = name;
    ofStringReplace(safeName, " ", "_");
    gui.setup(safeName);
    gui.add(separationDistance.setup("separation", 10, 1, 500));
    gui.add(speed.setup("speed", 0.004, 0.0003, 2.0));
    
}

// ----------------------------------------------------
void CellScene::update() {
    
    // reset the forces
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
        it->frc = 0;
    }
    
    float t = ofGetElapsedTimef()*0.02;
    
    for (vector<CellNode>::iterator itA=cells.begin(); itA!=cells.end(); ++itA) {
    
        ofVec2f pos = itA->pos;
        float frwX = ofNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
		float noiseDamp = speed;
        
        ofVec2f noiseFrc;
     	noiseFrc.x = frwX + ofSignedNoise(t, pos.y * 0.04) * 0.6;
		noiseFrc.y = ofSignedNoise(itA->uniquef, pos.x * 0.006, t);
        
        noiseFrc *= noiseDamp;
       
        
        
        ofVec2f sepFrc = 0;
        for (vector<CellNode>::iterator itB = itA; itB!=cells.end(); ++itB) {
            if (itA==itB) continue;
            
            ofVec2f v = itA->pos - itB->pos;
            float   d = v.length();
            if(d < separationDistance) {
                
                v.normalize();
                sepFrc += v;
            }
        }
        sepFrc.normalize();
        sepFrc.limit(0.4);
        
        // add forces
        itA->frc += noiseFrc;
        itA->frc += sepFrc;
        
        // applu the forces
		itA->vel *= itA->drag; 
		itA->vel += itA->frc * 0.4;
        itA->pos += itA->vel;
        
        // wrap the screen
        itA->wrapScreen(-200, -200, ofGetWidth()+400, ofGetHeight()+400);
    }
    
    
    
    // voronoi up this scene
    if(cells.size() > 0) {
        voronoi.clear();
        voronoi.addPoint(0, 0);
        voronoi.addPoint(ofGetWidth(), 0);
        voronoi.addPoint(ofGetWidth(), ofGetHeight());
        voronoi.addPoint(0, ofGetHeight());
        
        for (vector<CellNode>::iterator itA=cells.begin(); itA!=cells.end(); ++itA) {
            voronoi.addPoint(itA->pos);
        }
        bool bMadeVor = voronoi.generateVoronoi();
    }
}

// ----------------------------------------------------
void CellScene::draw() {
    
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
        it->draw();
    }
    
    
    for(vector<ofxVoronoiEdge>::iterator it=voronoi.edges.begin(); it!=voronoi.edges.end(); ++it) {
        it->draw();
    }
    
    // draw the gui
    gui.draw();
}

// ----------------------------------------------------
void CellScene::keyPressed(int key) {
    if(key == ' ') {
        
        CellNode c;
        c.setPosition(-100, ofRandom(-400, 400)+ofGetHeight()/2);
        cells.push_back(c);
        
    }
}