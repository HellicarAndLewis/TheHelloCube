#include "DrawnScene.h"

// ----------------------------------------------------
void DrawnScene::setup() {
    name = "drawn scene";
    bgColorTarget = ofRandomColor();
    
    
    
	box2d.init();
	box2d.setGravity(10, 0);
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
    
    Viner v;
	v.make(CUBE_SCREEN_WIDTH/2, 10, box2d);
    
    
    vines.push_back(v);
    
}

// ----------------------------------------------------
void DrawnScene::update() {
	box2d.update();	
}



// ----------------------------------------------------
void DrawnScene::draw() {
    
    drawBackground();
    
    for(vector<Viner>::iterator it=vines.begin(); it!=vines.end(); ++it) {
        it->draw();
    }
    
    
}

// ----------------------------------------------------
void DrawnScene::keyPressed(int key) {
    if(key == 'b') {
        bgColorTarget = ofRandomColor();
    }
}