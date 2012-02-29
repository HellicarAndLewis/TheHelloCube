#include "ofMain.h"
#include "App.h"
#include "ofAppGlutWindow.h"

int main() {
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp( new App());
}
