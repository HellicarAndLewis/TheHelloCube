#pragma once
#include "ofMain.h"

class AudioManager {
    
public:
      
    enum {
      TL = 0,
      TR = 1,
      BL = 2,
      BR = 3  
    };
        
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
    
    float getVolume(int side);
    
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
