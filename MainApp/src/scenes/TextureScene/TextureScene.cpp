#include "TextureScene.h"


// ----------------------------------------------------
void TextureScene::setup() {
    name = "texture scene";
    index = 0;
    
    ofDisableArbTex();
    for(int i=0; i<3; i++) {
        textures.push_back(ofTexture());
        if(!ofLoadImage(textures.back(), "graphics/text_"+ofToString(i)+".png")) printf("error loading\n");
    }
    // setup box2d
    box2d.init();
    box2d.setGravity(0, 0.2);
    box2d.setFPS(60);
    
    addPoints();
}

// ----------------------------------------------------
void TextureScene::update() {
    box2d.update();
}

// ----------------------------------------------------
void TextureScene::addPoints() {
    pts.clear();
    
    pts.push_back(ofVec2f(0, 0));
    pts.push_back(ofVec2f(CUBE_SCREEN_WIDTH, 0));
    pts.push_back(ofVec2f(CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT));
    pts.push_back(ofVec2f(0, CUBE_SCREEN_HEIGHT));
    
    for(int i=0; i<60; i++) {
        float x = ofRandom(10, CUBE_SCREEN_WIDTH-10);
        float y = ofRandom(10, CUBE_SCREEN_HEIGHT-10);
        pts.push_back(ofVec2f(x, y));
    }   
    
    
    tris = triangulatePolygon(pts);
}

// ----------------------------------------------------
void TextureScene::keyPressed(int key) {
    if(key == ' ') {
        addPoints();
    }
    
    if(key == 'b') {
        
        for (vector<TriangleShape>::iterator it=tris.begin(); it!=tris.end(); ++it) {
            
            float radius = GeometryUtils::getTriangleRadius(it->c, it->b, it->a);
            ofVec2f center = GeometryUtils::getTriangleCenter(it->a, it->b, it->c);
            
            radius = ofRandom(10, 25);
            
            TexturedShape shape;
            shape.setPhysics(1, 0.1, 1);
            shape.setup(box2d.getWorld(), center, radius);
            shape.tex = &textures[(int)ofRandom(0, textures.size()-1)];
            shapes.push_back(shape);
        }        
        
    }
}


// ----------------------------------------------------
void TextureScene::draw() {
    
    BaseScene::draw();
    ofEnableAlphaBlending();
    
    /*
    for (vector<TexturedShape>::iterator it=shapes.begin(); it!=shapes.end(); ++it) {
        it->draw();
    }*/
 
    for (vector<TexturedShape>::iterator it=shapes.begin(); it!=shapes.end(); ++it) {
        
        ofVec2f p = it->getPosition();
        float r = it->getRadius();
        
        it->tex->bind();
        glBegin(GL_QUADS);
        
        glVertex2f(p.x-r, p.y-r); glTexCoord2f(0, 0);
        glVertex2f(p.x+r, p.y-r); glTexCoord2f(1, 0);
        glVertex2f(p.x+r, p.y+r); glTexCoord2f(1, 1);
        glVertex2f(p.x-r, p.y+r); glTexCoord2f(0, 1);
        
        //ofSphere(p, 10);
        glEnd();
        it->tex->unbind();  
        
		/*
		// roxlu 03/08,  isPointInScreen is found, but I get a compiler error 
        if(!isPointInScreen(p, r)) {
            it->destroy();
            it->dead = true;
        }
		*/
        // ofCircle(p, r);
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
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(box2d.getBodyCount()), 20, 50);
    ofDrawBitmapString(ofToString(shapes.size()), 20, 90);
    ofDrawBitmapString("space to make new points\npress b to add shpaes", 20, 100);
}