#include "BaseScene.h"


// ----------------------------------------------------
BaseScene::BaseScene() {
    audioPtr = NULL;
}

// ----------------------------------------------------
BaseScene::~BaseScene() {
    
}

// ----------------------------------------------------
void BaseScene::drawBackground() {
    
    bgColor.r += (bgColorTarget.r - bgColor.r) * 0.2;
    bgColor.g += (bgColorTarget.g - bgColor.g) * 0.2;
    bgColor.b += (bgColorTarget.b - bgColor.b) * 0.2;
    bgColor.a = 255;

    ofSetColor(bgColor);
    ofFill();
    ofRect(0, 0, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
}
// ----------------------------------------------------
void BaseScene::draw() {
    ofSetColor(255);
    AppAssets::inst()->appFont.drawString(name, 10, 15);
};