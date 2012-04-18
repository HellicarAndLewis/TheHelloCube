#include "DrawnScene.h"

// ----------------------------------------------------
void DrawnScene::setup() {
    name = "drawn scene";
    
    string safeName = name;
    ofStringReplace(safeName, " ", "_");
    gui.setup(name, safeName+".xml", 20, 20);
    
    gui.add(frequency.setup("frequency", 7, 0.0002, 10));
    gui.add(damping.setup("damping", 1, 0.0003, 1.0));
    
    
	// box2d
    box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(30.0);
    
    
    // images
    eyeA.loadImage("graphics/drawn/eye_002.png");
    eyeRatio = (eyeA.getHeight()/eyeA.getWidth());
    
    for(int i=0; i<6; i++) {
        dots.push_back(ofImage());
        dots.back().loadImage("graphics/drawn/dot_"+ofToString(i)+".png");
    }
    
    for(int i=0; i<3; i++) {
        tris.push_back(ofImage());
        tris.back().loadImage("graphics/drawn/triangle_"+ofToString(i)+".png");
    }
    ofDisableArbTex();
    dotRepeatImg.loadImage("graphics/drawn/kusamadot_repeat.png");
    
    /*
    addBush(30);
    addBush(500);    
    addBush(CUBE_SCREEN_WIDTH-300);
    */
    
    // ----------------------------------
    // Field
    // ----------------------------------
    field.setupField(80, 60, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
    field.randomizeField(3);
    field.fadeField(0.8);
    bgColor.setHex(0xE1E3D3);
    
    
    // get all the bushes
    for(int i=0; i<vines.size(); i++) {
        
        if(vines[i].type == Viner::LINER) {
            poopVines.push_back(&vines[i]);
        }
        
    }
    
    
    // make 3 wigglers...
    for(int i=0; i<4; i++) {
        makeWiggler();
    }
    
    
    
    // ----------------------------------
    // the land
    // ----------------------------------
    ofDirectory dir;
    int nLandFiles = dir.listDir("graphics/drawn/land/");
    for(int i=0; i<nLandFiles; i++) {
        land.push_back(LandMass());
        if(ofLoadPolyPoints(land.back(), dir.getPath(i))) {
            printf("land_%i.dat loaded\n", i);
        }
    }  
    
    // get the perps
    for(int i=0; i<land.size(); i++) {
        ofVec2f vec, perp;
        for(int j=0; j<land[i].size(); j++) {
            
            if(land[i][0].x < land[i].getVertices().back().x && land[i][0].y < CUBE_SCREEN_HEIGHT/2) {
                if(j == 0) vec = land[i][j] - land[i][land[i].size()-1];
                else       vec = land[i][j] - land[i][j-1];
            }
            else {
                if(j == 0) vec = land[i][land[i].size()-1] - land[i][j];
                else       vec = land[i][j-1] - land[i][j];
            }
            perp  = vec.perpendicular();
            land[i].perps.push_back(perp);
            land[i].circleVariance.push_back(ofRandom(.5, 1));
        }
        
    }
    
    for(int i=0; i<land.size(); i++) {
        for(int j=0; j<land[i].size(); j++) {
            ofVec2f p = land[i][j]; 
            
            Hair hr;
            hr.landHairIndex[0] = i;
            hr.landHairIndex[1] = j;
            hr.init((int)ofRandom(4, 10));
            hr.dir = land[i].perps[j];   
            landHair.push_back(hr);

        }        
    }
    landHairSep = 4;
    
    
    // ----------------------------------
    // tiny vines on the land
    // ----------------------------------
    for(int i=0; i<10; i++) {
        makeTinyVine();
    }
    
    bBigMode  = true;
    bTinyPoop = true;
    tinyVinesPoopAmt = 30;
    
    
    // ----------------------------------
    // add some land wiggles
    // ----------------------------------
    int nwiggles = 10;
    for(int i=0; i<nwiggles; i++) {
   
        int     nPoly  = ofRandomIndex(land);
        int     ind    = ofRandomIndex(land[nPoly].getVertices());
        float   x      = land[nPoly][ind].x; 
        float   y      = land[nPoly][ind].y; 
        
        Wiggler w;
        w.txt = &dotRepeatImg.getTextureReference();
        w.spikeTex = &tris[ofRandomIndex(tris)].getTextureReference();        
        w.make(x, y, Wiggler::LAND_WIGGLE);
        w.dir = land[nPoly].perps[ind];
        landWiggles.push_back(w);
    }
    
}

// ----------------------------------------------------
void DrawnScene::enterScene() {
    
    generateComplimentaryColours();
    respondToNewComplimentaryColours();
    ofRegisterMouseEvents(&gui);
    
    for (int i=0; i<wigglers.size(); i++) {
        float x = ofRandom(50, CUBE_SCREEN_WIDTH-50);
        float y = CUBE_SCREEN_HEIGHT+10;
        wigglers[i].root.set(x, y);
    }
    
    // get new postion for land wiggles
    for (int i=0; i<landWiggles.size(); i++) {
        int     nPoly  = ofRandomIndex(land);
        int     ind    = ofRandomIndex(land[nPoly].getVertices());
        float   x      = land[nPoly][ind].x; 
        float   y      = land[nPoly][ind].y; 
        landWiggles[i].spikeTex = &tris[ofRandomIndex(tris)].getTextureReference();        
        landWiggles[i].root.set(x, y);
        landWiggles[i].dir = land[nPoly].perps[ind];
    }
    
    for(int i=0; i<land.size(); i++) {
        land[i].texId = ofRandomIndex(dots);
    }
}

// ----------------------------------------------------
void DrawnScene::makeWiggler() {
    
    float x = ofRandom(50, CUBE_SCREEN_WIDTH-50);
    float y = CUBE_SCREEN_HEIGHT+10;
    
    Wiggler w;
    w.txt = &dotRepeatImg.getTextureReference();
    w.make(x, y, Wiggler::NORMAL);
    
    
    wigglers.push_back(w);
    
}

// ----------------------------------------------------
void DrawnScene::makeTinyVine() {
    
    int     nPoly  = ofRandomIndex(land);
    int     ind    = ofRandomIndex(land[nPoly].getVertices());
    float   x      = land[nPoly][ind].x; 
    float   y      = land[nPoly][ind].y; 
    
    int   side  = BaseScene::TOP;// ofRandom(0, 4);
    for(int i=0; i<5; i++) {
        TinyVine w;
        w.side = side;
        w.make(x+ofRandom(-3,3), y, ofRandom(4, 10));
        w.headImg = &dots[ofRandomIndex(dots)];
        w.colorD  = complimentaryColours[ofRandomIndex(complimentaryColours)]; 
        w.dir     = land[nPoly].perps[ind];
        tinyVines.push_back(w);
    }
}

// ----------------------------------------------------
void DrawnScene::addBush(float startX) {
    int n = ofRandom(3, 10);
    int ranDotImg = ofRandomIndex(dots);
    for(int i=0; i<n; i++) {
        Viner v;
        v.type = Viner::LINER;
        v.dotImg = &dots[ranDotImg];
        v.pullForce.set(0, -2);
        v.make(ofRandom(-10, 10)+startX, CUBE_SCREEN_HEIGHT, Viner::BOTTOM, box2d);
        v.colorDes = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]; //choice of random complimentary colour
        
        vines.push_back(v);
        
    }
    
}

// ----------------------------------------------------
void DrawnScene::addChasers(float x, float y) {
    if(chasers.size() < MAX_CHASERS) {
        Chaser p;
        p.radius = ofRandom(20, 50);
        p.setPosition(x, y);
        
        chasers.push_back(p);
    }
}

// ----------------------------------------------------
void DrawnScene::makePoop() {
    if(poop.size() < MAX_POOP) {
        int ranVine  = ofRandomIndex(poopVines);
        if(ranVine != -1 && poopVines.size()>0) {
            Viner * vine = poopVines[ranVine];
            if(vine->type == Viner::LINER) {
                int nPoops = 2;
                for(int i=0; i<nPoops; i++) {
                    
                    
                    ofVec2f pt = vine->circles.back().getPosition();
                    float   r  = (vine->dotImg->getWidth() * vine->dotSize)/2;
                    
                    VinePoop p;
                    p.setPhysics(0.2, 0.2, 0.9);
                    p.setup(box2d.getWorld(), pt.x, pt.y, r);
                    p.img = vine->dotImg;
                    p.dotSizeD = vine->dotSizeD;
                    p.dotSize  = vine->dotSizeD;
                    p.colorD = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())];
                    
                    poop.push_back(p);
                    
                }
                vine->bPop = true;
            }
        }
    }
}

// ----------------------------------------------------
void DrawnScene::makePoopFromTinyVines() {
    if(poop.size() < MAX_POOP) {
        int ranVine  = ofRandomIndex(tinyVines);
        
        TinyVine * vine = &tinyVines[ranVine];
        
        int nPoops = ofRandom(1, 5);
        for(int i=0; i<nPoops; i++) {
            
            
            ofVec2f pt = vine->pts.back();
            float   r  = vine->headSize / 2;
            
            VinePoop p;
            p.setPhysics(0.2, 0.2, 0.9);
            p.setup(box2d.getWorld(), pt.x, pt.y, r);
            p.img = vine->headImg;
            p.dotSizeD = r;
            p.dotSize  = r;
            p.colorD = complimentaryColours[(int)ofRandom(0, complimentaryColours.size())];
            p.setVelocity(ofRandomf(), ofRandom(0.4, 2));// (ofVec2f(ofRandom(-0.3, 0.3), ofRandom(0.1, 1.4)), .004);
            poop.push_back(p);
            
        }
        
    }
}

// ----------------------------------------------------
void DrawnScene::update() {
    
    
    bgColor.setHex(0xE1E3D3);
    ofRectangle screen(0, 0, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
    
    
    // ----------------------
    // Poop 
    // ----------------------
    for(vector<VinePoop>::iterator it=poop.begin(); it!=poop.end(); ++it) {
        
        ofVec2f frc;
        int nFrc = 0;
        ofVec2f pos = it->getPosition();
        ofVec2f vel = it->getVelocity();
        
        float   r   = it->getRadius();
        for(vector<Chaser>::iterator itC=chasers.begin(); itC!=chasers.end(); ++itC) {
            ofVec2f vec = itC->pos - pos;
            if(vec.length() <  r*2) {
                vec.normalize();
                frc += vec;
                ++nFrc;
            }
        }
        if(nFrc > 0) {
            frc /= nFrc;
        }
        it->updateColor();
        if(!it->bGetBig) it->addForce(frc, 1);
        field.addVectorCircle(pos.x, pos.y, vel.x, vel.y, r*2, .1);
        
    }    
    
    
    // ----------------------
    // Vines
    // ----------------------
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        
        for(int i=0; i<it->joints.size(); ++i) {
            it->joints[i].setFrequency(frequency);
            it->joints[i].setDamping(damping);
        }
        
        
        ofVec2f v = getCentreCubeScreen() -  it->circles.back().getPosition();
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
        
        if(it->bPop) {
            
            it->dotSize  = 0;
            it->dotSizeD = ofRandom(.03, 0.2);
            it->dotImg   = &dots[(int)ofRandomIndex(dots)];
            
            it->bPop = false;
            
        }
        
        it->update();
        
    }   
    
    // ----------------------
    // Chasers
    // ----------------------
    float t = ofGetElapsedTimef()*0.02;
    for(vector<Chaser>::iterator it=chasers.begin(); it!=chasers.end(); ++it) {
        
        ofVec2f pos = it->pos;
        float frwX  = ofNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
        
        ofVec2f noiseFrc;
     	noiseFrc.x = ofSignedNoise(t, pos.y * 0.04) * 0.6;
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
        
        
        ofVec2f avoidFrc = 0;
        int nAvoidFrc = 0;
        for(vector<Chaser>::iterator other=it; other!=chasers.end(); ++other) {
            ofVec2f vec = other->pos - it->pos;
            float minDis = 50;
            if(vec.length() < minDis) {
                vec.normalize();
                avoidFrc -= vec; // avoid
                ++nAvoidFrc;
            }
        }    
        if(nAvoidFrc > 0) {
            avoidFrc /= nAvoidFrc;
            avoidFrc *= 1.2;
        }
        
        
        // add forces
        it->frc += noiseFrc;        
        it->frc += vineFrc;
        it->frc += avoidFrc;
        it->frc.limit(3.0);
        
        // applu the forces
		it->vel *= it->drag; 
		it->vel += it->frc;
        it->pos += it->vel;
        
        it->bRemove = !screen.inside(it->pos);
        
        field.addVectorCircle(it->pos.x, it->pos.y, it->vel.x, it->vel.y, 30, .1);
        
        
    }
    ofRemove(chasers, Particle::shouldRemove);
    
    // ----------------------
    // wigglers
    // ----------------------
    for(vector<Wiggler>::iterator it=wigglers.begin(); it!=wigglers.end(); ++it) {
        it->update();
        it->soundAmp = audioPtr->getVolume(0);
    }
    
    // ----------------------
    // land wigglers
    // ----------------------
    for(vector<Wiggler>::iterator it=landWiggles.begin(); it!=landWiggles.end(); ++it) {
        it->update();
        it->soundAmp = audioPtr->getVolume(0);
    }
    
    // Tiny Vines
    // ----------------------
    for(vector<TinyVine>::iterator it=tinyVines.begin(); it!=tinyVines.end(); ++it) {
        it->update();
    }
    
    
    // ----------------------
    box2d.update();	    
    field.fadeField(0.98);
}

// ----------------------------------------------------
void DrawnScene::draw() {
    
    float t = ofGetElapsedTimef() * 0.3;
    drawBackground();
    ofColor peteWhite = ofColor(255,255,255);
    ofRectangle screen(0, 0, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
    ofSetColor(peteWhite);
    ofFill();
    ofRect(screen);
    
    
    // ----------------------
    // vector field
    // ----------------------
    float scalex = (float)field.externalWidth / (float)field.fieldWidth;
    float scaley = (float)field.externalHeight / (float)field.fieldHeight;
    ofSetColor(150);
    for (int i = 0; i < field.fieldWidth; i++){
        for (int j = 0; j < field.fieldHeight; j++){
            
            // pos in array
            int pos = j * field.fieldWidth + i;
            // pos externally
            float px = 	i * scalex;
            float py = 	j * scaley;
            float px2 = px + field.field[pos].x * 5;
            float py2 = py + field.field[pos].y * 5;
            
            ofLine(px,py, px2, py2);
            
            
        }
    }
    
    
    
    // ----------------------
    // land hair
    // ----------------------
    for(int i=0; i<landHair.size(); i++) {
        int     nPoly  = landHair[i].landHairIndex[0];
        int     ind    = landHair[i].landHairIndex[1];
        ofVec2f pt = land[nPoly][ind];
        ofSetColor(90, 0, 0);
        landHair[i].draw(pt, landHairSep);
    }
    
    // ----------------------
    // Tiny Vines
    // ----------------------
    for(vector<TinyVine>::iterator it=tinyVines.begin(); it!=tinyVines.end(); ++it) {
        it->draw();
    }
   
    
    // ----------------------
    // land wigglers
    // ----------------------
    for(vector<Wiggler>::iterator it=landWiggles.begin(); it!=landWiggles.end(); ++it) {
        it->draw();
    }
    
    
    // ----------------------
    // land masses (draw on top of land items)
    // ----------------------
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    int step = 1;
    if(bBigMode) step = 2;
    for(int i=0; i<land.size(); i++) {
        for(int j=0; j<land[i].size(); j+=step) {
            ofSetColor(0);
            float cv = land[i].circleVariance[j] + (ofNoise(t, land[i].circleVariance[j]/ 4.0, land[i][j].x/500.0) * 2);
            float sz = bBigMode?4:2;
            sz *= 2;
            sz += 2;
            dots[land[i].texId].draw(land[i][j], cv + sz, cv + sz);
            //ofCircle(land[i][j], cv + sz);
        }        
    }
    ofPopStyle();
    
    
    // ----------------------
    // wigglers
    // ----------------------
    for(vector<Wiggler>::iterator it=wigglers.begin(); it!=wigglers.end(); ++it) {
        it->draw();
    }
    
   
    
    // ----------------------
    // Chasers
    // ----------------------
    for(vector<Chaser>::iterator it=chasers.begin(); it!=chasers.end(); ++it) {
        
        
        it->nConnections = 0;
        
        if(it->nConnections < 2) {
            for(vector<Chaser>::iterator itB=it; itB!=chasers.end(); ++itB) {
                ofVec2f a = it->pos;
                ofVec2f b = itB->pos;
                if(a.distance(b) < 20) {
                    it->nConnections ++;
                    //ofSetColor(0);
                    ofSetColor(complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]);
                    ofLine(it->pos, itB->pos);
                }
            }
        }
        
        //ofSetColor(0);
        //ofNoFill();
        //it->draw();
        
        float angle = ofRadToDeg( atan2(it->vel.y, it->vel.x) );
        it->rotation += (angle-it->rotation) * 0.3;
        ofEnableAlphaBlending();
        ofSetColor(complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]);
        ofPushMatrix();
        ofTranslate(it->pos);
        ofRotate(it->rotation);
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        eyeA.draw(0, 0, it->radius, it->radius * eyeRatio);
        ofPopStyle();
        ofPopMatrix();
    }
    
    
    // draw the vines
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        ofSetColor(complimentaryColours[(int)ofRandom(0, complimentaryColours.size())]);
        it->drawAsVine();
    }
    
    // ----------------------
    // Poop
    // ----------------------
    for(vector<VinePoop>::iterator it=poop.begin(); it!=poop.end(); ++it) {
        
        float r = it->getRadius() * 2;
        ofVec2f pos = it->getPosition();
        ofEnableAlphaBlending();
        //previously just black
        ofSetColor(it->color, it->alpha);
        ofPushMatrix();
        ofTranslate(pos);
        ofRotate(it->getRotation());
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        it->img->draw(0, 0, r, r);
        ofPopStyle();
        ofPopMatrix();
        
        float age = ofGetElapsedTimef() - it->birthdate;
        if(!screen.inside(pos.x, pos.y)) {
            it->alive  = false;
            it->dead   = true;
        }
        
        if(age > 10 && it->alpha>0){
            it->alpha -= 3.0;
            if(it->alpha <= 0) {
                it->alive  = false;
                it->dead   = true;
            }
        }
    }
    ofRemove(poop, ofxBox2dBaseShape::shouldRemove);
    
    
    // ----------------------
    // sound
    // ----------------------
    float peak = 0.5;
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
        
        
        if(amp > 0.3) {
            int ranTinyVine = ofRandomIndex(tinyVines);
            tinyVines[ranTinyVine].grow();
        }
        
        if(amp>peak) addChasers(pos.x, pos.y);
        
        if(mustDrawGui){
            // draw
            ofSetColor(255);
            ofCircle(pos, 25);
            ofSetColor(255, 0, 255);
            ofCircle(pos, 5+(20*amp));
            ofSetColor(0, 250, 250);
            ofDrawBitmapString(AudioManager::getSideName(i), pos);
        }
    }
    
    
    // ----------------------
    // add some randomly
    // ----------------------
    
    if((int)ofRandom(0, 40)==10) addChasers();
    if((int)ofRandom(0, 4)==2)   makePoop();
    
    if(bTinyPoop) {
        if((int)ofRandom(0, tinyVinesPoopAmt)==0) {
            makePoopFromTinyVines();
        }
    }
    if(bBigMode && poop.size()>10 && (int)ofRandom(0,50)==0) {
        int ranPoop = ofRandomIndex(poop);
        poop[ranPoop].newPopSize = ofRandom(10, 40);
        poop[ranPoop].bGetBig = true;
        
        int ranTiny = ofRandomIndex(tinyVines);
        tinyVines[ranTiny].headSizePctD = ofRandom(0.2, 1.2);
    }
    
    
    
}

// ----------------------------------------------------
void DrawnScene::drawGui() {
    
    if(!mustDrawGui){
		return;
	}
	
	gui.draw();
#ifdef USE_SWIRPS
	ofSetColor(BaseScene::twitterColour);
	particles.draw();
	glColor3f(0,0,0);
	ofDrawBitmapString("Click to create SWIRP\n1 - repel\n2 - atract",10,ofGetHeight()-35);
#endif        
}

// ----------------------------------------------------
void DrawnScene::keyPressed(int key) {
    
    
    if(key == 'q') {
        
        int ranTinyVine = ofRandomIndex(tinyVines);
        tinyVines[ranTinyVine].grow();
        
    }
    
    switch (key) { 
        case 'g': {
            mustDrawGui = !mustDrawGui;
            break;
        }
        case 'c': {
            addChasers();
            break;
        }    
        case ' ': {
            int ranVine = ofRandomIndex(vines);
            vines[ranVine].add(box2d);
            break;
        }
        case 's':{
            // *****************************
            // *** working on this still ***
            // *****************************
            int ranVine  = ofRandomIndex(vines);
            int ranJoint = ofRandomIndex(vines[ranVine].joints);
            
            //vines[ranVine].joints[ranJoint].destroy();
            //vines[ranVine].joints.erase( vines[ranVine].joints.begin() + ranJoint );// + ranJoint);
            break;
        }
        case 'p' : {
            makePoop();
            bTinyPoop = !bTinyPoop;
            break;
        }    
#ifdef USE_SWIRPS
        case '1': {
            repel_effect = 0.3;
            follow_effect = 0.001;
            break;
        }
        case '2': {
            repel_effect = 0.24;
            follow_effect = 0.002;
            break;
        }
#endif		
        default: {
            break;
        } 
    }
    
    
    // for making the land
#ifdef MAKE_LAND
    if(key == 'C') {
        land.clear();
    }
    if(key == 'c') {
        if(land.size()>0) {
            land.back().clear();
        }
    }
    if(key == 's') {
        for(int i=0; i<land.size(); i++) {
            ofPolyline resampled = land[i].getResampledBySpacing(5);
            land[i].clear();
            land[i].addVertexes(resampled.getVertices());
            if(ofSavePolyPoints(land[i], "graphics/drawn/land/land_"+ofToString(i)+".dat")) {
                printf("land_%i.dat saved\n", i);
            }
        }        
    }
    
#endif
    
}

// ----------------------------------------------------
void DrawnScene::mousePressed(int x, int y, int button){
#ifdef USE_SWIRPS
    rxSwirp* sw = new rxSwirp(ofVec3f(x,y,0),particles, (int)ofRandom(5,15));
    particles.addSwirp(sw);
#endif   
    
#ifdef MAKE_LAND
    land.push_back(LandMass());
#endif
    
}

// ----------------------------------------------------
void DrawnScene::mouseMoved(int x, int y ){
#ifdef USE_SWIRPS
    ofVec3f m(x, y, 0);
    particles.follow(m,follow_effect);
    particles.repel(repel_effect);
    particles.update();
#endif	
}

// ----------------------------------------------------
void DrawnScene::mouseDragged(int x, int y, int button) {
    
#ifdef MAKE_LAND
    land.back().addVertex(x, y);
#endif
    
}

// ----------------------------------------------------
void DrawnScene::respondToNewComplimentaryColours(){
    for(int i=0; i<vines.size(); i++) {
        vines[i].colorDes = complimentaryColours[ofRandomIndex(complimentaryColours)];  
    }
    for(int i=0; i<poop.size(); i++) {
        poop[i].colorD = complimentaryColours[ofRandomIndex(complimentaryColours)];  
    }
    for(int i=0; i<tinyVines.size(); i++) {
        tinyVines[i].colorD = complimentaryColours[ofRandomIndex(complimentaryColours)]; 
    }
    for(int i=0; i<wigglers.size(); i++) {
        wigglers[i].colorD = complimentaryColours[ofRandomIndex(complimentaryColours)]; 
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
            tinyVinesPoopAmt = 20;
            bBigMode = true;
            landHairSep = ofRandom(10, 20);
		}
		else if(c == "smaller") {
            tinyVinesPoopAmt = 50;
            bBigMode = false;
            landHairSep = ofRandom(4, 8);
            
            for(int i=0; i<tinyVines.size(); i++) {
               tinyVines[i].headSizePctD = ofRandom(0.1, 0.2);
            }

            
		}
        else if(c=="more") {
            bTinyPoop = true;
        }
        else if(c=="less") {
            bTinyPoop = false;
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