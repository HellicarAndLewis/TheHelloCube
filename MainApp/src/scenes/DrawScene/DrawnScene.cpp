#include "DrawnScene.h"

// ----------------------------------------------------
void DrawnScene::setup() {
    name = "drawn scene";
    
    string safeName = name;
    ofStringReplace(safeName, " ", "_");
    gui.setup(name, safeName+".xml", 20, 20);
    
    gui.add(frequency.setup("frequency", 7, 0.0002, 10));
    gui.add(damping.setup("damping", 1, 0.0003, 1.0));
    
    
    bgColorTarget = ofRandomColor();
    
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(30.0);

	// TOP
    int nVines = 20;
    for(int i=0; i<nVines; i++) {
        float x = ofMap(i, 0, nVines-1, 20, CUBE_SCREEN_WIDTH-20);
        Viner v;
        v.pullForce.set(0, 2);
        v.make(x, 0, Viner::TOP, box2d);
        vines.push_back(v);
    }
    
    // BOTTOM
    nVines = 10;
    for(int i=0; i<nVines; i++) {
        float x = ofMap(i, 0, nVines-1, 20, CUBE_SCREEN_WIDTH-20);
        Viner v;
        v.pullForce.set(0, -2);
        v.make(x, CUBE_SCREEN_HEIGHT, Viner::BOTTOM, box2d);
        vines.push_back(v);
    }
}

// ----------------------------------------------------
void DrawnScene::update() {
	
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        
        for(int i=0; i<it->joints.size(); ++i) {
            it->joints[i].setFrequency(frequency);
            it->joints[i].setDamping(damping);
        }
        
        
    }
    
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        
        ofVec2f v = ofGetCenterScreen() -  it->circles.back().getPosition();
        v.normalize();
        
       // it->circles.back().addAttractionPoint(ofGetMouse(), .08);
        ofVec2f frc = it->pullForce;
        
        
        int nFrc = 0;
        for(vector<Chaser>::iterator itC=chasers.begin(); itC!=chasers.end(); ++itC) {
            ofVec2f vec = itC->pos - it->getHead()->getPosition();
            if(vec.length() <  220) {
                vec.normalize();
                frc += vec;
                ++nFrc;
            }
        }
        if(nFrc > 0) {
            frc /= nFrc;
        }
        it->circles.back().addForce(frc, 1);

    }   
    
    
    // Chasers
    float t = ofGetElapsedTimef()*0.02;
    for(vector<Chaser>::iterator it=chasers.begin(); it!=chasers.end(); ++it) {
        
        ofVec2f pos = it->pos;
        float frwX  = ofNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
        
        ofVec2f noiseFrc;
     	noiseFrc.x = frwX + ofSignedNoise(t, pos.y * 0.04) * 0.6;
		noiseFrc.y = ofSignedNoise(it->uniquef, pos.x * 0.006, t);
        noiseFrc *= 0.94;
  
        
        // avoid the vines
        ofVec2f vineFrc = 0;
        int nVecFrc = 0;
        for(vector<Viner>::iterator itV=vines.begin(); itV!=vines.end(); ++itV) {
            ofVec2f vec = itV->getHead()->getPosition() - it->pos;
            if(vec.length() < 60) {
                vec.normalize();
                vineFrc -= vec; // avoid
                ++nVecFrc;
            }
        }    
        if(nVecFrc > 0) {
            vineFrc /= nVecFrc;
            vineFrc *= 0.89;
        }
        
        
        // add forces
        it->frc += noiseFrc;        
        it->frc += vineFrc;        
        it->frc.limit(3.0);
        
        // applu the forces
		it->vel *= it->drag; 
		it->vel += it->frc;
        it->pos += it->vel;
        
        it->bRemove = isPointInScreen(it->pos) == false;
        
    }
    ofRemove(chasers, Particle::shouldRemove);

    
    box2d.update();	
}

// ----------------------------------------------------
void DrawnScene::addChasers() {
    
    Chaser p;
    p.radius = 3;
    p.setPosition(0, ofRandom(-300, 300) + CUBE_SCREEN_HEIGHT/2);
    
    chasers.push_back(p);
}

// ----------------------------------------------------
void DrawnScene::draw() {
    
    drawBackground();
    
 
    for(vector<Chaser>::iterator it=chasers.begin(); it!=chasers.end(); ++it) {
        
        it->nConnections = 0;

        if(it->nConnections < 2) {
            for(vector<Chaser>::iterator itB=it; itB!=chasers.end(); ++itB) {
                ofVec2f a = it->pos;
                ofVec2f b = itB->pos;
                if(a.distance(b) < 20) {
                    it->nConnections ++;
                    ofLine(it->pos, itB->pos);
                }
            }
        }
        
        ofSetColor(0);
        ofNoFill();
        it->draw();
    }

    
    // draw the vines
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        it->drawAsVine();
    }
    
    gui.draw();
    
}

// ----------------------------------------------------
void DrawnScene::keyPressed(int key) {
    
    if(key == 'b') {
        bgColorTarget = ofRandomColor();
    }
    
    if(key == ' ') {
        int ranVine = ofRandomIndex(vines);
        vines[ranVine].add(box2d);
    }
    
    // *****************************
    // *** working on this still ***
    // *****************************
    if(key == 's') {
        int ranVine  = ofRandomIndex(vines);
        int ranJoint = ofRandomIndex(vines[ranVine].joints);
        
        //vines[ranVine].joints[ranJoint].destroy();
        //vines[ranVine].joints.erase( vines[ranVine].joints.begin() + ranJoint );// + ranJoint);
    }
    
    if(key == 'c') {
        addChasers();
    }
}


// ----------------------------------------------------
// @todd you can handle commands here events to change the scene....
void DrawnScene::handleCommands(TwitterCommand& cmd, Effects& fx) {

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
		break;
	}
}