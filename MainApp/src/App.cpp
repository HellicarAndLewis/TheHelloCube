#include "App.h"

//--------------------------------------------------------------
void App::setup() {
    
    AppAssets::inst()->appFont.loadFont("fonts/Helvetica.ttf", 30);
    
    scenes.push_back(&cellSC);
    scenes.push_back(&drawnSC);
    scenes.push_back(&textureSC);
    scenes.push_back(&spotsSC);
    
    // setup the scenes
    for(vector<BaseScene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
        (*it)->setup();
    }
    
    // start with the frist scene
    sceneIndex   = 0;
    currentScene = scenes[sceneIndex];
}

//--------------------------------------------------------------
void App::update() {
    
    // later maybe just update the scene that needs to be rendered
    for(vector<BaseScene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
        (*it)->update();
    }
}

//--------------------------------------------------------------
void App::draw(){
    ofBackgroundGradient(ofColor(40, 60, 70), ofColor(10,10,10));
    
    if(currentScene != NULL) {
        ofSetColor(255);
        currentScene->draw();
    }
    
}

//--------------------------------------------------------------
void App::keyPressed(int key) {
    
    if(key == OF_KEY_RIGHT) sceneIndex++;
    if(key == OF_KEY_LEFT)  sceneIndex--;
    sceneIndex %= (int)scenes.size();
    if(sceneIndex < 0) sceneIndex = scenes.size()-1;
    currentScene = scenes[sceneIndex];
    
}

//--------------------------------------------------------------
void App::keyReleased(int key){

}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void App::windowResized(int w, int h){

}

//--------------------------------------------------------------
void App::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void App::dragEvent(ofDragInfo dragInfo){ 

}