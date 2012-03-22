#include "BaseScene.h"

ofColor BaseScene::twitterColour = ofColor(255,0,0);

// ----------------------------------------------------
BaseScene::BaseScene() {
    audioPtr = NULL;
    generateComplimentaryColours();
	mustDrawGui = true;
}

// ----------------------------------------------------
BaseScene::~BaseScene() {
    
}

// ----------------------------------------------------
void BaseScene::drawBackground() {
    ofSetColor(ofColor::black);
    ofFill();
    ofRect(0, 0, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
}
// ----------------------------------------------------
void BaseScene::draw() {
    ofSetColor(255);
    AppAssets::inst()->appFont.drawString(name, 10, 15);
};

void BaseScene::generateComplimentaryColours(){
    cout << "Making new complimentary colours " << endl;
    
    //twitterColour is the value that we are going to work with
    float hue, saturation, brightness;
    twitterColour.getHsb( hue, saturation, brightness );
    
    //lets clear out any old colours 
    complimentaryColours.clear();
    
    size_t numberOfComplimentaryColours = 16;
    
    complimentaryColours.resize(numberOfComplimentaryColours);
    
    //first pass at a colour generator
    
    for(int i = 0; i < complimentaryColours.size(); i++){
        float newHue = hue;//fmodf(hue + ofRandom(0, 12.f), 255.f);
        float newSaturation = saturation;//fmodf(saturation + ofRandom(0.f, 255.f), 255.f);
        float newBrightness = fmodf(brightness + ofRandom(0.f, 128.f), 255.f);
        
        if(i==0){
            newHue = hue;
            newSaturation = saturation;
            newBrightness = brightness;            
        }
        
        complimentaryColours[i] = ofColor::fromHsb(newHue, newSaturation, newBrightness);
    }
};

void BaseScene::drawGui() {
	if(mustDrawGui) {
		gui.draw();
	}
}
