#pragma once
#include "ofMain.h"

class AudioManager {
    
public:
        
    void setup(ofBaseApp * app);
    void update();
    
    void audioIn(float * input, int bufferSize, int nChannels); 
	
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    ofSoundStream soundStream;    
};
