#include "CellScene.h"

// ----------------------------------------------------
void CellScene::setup() {
    name = "cell scene";
}

// ----------------------------------------------------
void CellScene::update() {
    
    // reset the forces
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
        it->frc = 0;
    }
    
    
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
    
        ofVec2f pos = it->pos;
        float frwX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
		
     	it->frc.x = frwX * 0.25 + ofSignedNoise(it->uniquef, pos.y * 0.04) * 0.6;
		it->frc.y = ofSignedNoise(it->uniquef, pos.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;
        
		it->vel *= it->drag; 
		it->vel += it->frc * 0.4;
        it->pos += it->vel;
    }
}

// ----------------------------------------------------
void CellScene::draw() {
    
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
        it->draw();
    }
    
}

// ----------------------------------------------------
void CellScene::keyPressed(int key) {
    if(key == ' ') {
        
        CellNode c;
        c.setPosition(100, ofRandom(-400, 400)+ofGetHeight()/2);
        cells.push_back(c);
        
    }
}