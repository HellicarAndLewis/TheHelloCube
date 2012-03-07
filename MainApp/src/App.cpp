#include "App.h"

//--------------------------------------------------------------
void App::setup() {
    
    ofBackground(255);
    ofSetVerticalSync(true);
    
    AppAssets::inst()->appFont.loadFont("fonts/Helvetica.ttf", 12);
    
    // add all the scenes
    scenes.push_back(&cellSC);
    scenes.push_back(&drawnSC);
    scenes.push_back(&textureSC);
    scenes.push_back(&spotsSC);
    
    // setup the scenes
    for(vector<BaseScene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
        (*it)->setup();
    }
    
    // start with the frist scene
    sceneIndex   = SCENE_CELL;
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
void App::draw() {

    ofBackgroundGradient(ofColor(40, 60, 70), ofColor(10,10,10));
    
    if(bExportPDF) {
        ofBeginSaveScreenAsPDF("exports/"+ofToString(ofGetUnixTime())+".pdf");
    }
    
    // we only want to draw the current screen
    // later we will have a transition from 
    // one scene to the next
    if(currentScene != NULL) {
        ofSetColor(255);
        currentScene->draw();
    }
    
    if(bExportPDF) {
        bExportPDF = false;
        ofEndSaveScreenAsPDF();
    }
    
}

//--------------------------------------------------------------
void App::keyPressed(int key) {
    
    
    // scene changing
    if(key == OF_KEY_RIGHT) sceneIndex++;
    if(key == OF_KEY_LEFT)  sceneIndex--;
    sceneIndex %= (int)scenes.size();
    if(sceneIndex < 0) sceneIndex = scenes.size()-1;
    currentScene = scenes[sceneIndex];
    if(currentScene!=NULL)currentScene->keyPressed(key);
    
    
    // for exporting a screen grab
    if(key == 'E') {
        ofSaveScreen("exports/"+ofToString(ofGetUnixTime())+".png");
    }
    if(key == 'P') {
        bExportPDF = true;
    }
    
    // fullscreen or not
    if(key == 'f') ofToggleFullscreen();
}

//--------------------------------------------------------------
void App::keyReleased(int key){
    if(currentScene!=NULL)currentScene->keyReleased(key);
}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){
    if(currentScene!=NULL)currentScene->mouseMoved(x, y);
}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){
    if(currentScene!=NULL)currentScene->mouseDragged(x, y, button);
    
    if(textureSC.index!=-1) {
        textureSC.pts[textureSC.index].set(x, y);
    }
}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){
    if(currentScene!=NULL)currentScene->mousePressed(x, y, button);
    
    
    for (int i=0; i<textureSC.pts.size(); i++) {
        if(ofVec2f(x,y).distance(textureSC.pts[i]) < 10) {
            textureSC.index = i;
        }
    }
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){
    if(currentScene!=NULL)currentScene->mouseReleased(x, y, button);
    textureSC.index = -1;
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