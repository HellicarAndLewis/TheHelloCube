#include "ofMain.h"
#include "App.h"
#include "ofAppGlutWindow.h"

int main() {
    ofAppGlutWindow window;
	
	window.setGlutDisplayString("rgba double samples>=4 depth");
//	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofSetupOpenGL(&window, CUBE_SCREEN_WIDTH + CAMERA_PROJECTION_SCREEN_WIDTH, CAMERA_PROJECTION_SCREEN_HEIGHT, OF_WINDOW);
	ofRunApp( new App());
}
