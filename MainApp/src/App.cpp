#include "App.h"

//--------------------------------------------------------------
void App::setup() {
    
    ofBackground(255);
    ofSetFrameRate(60);
    //ofSetVerticalSync(true);
    
    //initialise camera
	camWidth 		= CAM_WIDTH;	// try to grab at this size. 
	camHeight 		= CAM_HEIGHT;
	
	//vidGrabber.listVideoDevices();											
    vidGrabber.setVideoDeviceID(0); //0 is first, iSight is always last, so this is safe...
	vidGrabber.initGrabber(camWidth, camHeight);  
    
    //now LUTs
    
    dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
	if (dir.size()>0) {
		dirLoadIndex=0;
		loadLUT(dir.getPath(dirLoadIndex));
		doLUT = true;
	}else{
		doLUT = false;
	}
	doLUT = false; // roxlu (drops fps to 11)
	
	lutImg.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
    
    //now fonts
    AppAssets::inst()->appFont.loadFont("fonts/Helvetica.ttf", 12);
    
    // add all the scenes
    scenes.push_back(&cellSC);
    scenes.push_back(&drawnSC);
    scenes.push_back(&textureSC);
    scenes.push_back(&spotsSC);
    
    // setup the scenes
    for(vector<BaseScene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
        (*it)->setup();
        (*it)->exitScene();
        
    }
    
    // start with the frist scene
    sceneIndex = SCENE_DRAWN;
    changeScene(sceneIndex);
    

	twitter.init();
	// fx.setup(CUBE_SCREEN_WIDTH, CUBE_SCREEN_HEIGHT);
    fx_duration = 5.0; // sec
	fx.setup(ofGetWidth(), ofGetHeight()); //like this for now....
	twitter.getSimulator().setEffects(fx);
	command_timeout = ofGetElapsedTimef() + fx_duration;
}

//--------------------------------------------------------------
void App::update() {
    
	float now = ofGetElapsedTimef();
	if(now >= command_timeout) {
		if(twitter.hasNewCommands() && twitter.getNextCommand(command)) {
			string switch_scene;
			if(command.mustSwitchScene(switch_scene)) {
				if(switch_scene == "cell") {
					currentScene = &cellSC;
				}
				else if(switch_scene == "drawn") {
					currentScene = &drawnSC;
				}
				else if(switch_scene == "texture") {
					currentScene = &textureSC;
				}
				else if(switch_scene == "spots") {
					currentScene = &spotsSC;
				}	
			}
			currentScene->handleCommands(command, fx);
		}
		else {
			fx.reset();
		}
		command_timeout = now + fx_duration;
	}

	fx.update();
    twitter.update();
	vidGrabber.update();
	
	if (vidGrabber.isFrameNew()){
        //new frame fun here....
        
        if (doLUT) {
           // applyLUT(vidGrabber.getPixelsRef());
        }
	}    
    
    // later maybe just update the scene that needs to be rendered
    for(vector<BaseScene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
        (*it)->update();
    }
}

//--------------------------------------------------------------
void App::draw() {
	fx.beginGrabPixels();
	
    // we only want to draw the current screen
    // later we will have a transition from 
    // one scene to the next
    if(currentScene != NULL) {
        ofSetColor(255);
        currentScene->draw();
    }

	fx.endGrabPixels();
    fx.draw();
    
    if(doLUT){
        lutImg.draw(CUBE_SCREEN_WIDTH,0,CAMERA_PROJECTION_SCREEN_WIDTH, CAMERA_PROJECTION_SCREEN_HEIGHT);
//		lutImg.draw(lutPos.x, lutPos.y);
//		ofRect(thumbPos.x-3, thumbPos.y-3, 166, 126);
//		vidGrabber.draw(thumbPos.x, thumbPos.y, 160, 120);
//		
//		ofDrawBitmapString(dir.getName(dirLoadIndex), lutPos.x, -lutPos.y+50);
//		
//		ofDrawBitmapString("Use the up and down arrows of your keyboard \nto go through the different filters", lutPos.x, -lutPos.y+100);
		
	}else {
        vidGrabber.draw(CUBE_SCREEN_WIDTH,0,CAMERA_PROJECTION_SCREEN_WIDTH, CAMERA_PROJECTION_SCREEN_HEIGHT); //stick the camera up on screen....
	}
   
	if(twitter.getSimulator().take_screenshot) {	
		rtt::Tweet tweet;
		tweet.setScreenName("roxlutest");
		bool grab = 1; // 0 = screen, 1 = webcam	

		if(grab == 0) {
			ofImage img;
			img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
			twitter.getUploader().uploadScreenshot(
				 (unsigned char*)img.getPixels()
				,img.getWidth()
				,img.getHeight()
				,tweet
			);
		}
		else if(grab == 1) {
			twitter.getUploader().uploadScreenshot(
				 (unsigned char*)vidGrabber.getPixels()
				,vidGrabber.getWidth()
				,vidGrabber.getHeight()
				,tweet
			);
		}
		twitter.getSimulator().take_screenshot = false;
	}

    
    // draw some stats about the app...
    ofEnableAlphaBlending();
    ofFill();
    ofSetColor(255, 130);
    ofRect(0, ofGetHeight()-60, 255, 60);
    ofSetColor(0);
    string info;
    info += ofToString(ofGetFrameRate(), 0)+" fps\n";
    if(currentScene) info += "scene "+currentScene->name+"\n";
    ofDrawBitmapString(info, 10, ofGetHeight()-40);

}

//--------------------------------------------------------------
void App::changeScene(int scene) {
    
    BaseScene * lastScene = scenes[sceneIndex];
    lastScene->exitScene();
    
    sceneIndex = scene;
    currentScene = scenes[scene];
    currentScene->enterScene();
    
}

//--------------------------------------------------------------
void App::keyPressed(int key) {
    
    
    // scene changing
    if(key == OF_KEY_RIGHT) sceneIndex++;
    if(key == OF_KEY_LEFT)  sceneIndex--;
    sceneIndex %= (int)scenes.size();
    if(sceneIndex < 0) sceneIndex = scenes.size()-1;
    
    changeScene(sceneIndex);
    
    if(currentScene!=NULL)currentScene->keyPressed(key);
    
    // for exporting a screen grab
    if(key == 'E') {
        ofSaveScreen("exports/"+ofToString(ofGetUnixTime())+".png");
    }
    
    // fullscreen or not
    if(key == 'f') ofToggleFullscreen();
    
    switch (key) { //JGL should clean this up
		case 'L':
			doLUT^=true;
			break;
		case OF_KEY_UP:
			dirLoadIndex++;
			if (dirLoadIndex>=dir.size()) {
				dirLoadIndex=0;
			}
			loadLUT(dir.getPath(dirLoadIndex));
			
			break;
		case OF_KEY_DOWN:
			dirLoadIndex--;
			if (dirLoadIndex<0) {
				dirLoadIndex=dir.size()-1;
			}
			loadLUT(dir.getPath(dirLoadIndex));
			break;
		default:
			break;
	}  
}

//--------------------------------------------------------------
void App::keyReleased(int key){
    if(currentScene!=NULL)currentScene->keyReleased(key);
}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){
    if(currentScene!=NULL)currentScene->mouseMoved(x, y);
}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){
    if(currentScene!=NULL)currentScene->mouseDragged(x, y, button);

}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){
    if(currentScene!=NULL)currentScene->mousePressed(x, y, button);
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){
    if(currentScene!=NULL)currentScene->mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void App::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void App::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void App::dragEvent(ofDragInfo dragInfo){ 
    
}

//--------------------------------------------------------------
void App::loadLUT(string path){
    LUTloaded=false;

    ofFile file(path);
    string line;
    for(int i = 0; i < 5; i++) {
     getline(file, line);
     ofLog() << "Skipped line: " << line;	
    }
    for(int z=0; z<32; z++){
     for(int y=0; y<32; y++){
         for(int x=0; x<32; x++){
             ofVec3f cur;
             file >> cur.x >> cur.y >> cur.z;
             lut[x][y][z] = cur;
         }
     }
    }

    LUTloaded = true;
}
//--------------------------------------------------------------
void App::applyLUT(ofPixelsRef pix){
    if (LUTloaded) {
     
     for(int y = 0; y < pix.getHeight(); y++){
         for(int x = 0; x < pix.getWidth(); x++){
             
             ofColor color = pix.getColor(x, y);
             
             int lutPos [3];
             for (int m=0; m<3; m++) {
                 lutPos[m] = color[m] / 8;
                 if (lutPos[m]==31) {
                     lutPos[m]=30;
                 }
             }
             
             ofVec3f start = lut[lutPos[0]][lutPos[1]][lutPos[2]];
             ofVec3f end = lut[lutPos[0]+1][lutPos[1]+1][lutPos[2]+1]; 
             
             for (int k=0; k<3; k++) {
                 float amount = (color[k] % 8) / 8.0f;
                 color[k]= (start[k] + amount * (end[k] - start[k])) * 255;
             }
             
             lutImg.setColor(x, y, color);
             
         }			
     }
     
     lutImg.update();
    }
}