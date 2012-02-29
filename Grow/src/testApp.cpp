#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    
    ofBackgroundGradient(ofColor(40, 60, 70), ofColor(10,10,10));
    ofSetFrameRate(60);
    
}

//--------------------------------------------------------------
void testApp::update(){
    p.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor(40, 60, 70), ofColor(10,10,10));
    p.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    p.grow(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}