#include "TextureScene.h"


// ----------------------------------------------------
void TextureScene::setup() {
    name = "texture scene";    
    string safeName = name;
	
	int gui_w = 500;
    ofStringReplace(safeName, " ", "_");
	string gui_file = safeName+".xml";
	
	gui.setSize(gui_w, 300);
    gui.setup(name,gui_file,CUBE_SCREEN_WIDTH-(gui_w+20), ofGetHeight()-150);
    gui.add(maxShapesOnScreen.setup("max shapes", 250, 1, 1000, gui_w));
    gui.add(releaseRate.setup("release rate", 0.2, 0.0, 1.0, gui_w));
	gui.add(repulsionForce.setup("repulsion force", 0.002, 0.0001, 0.2, gui_w));
	gui.add(attractionForce.setup("attraction force", 0.03, 0.01, 0.2, gui_w));
	gui.add(shapeBounceForce.setup("bounce force", 0.001, 0.0001, 0.01, gui_w));
	gui.add(radialForce.setup("radial force", 0.4, 0.0001, 1.0, gui_w));
    gui.loadFromFile(gui_file);
	  
 //   ofDisableArbTex();
    for(int i=1; i<26; i++) {
        textures.push_back(ofTexture());
        if(!ofLoadImage(textures.back(), "graphics/texture/"+ofToString(i)+".png")) printf("error loading: '%d.png'\n", i);
    }
	
    // setup box2d
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.setFPS(30);
    
    circleFrcFlip = false;
    bgColorTarget = ofRandomColor();    
    
    Attractor attTL;
    attTL.pos.set(20, 20);
    attractors.push_back(attTL);
    
    Attractor attTR;
    attTR.pos.set(CUBE_SCREEN_WIDTH-20, 20);
    attractors.push_back(attTR);
    
    Attractor attBL;
    attBL.pos.set(20, CUBE_SCREEN_HEIGHT-20);
    attractors.push_back(attBL);
    
    Attractor attBR;
    attBR.pos.set(CUBE_SCREEN_WIDTH-20, CUBE_SCREEN_HEIGHT-20);
    attractors.push_back(attBR);
    
   
    
}


// ----------------------------------------------------
void TextureScene::exitScene() {
    BaseScene::exitScene();
}

// ----------------------------------------------------
void TextureScene::update() {
    
    for(int i=0; i<attractors.size(); i++) {
        if(i == 0) {
            attractors[i].amp = audioPtr->getVolume(AudioManager::TL);
        }
        if(i == 1) {
            attractors[i].amp = audioPtr->getVolume(AudioManager::TR);
        }
        if(i == 2) {
            attractors[i].amp = audioPtr->getVolume(AudioManager::BL);
        }
        if(i == 3) {
            attractors[i].amp = audioPtr->getVolume(AudioManager::BR);
        }
    }
    
    int nShapesToAdd = MAX(1, 10 * releaseRate);
    for(int i=0; i<nShapesToAdd; i++) {
        addShape();
    }
    
    for (vector<TexturedShape>::iterator it=shapes.begin(); it!=shapes.end(); ++it) {
        it->update();
        it->addAttractionPoint((circleFrc*radialForce) + getCentreCubeScreen(), attractionForce);

        if(it->getPosition().distance(getCentreCubeScreen()) < 300) {
            it->addRepulsionForce(getCentreCubeScreen(), repulsionForce);
        }
        
        for(int i=0; i<attractors.size(); i++) {
            if(attractors[i].getAmp() > 0.3) {
                it->addAttractionPoint(attractors[i].pos, attractors[i].getAmp()*0.2);
            }
        }
        
    }
    box2d.update();
}

// ----------------------------------------------------
void TextureScene::addPoints() {
    pts.clear();
    for(int i=0; i<6; i++) {
        float x = ofRandom(10, CUBE_SCREEN_WIDTH-10);
        float y = ofRandom(10, CUBE_SCREEN_HEIGHT-10);
        pts.push_back(ofVec2f(x, y));
    }   
    tris = triangulatePolygon(pts);
}


// ----------------------------------------------------
void TextureScene::addShape() {
    if(shapes.size() < maxShapesOnScreen) {
        ofVec2f pt = getCentreCubeScreen();
        float r = ofRandom(10, 30);
        pt.x += cos(ofRandomuf()*TWO_PI) * r;
        pt.y += sin(ofRandomuf()*TWO_PI) * r;
        
        TexturedShape shape;
        shape.colour = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())];
        shape.setPhysics(2.5, shapeBounceForce, 0.0001); // rox: looks like 3rd param isn't doing anything. set density to 2.5 made the anim more smooth.
        shape.setup(box2d.getWorld(), pt, 1);
        shape.radiusTarget = ofRandom(10, 30);
        shape.tex = &textures[(int)ofRandom(0, textures.size()-1)];
        shapes.push_back(shape);
    }
}


// ----------------------------------------------------
void TextureScene::makeObstacles() {
    obsticals.clear();
    for (vector<TriangleShape>::iterator it=tris.begin(); it!=tris.end(); ++it) {
        
        float   radius = GeometryUtils::getTriangleRadius(it->c, it->b, it->a);
        ofVec2f center = GeometryUtils::getTriangleCenter(it->a, it->b, it->c);
        
        ofxBox2dCircle ob;
        ob.setPhysics(0, 0.1, 1);
        ob.setup(box2d.getWorld(), center, radius);
        obsticals.push_back(ob);
    }  
}


// ----------------------------------------------------
void TextureScene::keyPressed(int key) {
    if(key == ' ') {
        //addPoints();
        //makeObstacles();
    }
    
    if(key == 'r') {
        for(int i=0; i<MIN(30, shapes.size()-1); i++) {
            int ranId = (int)ofRandom(0, shapes.size()-1);
            shapes[ranId].radiusTarget = ofRandom(30, 100);
        }
    }
    if(key == 's') {
        for(int i=0; i<MIN(230, shapes.size()-1); i++) {
            int ranId = (int)ofRandom(0, shapes.size()-1);
            shapes[ranId].radiusTarget = ofRandom(2, 10);
        }
    }
    
    if(key == 'b') {
        bgColorTarget = ofRandomColor();    
    }
}


// ----------------------------------------------------
void TextureScene::draw() {
    
    drawBackground();
    ofEnableAlphaBlending();
    if((int)ofRandom(0, 300) == 30) {
        circleFrcFlip = !circleFrcFlip;
    }
    float n = ofGetElapsedTimef() * 0.2;
    if(circleFrcFlip) n *= -1;
    float r = 5 + ofNoise(n, circleFrc.x/3000.0) * 300;
    circleFrc.x = cos(n*TWO_PI) * r;
    circleFrc.y = sin(n*TWO_PI) * r;
    ofSetColor(255, 0, 0);
    ofCircle(circleFrc+getCentreCubeScreen(), 13);
    
    for (vector<TexturedShape>::iterator it=shapes.begin(); it!=shapes.end(); ++it) {
        
        ofVec2f p = it->getPosition();
        float r = it->getRadius();
        
        ofSetColor(it->colour);
        ofSetColor(255,255,255);
        
        it->tex->bind();
        glBegin(GL_QUADS);
        
        glVertex2f(p.x-r, p.y-r); glTexCoord2f(0, 0);
        glVertex2f(p.x+r, p.y-r); glTexCoord2f(1, 0);
        glVertex2f(p.x+r, p.y+r); glTexCoord2f(1, 1);
        glVertex2f(p.x-r, p.y+r); glTexCoord2f(0, 1);
        
        //ofSphere(p, 10);
        glEnd();
        it->tex->unbind();  
        
        if(!isPointInScreen(p, r)) {
            it->destroy();
            it->dead = true;
        }
    }
    
    ofRemove(shapes, ofxBox2dBaseShape::shouldRemove);
    
    
    ofFill();
    ofSetColor(255, 100);
    for (vector<ofVec2f>::iterator it=pts.begin(); it!=pts.end(); ++it) {
        ofCircle(*it, 2);
    }
    
    for (vector<TriangleShape>::iterator it=tris.begin(); it!=tris.end(); ++it) {
        ofSetColor(255, 10);
        ofNoFill();
        ofTriangle(it->a, it->b, it->c);
        
        // the triangle area
        float radius = GeometryUtils::getTriangleRadius(it->c, it->b, it->a);
        
        // center of the triangle
        ofVec2f center = GeometryUtils::getTriangleCenter(it->a, it->b, it->c);
        ofCircle(center, radius);
        
        /*ofSetColor(255);
         ofDrawBitmapString("a", it->a+10);
         ofDrawBitmapString("b", it->b+10);
         ofDrawBitmapString("c", it->c+10);*/
        ofCircle(center, 3);
        
    }
    
    
    for(int i=0; i<attractors.size(); i++) {
        ofSetColor(255);
        ofCircle(attractors[i].pos, 25);
        ofSetColor(255, 0, 255);
        ofCircle(attractors[i].pos, 5+(20*attractors[i].getAmp()));
    }
    
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(box2d.getBodyCount()), 20, 50);
    ofDrawBitmapString(ofToString(shapes.size()), 20, 90);
    ofDrawBitmapString("r make bigger\ns make smaller", 20, 120);
    gui.draw();
}

void TextureScene::respondToNewComplimentaryColours(){
    for(vector<TexturedShape>::iterator it=shapes.begin(); it!= shapes.end(); ++it) {
        it->colour = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())];
    }    
}

// ----------------------------------------------------
void TextureScene::handleCommands(TwitterCommand& cmd, Effects& fx) {
	// handle commands.
	set<string>::const_iterator it = cmd.tokens.begin();
	while(it != cmd.tokens.end()) {
		const string& c = (*it);
		if(c == "bigger") {
			 for(int i=0; i<MIN(30, shapes.size()-1); i++) {
            	int ranId = (int)ofRandom(0, shapes.size()-1);
            	shapes[ranId].radiusTarget = ofRandom(30, 100);
        	}
		}
		else if(c == "smaller") {
			for(int i=0; i<MIN(30, shapes.size()-1); i++) {
				int ranId = (int)ofRandom(0, shapes.size()-1);
				shapes[ranId].radiusTarget = ofRandom(2, 10);
			}
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