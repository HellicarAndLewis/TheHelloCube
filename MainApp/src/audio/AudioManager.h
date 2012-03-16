#pragma once
#include "ofMain.h"

class AudioManager {
    
public:
        
    void setup(ofBaseApp * app);
    void update();
    void draw();
    void exit();
    void audioIn(float * input, int bufferSize, int nChannels); 
	
    vector <float> left;
    vector <float> right;
    vector <float> top;
    vector <float> bottom;
    
    ofSoundStream soundStream;
    
    //from Marek Project Donk Audio Server...
    
    // locking stuff
    ofMutex audioMutex;
    float audioFps;
    float audioPos;
    float lastTimeSent;
    float smoothing;
    float exponent;
    
    int bufferSize;
    int bufferCounter;
    int drawCounter;
    
    bool mAudioPresent;  
};
