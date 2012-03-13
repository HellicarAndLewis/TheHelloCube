#include "CellScene.h"

// ----------------------------------------------------
void CellScene::setup() {
    name = "cell scene";
    
    string safeName = name;
    ofStringReplace(safeName, " ", "_");
    gui.setup(name, safeName+".xml", 20, 20);
    gui.add(separationDistance.setup("separation", 10, 1, 500));
    gui.add(damping.setup("damping", 0.4, 0.0003, 1.0));
    gui.add(maxParticleSpeed.setup("speed", 0.1, 0.0003, 2.0));
    
    gui.add(maxCellsOnScreen.setup("max cells", 500, 1, 1000));
    gui.add(releaseRate.setup("release rate", 0.2, 0.0, 1.0));
    
    // we need some nice colors...
    bgColorTarget = ofRandomColor();
    
}

// ----------------------------------------------------
void CellScene::update() {
    
    // reset the forces & remove deads
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
        it->frc = 0;
        it->bRemove = isPointInScreen(it->pos, 100) == false;
    }
    ofRemove(cells, Particle::shouldRemove);
    
    
    float t = ofGetElapsedTimef()*0.02;
    
    for (vector<CellNode>::iterator itA=cells.begin(); itA!=cells.end(); ++itA) {
        
        ofVec2f pos = itA->pos;
        float frwX  = ofNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
        
        ofVec2f noiseFrc;
     	noiseFrc.x = frwX + ofSignedNoise(t, pos.y * 0.04) * 0.6;
		noiseFrc.y = ofSignedNoise(itA->uniquef, pos.x * 0.006, t);
        noiseFrc *= damping;
        
        
        
        ofVec2f sepFrc = 0;
        for (vector<CellNode>::iterator itB = itA; itB!=cells.end(); ++itB) {
            if (itA==itB) continue;
            ofVec2f v = itA->pos - itB->pos;
            float   d = v.length();
            float minRad = separationDistance + (itA->radius+itB->radius);
            if(d < minRad) {
                v.normalize();
                sepFrc += v;
            }
        }
        sepFrc.normalize();
        //sepFrc.limit(1.9);
        
        
        
        // add forces
        itA->frc += noiseFrc;
        itA->frc += sepFrc;
        
        itA->frc.limit(maxParticleSpeed);
        
        // applu the forces
		itA->vel *= itA->drag; 
		itA->vel += itA->frc;
        itA->pos += itA->vel;
        
        // wrap the screen
        //itA->wrapScreen(-200, -200, CUBE_SCREEN_WIDTH+400, CUBE_SCREEN_HEIGHT+400);
    }
    
    
    // voronoi up this scene
    if(cells.size() > 0) {
        voronoi.clear();
        voronoi.addPoint(0, 0);
        voronoi.addPoint(CUBE_SCREEN_WIDTH, 0);
        voronoi.addPoint(CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
        voronoi.addPoint(0, CUBE_SCREEN_HEIGHT);
        
        for (vector<CellNode>::iterator itA=cells.begin(); itA!=cells.end(); ++itA) {
            voronoi.addPoint(itA->pos);
        }
        bool bMadeVor = voronoi.generateVoronoi();
    }
    
    
    // add some more
    int nRelease = MAX(1, 50 * releaseRate);
    for(int i=0; i<nRelease; i++) {
        addCells();
    }
}

// ----------------------------------------------------
void CellScene::addCells() {
    if(cells.size() < maxCellsOnScreen) {
        CellNode c;
        c.setPosition(-50, ofRandom(-400, 400)+CUBE_SCREEN_HEIGHT/2);
        cells.push_back(c);
    }
}

// ----------------------------------------------------
void CellScene::draw() {
    
    drawBackground();
    
    ofSetColor(255);
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
        it->draw();
    }
    
    
    ofSetColor(0);
    for(vector<ofxVoronoiEdge>::iterator it=voronoi.edges.begin(); it!=voronoi.edges.end(); ++it) {
        it->draw();
    }
    
    
    // draw the gui
    gui.draw();
}

// ----------------------------------------------------
void CellScene::keyPressed(int key) {
    if(key == ' ') {
        
        addCells();
        
    }
    
    if(key == 'r') {
        for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
            it->radius = ofRandom(10, separationDistance);
        }    
    }
    
    if(key == 'b') {
        bgColorTarget = ofRandomColor();
    }
}

// ----------------------------------------------------
void CellScene::handleCommands(TwitterCommand& cmd, Effects& fx) {
	// handle commands.
	set<string>::const_iterator it = cmd.tokens.begin();
	while(it != cmd.tokens.end()) {
		const string& c = (*it);
		if(c == "bigger") {
		}
		else if(c == "smaller") {
		}
		fx.applyEffect(c);
		++it;
	}
	
	// handle colour
	map<string, ofColor>::const_iterator cit = cmd.colours.begin();
	while(cit != cmd.colours.end()) {	
		bgColorTarget = cit->second;
		break;
	}
}