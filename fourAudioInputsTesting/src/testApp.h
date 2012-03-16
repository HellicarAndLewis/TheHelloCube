#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
        void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
				
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

#endif	

