#include "CellScene.h"
static bool shouldRemove(const CellNode &p) {
    return p.bRemove;
}
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
    
    drawGUI = false; //for screen grabbing...   
    
    cellWallWidth = 1.f; //v thin line to start...
}

// ----------------------------------------------------
void CellScene::update() {
    
    // reset the forces & remove deads
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
        it->frc = 0;
        it->bRemove = isPointInScreen(it->pos, 100) == false;
    }
    ofRemove(cells, shouldRemove);
    
    
    float t = ofGetElapsedTimef()*0.02;
    
    ofVec2f dirVec = 0;    
    for(int i=0; i<4; i++) {
        
        float amp = audioPtr->getVolume(i) * 10.0;
        
        if(i == AudioManager::TOP) {
            dirVec.y -= amp;
        }
        else if(i == AudioManager::BOTTOM) {
             dirVec.y += amp;
        }
        else if(i == AudioManager::LEFT) {
             dirVec.x -= amp;
        }
        else if(i == AudioManager::RIGHT) {
             dirVec.x += amp;
        }
        
    }
    
    dirVec *= 100;
    //printf("%f %f\n", dirVec.x, dirVec.y);
    
    for (vector<CellNode>::iterator itA=cells.begin(); itA!=cells.end(); ++itA) {
        
        ofVec2f pos = itA->pos;
        float frwX  = ofNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
        
        ofVec2f noiseFrc;
     	noiseFrc.x = ofSignedNoise(t, pos.y * 0.04) * 0.6;
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
        
        itA->frc += dirVec;
        
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
        ofColor newColourForCellNode = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())];
        CellNode c;
        c.setPosition(ofRandom(0, CUBE_SCREEN_WIDTH), ofRandom(-400, 400)+CUBE_SCREEN_HEIGHT/2);
        c.colour = newColourForCellNode;
        cells.push_back(c);
    }
}

// ----------------------------------------------------
void CellScene::draw() {
    
    drawBackground();
    
    ofColor peteBlack = ofColor(0,4,2);
    
    ofSetColor(peteBlack);
    ofFill();
    ofRect(0, 0, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
    
    //draws the dot in the centre
    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
        it->draw();
    }
    
    //draws lines between cell nodes as in Screen shot 2012-03-15 at 18.21.41.png from pete
    //not quite...
//    for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
//        //it->draw();
//        for (vector<CellNode>::iterator jt=cells.begin(); jt!=cells.end(); ++jt) {
//            //it->draw();
//            ofSetColor(it->colour);
//            ofLine(it->pos, jt->pos);
//        } 
//    }    
    
    ofPushStyle();
//    ofEnableSmoothing();
    
    ofSetLineWidth(cellWallWidth);
    
    //draws the edges
    ofSetColor(BaseScene::twitterColour);
    for(vector<ofxVoronoiEdge>::iterator it=voronoi.edges.begin(); it!=voronoi.edges.end(); ++it) {
        //ofSetColor(complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]);
        it->draw();
    }
    
//    ofDisableSmoothing();
    ofPopStyle();
    
    // draw the gui
    if(drawGUI){
        
        gui.draw();
    
    
        for(int i=0; i<4; i++) {

            float amp = audioPtr->getVolume(i);
            ofVec2f pos;
            // TOP
            if(i == AudioManager::TOP) {
                pos.set(CUBE_SCREEN_WIDTH/2, 20);
            }
            
            // BOTTOM
            if(i == AudioManager::BOTTOM) {
                pos.set(CUBE_SCREEN_WIDTH/2, CUBE_SCREEN_HEIGHT-20);
            }
            
            // LEFT
            if(i == AudioManager::LEFT) {
                pos.set(20, CUBE_SCREEN_HEIGHT/2);
            }
            
            // RIGHT
            if(i == AudioManager::RIGHT) {
                pos.set(CUBE_SCREEN_WIDTH-20, CUBE_SCREEN_HEIGHT/2);
            }
            
            
            // draw
            ofSetColor(255);
            ofCircle(pos, 25);
            ofSetColor(255, 0, 255);
            ofCircle(pos, 5+(20*amp));
            ofSetColor(0, 250, 250);
            ofDrawBitmapString(AudioManager::getSideName(i), pos);
        }
    }
}

void CellScene::respondToNewComplimentaryColours(){
    for(vector<CellNode>::iterator it=cells.begin(); it!= cells.end(); ++it) {
        it->colour = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())];
    }    
}

// ----------------------------------------------------
void CellScene::keyPressed(int key) {
    switch (key) { 
        case ' ': {
                addCells();
                break;
            }    
        case 'r': {
            for (vector<CellNode>::iterator it=cells.begin(); it!=cells.end(); ++it) {
                it->radius = ofRandom(10, separationDistance);
            }    
            break;
            }
        case 'b': {
            bgColorTarget = ofRandomColor();
            break;
        }
        
        case 'g': {
            drawGUI = !drawGUI;
            break;
        }
        default:{
            break;
        }
	}  
}

// ----------------------------------------------------
void CellScene::handleCommands(TwitterCommand& cmd, Effects& fx) {
	printf("Cellscene handle commands.\n");
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
		BaseScene::twitterColour = cit->second;
		bgColor = BaseScene::twitterColour;
        generateComplimentaryColours();
        respondToNewComplimentaryColours();
		break;
	}
}