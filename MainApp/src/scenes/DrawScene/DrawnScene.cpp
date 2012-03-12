#include "DrawnScene.h"

// ----------------------------------------------------
void DrawnScene::setup() {
    name = "drawn scene";
    
    string safeName = name;
    ofStringReplace(safeName, " ", "_");
    gui.setup(name, safeName+".xml", 20, 20);
    
    gui.add(frequency.setup("frequency", 1, 0.0002, 10));
    gui.add(damping.setup("damping", 1, 0.0003, 1.0));
    
    
    bgColorTarget = ofRandomColor();
    
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(30.0);
	
    int nVines = 10;
    for(int i=0; i<nVines; i++) {
        float x = ofMap(i, 0, nVines-1, 20, CUBE_SCREEN_WIDTH-20);
        Viner v;
        v.make(x, 0, box2d);
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
        
        it->circles.back().addAttractionPoint(ofGetMouse(), .08);
    }    
    
    box2d.update();	
}



// ----------------------------------------------------
void DrawnScene::draw() {
    
    drawBackground();
    
    
    
    
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        //it->draw();
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
    
    
    if(key == 's') {
        int ranVine  = ofRandomIndex(vines);
        int ranJoint = ofRandomIndex(vines[ranVine].joints);
        
        //vines[ranVine].joints[ranJoint].destroy();
        //vines[ranVine].joints.erase( vines[ranVine].joints.begin() + ranJoint );// + ranJoint);
         
        
        
    }
}


// ----------------------------------------------------
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
		//bgColorTarget = cit->second;
		break;
	}
}