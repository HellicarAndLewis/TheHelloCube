#pragma once
#include "ofMain.h"

class AudioManager {
    
public:
      
    enum {
      TOP    = 0,
      BOTTOM = 1,
      LEFT   = 2,
      RIGHT  = 3  
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
    static string getSideName(int what) {
        switch (what) {
            case AudioManager::TOP:
                return "TOP";
                break;
            case AudioManager::BOTTOM:
                return "BOTTOM";
                break;
            case AudioManager::LEFT:
                return "LEFT";
                break;
            case AudioManager::RIGHT:
                return "RIGHT";
                break;
        }
    }

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
