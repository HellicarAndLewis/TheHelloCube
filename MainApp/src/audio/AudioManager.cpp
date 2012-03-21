#include "AudioManager.h"
#include "AppAssets.h"


float volumes[NUM_AUDIO_CHANNELS];
float meters[NUM_AUDIO_CHANNELS];
float gains[NUM_AUDIO_CHANNELS];
float thruBuffer[NUM_AUDIO_CHANNELS][4096];

void AudioManager::exit() {
	soundStream.close();
}

void AudioManager::setup(ofBaseApp * app){
	soundStream.listDevices();
	
	bufferSize = 256;
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
    top.assign(bufferSize, 0.0);
    bottom.assign(bufferSize, 0.0);
    
    /*device = 4 (M-Audio, Inc.: M-Audio Fast Track Ultra)
     ----* default ----* 
     maximum output channels = 8
     maximum input channels = 8*/
    
    //    // 0 output channels, 
    //	// 2 input channels
    //	// 44100 samples per second
    //	// 256 samples per buffer
    //	// 4 num buffers (latency)
    //	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    // 8 output channels, 
	// 8 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 1 num buffers (latency)
    mAudioPresent = true; //need to do this sensibly
#ifdef USE_SMALL_APP
    mAudioPresent = false;
#endif
    
    
	if(mAudioPresent) {
        //if you want to set a different device id 
        soundStream.setDeviceID(4); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
		soundStream.setup(app, 8, 8, 44100, 256, 1);
	} else {
        soundStream.setDeviceID(0); 
		soundStream.setup(app, 0, 2, 44100, 256, 1);
	}
    
    bufferCounter = 0;
    drawCounter = 0;
}

float AudioManager::getVolume(int side) {
#ifdef USE_MOUSE_HACK
    ofVec2f pos;
    // TOP
    if(side == AudioManager::TOP) {
        pos.set(CUBE_SCREEN_WIDTH/2, 20);
    }
    
    // BOTTOM
    if(side == AudioManager::BOTTOM) {
        pos.set(CUBE_SCREEN_WIDTH/2, CUBE_SCREEN_HEIGHT-20);
    }
    
    // LEFT
    if(side == AudioManager::LEFT) {
        pos.set(20, CUBE_SCREEN_HEIGHT/2);
    }
    
    // RIGHT
    if(side == AudioManager::RIGHT) {
        pos.set(CUBE_SCREEN_WIDTH-20, CUBE_SCREEN_HEIGHT/2);
    }    
    ofVec2f mouseVec = pos - ofGetMouse();
    float damp = 1.0 - ofClamp(mouseVec.length() / CUBE_SCREEN_WIDTH, 0.0, 1.0);
#endif
    
    float amp = 0;
    
    switch (side) {
        
        case LEFT:
            amp = volumes[0];
            break;
            
        case TOP:
        {
            if(mAudioPresent) {
                amp = volumes[2];
            }
            else {
                amp = volumes[0];
            }
        }
            break;
            
        case BOTTOM:
        {
            if(mAudioPresent) {
                amp = volumes[3];
            }
            else {
                amp = volumes[1];
            }
        }
            break;
        case RIGHT:
            amp = volumes[1];
            break;
    }
    
#ifdef USE_MOUSE_HACK
    return amp * damp;
#else 
    return amp;
#endif
    
}



void AudioManager::update(){
    //	//lets scale the vol up to a 0-1 range 
    //	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    //
    //	//lets record the volume into an array
    //	volHistory.push_back( scaledVol );
    //	
    //	//if we are bigger the the size we want to record - lets drop the oldest value
    //	if( volHistory.size() >= 400 ){
    //		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    //	}
    
    //always have left and right
    left.push_back(volumes[0]);
    right.push_back(volumes[1]);
    
    //if we have mAudio then we have the top and bottom too
    if(mAudioPresent){
        top.push_back(volumes[2]);
        bottom.push_back(volumes[3]);
    }else{ //else lets just duplicate the values going into left/right
        top.push_back(volumes[0]);
        bottom.push_back(volumes[1]);        
    }
    
    //    left.push_back(ofMap(volumes[0], 0.0, 0.17, 0.0, 1.0, true));
    //    right.push_back(ofMap(volumes[1], 0.0, 0.17, 0.0, 1.0, true));
    //    top.push_back(ofMap(volumes[2], 0.0, 0.17, 0.0, 1.0, true));
    //    bottom.push_back(ofMap(volumes[3], 0.0, 0.17, 0.0, 1.0, true));
    
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( left.size() >= bufferSize ){
		left.erase(left.begin(), left.begin()+1);
	}
    if( right.size() >= bufferSize ){
		right.erase(right.begin(), right.begin()+1);
	}
    if( top.size() >= bufferSize ){
		top.erase(top.begin(), top.begin()+1);
	}
    if( bottom.size() >= bufferSize ){
		bottom.erase(bottom.begin(), bottom.begin()+1);
	}  
}

//--------------------------------------------------------------
void AudioManager::draw(){
	
	ofSetColor(225);
	ofDrawBitmapString("AUDIO Inputs", 32, 32);
	
	ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Left Channel", 4, 18);
    
    ofSetLineWidth(1);	
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (int i = 0; i < left.size(); i++){
        ofVertex(i*2, 100 -left[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
    
	// draw the right channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 370, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Right Channel", 4, 18);
    
    ofSetLineWidth(1);	
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (int i = 0; i < right.size(); i++){
        ofVertex(i*2, 100 -right[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
    
	// draw the top channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32 + (bufferSize*2), 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Top Channel", 4, 18);
    
    ofSetLineWidth(1);	
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (int i = 0; i < top.size(); i++){
        ofVertex(i*2, 100 -top[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
    
	// draw the bottom channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32 + (bufferSize*2), 370, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Bottom Channel", 4, 18);
    
    ofSetLineWidth(1);	
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (int i = 0; i < bottom.size(); i++){
        ofVertex(i*2, 100 -bottom[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();    
	
	drawCounter++;
	
	ofSetColor(225);
	string reportString = "buffers received: "+ofToString(bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\nticks: " + ofToString(soundStream.getTickCount());
	ofDrawBitmapString(reportString, 32, 589);
    
}

//--------------------------------------------------------------
void AudioManager::audioIn(float * input, int bufferSize, int nChannels){	
	
    if(mAudioPresent){
        // clear out the thru buffer because we don't know if we're
        // going to use every channel yet.
        for(int i = 0; i < NUM_AUDIO_CHANNELS; i++) {
            memset(thruBuffer[i], 0, 4096*sizeof(float));
        }
        
        for(int channel = 0; channel < NUM_AUDIO_CHANNELS && channel < nChannels; channel++) {
            for(int i = 0; i < bufferSize; i++) {
                float absSignal;
                absSignal = ABS(input[i*nChannels+channel]);
                thruBuffer[channel][i] = input[i*nChannels+channel]*gains[channel];
                if(absSignal>volumes[channel]) {
                    volumes[channel] = absSignal;
                } else {
                    volumes[channel] *= smoothing;
                }
            }
            // clip 
            meters[channel] = MIN(volumes[channel]*gains[channel], 1);
            meters[channel] = pow(meters[channel], exponent);
        }        
    }else{
        // clear out the thru buffer because we don't know if we're
        // going to use every channel yet.
        for(int i = 0; i < NUM_AUDIO_CHANNELS; i++) {
            memset(thruBuffer[i], 0, 4096*sizeof(float));
        }
        
        for(int channel = 0; channel < NUM_AUDIO_CHANNELS && channel < nChannels; channel++) {
            for(int i = 0; i < bufferSize; i++) {
                float absSignal;
                absSignal = ABS(input[i*nChannels+channel]);
                thruBuffer[channel][i] = input[i*nChannels+channel]*gains[channel];
                if(absSignal>volumes[channel]) {
                    volumes[channel] = absSignal;
                } else {
                    volumes[channel] *= smoothing;
                }
            }
            // clip 
            meters[channel] = MIN(volumes[channel]*gains[channel], 1);
            meters[channel] = pow(meters[channel], exponent);
        }
    }
    
	audioMutex.lock();
	float framePeriod = 1.f/audioFps;
	audioMutex.unlock();
    
    bufferCounter++;
}