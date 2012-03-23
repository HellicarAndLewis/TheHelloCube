#include "ofMain.h"
#include "App.h"
#include "ofAppGlutWindow.h"

int main() {
    ofAppGlutWindow window;
	
	window.setGlutDisplayString("rgba double samples>=4 depth");

    
#ifdef USE_SMALL_APP
    ofSetupOpenGL(&window, CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT, OF_WINDOW);
#else
	ofSetupOpenGL(&window, CUBE_SCREEN_WIDTH + CAMERA_PROJECTION_SCREEN_WIDTH, CAMERA_PROJECTION_SCREEN_HEIGHT, OF_FULLSCREEN);
#endif
    
    ofRunApp( new App());

}
