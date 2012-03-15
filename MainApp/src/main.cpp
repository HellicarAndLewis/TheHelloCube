#include "ofMain.h"
#include "App.h"
#include "ofAppGlutWindow.h"

int main() {
    ofAppGlutWindow window;
	
//	ofSetupOpenGL(&window, CUBE_SCREEN_WIDTH + CAMERA_PROJECTION_SCREEN_WIDTH, CAMERA_PROJECTION_SCREEN_HEIGHT, OF_WINDOW);
	window.setGlutDisplayString("rgba double samples>=4 depth");
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);// CUBE_SCREEN_WIDTH + CAMERA_PROJECTION_SCREEN_WIDTH, CAMERA_PROJECTION_SCREEN_HEIGHT, OF_WINDOW);

	ofRunApp( new App());
}
