#include "BaseScene.h"


// ----------------------------------------------------
BaseScene::BaseScene() {

}

// ----------------------------------------------------
BaseScene::~BaseScene() {
    
}

// ----------------------------------------------------
void BaseScene::draw() {
    ofSetColor(255);
    AppAssets::inst()->appFont.drawString(name, (ofGetWidth()-AppAssets::inst()->appFont.stringWidth(name))/2, ofGetHeight()/2);
};