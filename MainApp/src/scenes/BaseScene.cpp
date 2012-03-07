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
    AppAssets::inst()->appFont.drawString(name, 10, 15);
};