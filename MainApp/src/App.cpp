#include "App.h"

//--------------------------------------------------------------
void App::setup() {
    
 	ofSetCircleResolution(120);
    ofBackground(255);
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
	
	setupEffectsGui();
	    
    //initialise camera
	camWidth 		= CAM_WIDTH;	// try to grab at this size. 
	camHeight 		= CAM_HEIGHT;

#ifndef USE_SMALL_APP
    vidGrabber.setVideoDeviceID(0); //0 is first, iSight is always last, so this is safe...
	vidGrabber.initGrabber(camWidth, camHeight);  
#endif
    
    audioManager.setup(this);
    drawAudio = false;
    
    //now fonts
    AppAssets::inst()->appFont.loadFont("fonts/Helvetica.ttf", 12);
    
    // add all the scenes
    scenes.push_back(&cellSC);
    scenes.push_back(&drawnSC);
    scenes.push_back(&textureSC);
    scenes.push_back(&spotsSC);
    
    // setup the scenes
    for(vector<BaseScene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
        
        // set the audio pointer
        (*it)->audioPtr = &audioManager;
        (*it)->setup();
        (*it)->exitScene();
        
    }
    
    // start with the frist scene
    sceneIndex = SCENE_DRAWN;
    changeScene(sceneIndex);
	
	twitter.setVerbose(true);
	twitter.init();
	fx.setup(ofGetWidth(), ofGetHeight()); 
	take_screenshot_on = 0;
	twitter.getSimulator().setEffects(fx);
	twitter.getSimulator().loadSettings();
	command_timeout = ofGetElapsedTimef() + fx_duration;
    showMouse = false;
    //ofHideCursor();
}

//--------------------------------------------------------------
void App::update() {
	// set center of screen for effects (quickfixes)
	fx.cx = fx_center_x;
	fx.cy = fx_center_y;
	fx.fx_flip_adjust_y = fx_flip_adjust_y;
	fx.fx_love_scale_x = fx_love_scale_x;
	fx.fx_love_scale_y = fx_love_scale_y;
	fx.fx_love_x = fx_love_x;
	fx.fx_love_y = fx_love_y;
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	
	
	float now = ofGetElapsedTimef();
	if(now >= command_timeout) {
		if(twitter.hasNewCommands() && twitter.getNextCommand(command)) {
			string switch_scene;
			if(command.mustSwitchScene(switch_scene)) {
				if(switch_scene == "cell") {
                    changeScene(SCENE_CELL);
				}
				else if(switch_scene == "drawn") {
				    changeScene(SCENE_DRAWN);
                }
				else if(switch_scene == "texture") {
					changeScene(SCENE_TEXTURE);
				}
				else if(switch_scene == "spots") {
                    changeScene(SCENE_SPOTS);
                }	
				else if(switch_scene == "next") {
					++sceneIndex;
					sceneIndex %= (int)scenes.size();
					changeScene(sceneIndex);
				}
				else if(switch_scene == "previous") {
					--sceneIndex;
					if(sceneIndex < 0) {
						sceneIndex = scenes.size()-1;
					}
					changeScene(sceneIndex);
				}
				else if(switch_scene == "random") {
					unsigned int rnd_dx = rand() % scenes.size();
					int max = 100;
					do {
						rnd_dx = rand() % scenes.size();
						--max;
					} while(rnd_dx == sceneIndex && max > 0);
					changeScene(rnd_dx);
				}
			}
			currentScene->handleCommands(command, fx);
			
			if(!command.isFake()) {
				twitter.getSimulator().take_screenshot = true;
				take_screenshot_on = ofGetElapsedTimeMillis() + delay_between_fx_and_screenshot;
			}
		}
		else if(fx_reset_automatically) {
			fx.reset();
		}
		command_timeout = now + (float)fx_duration;
	}

	fx.update();
    twitter.update();
#ifndef USE_SMALL_APP
    vidGrabber.update();
#endif
    audioManager.update();
	    
    // later maybe just update the scene that needs to be rendered
    for(vector<BaseScene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
        (*it)->update();
    }
}

//--------------------------------------------------------------
void App::draw(){

	//ofEnableSmoothing();
	fx.beginGrabPixels();
	
    // we only want to draw the current screen
    // later we will have a transition from 
    // one scene to the next
    if(currentScene != NULL) {
        ofSetColor(255);
		currentScene->draw();
    }


#ifdef USE_VEINS
	ofBackground(33);
	veins.draw();
#endif
		
	fx.endGrabPixels();
	fx.draw();
	
	if(currentScene != NULL) {
	 	currentScene->drawGui();
	}
	
    ofSetColor(255);
    
#ifndef USE_SMALL_APP
	vidGrabber.draw(CUBE_SCREEN_WIDTH,0,CAMERA_PROJECTION_SCREEN_WIDTH, CAMERA_PROJECTION_SCREEN_HEIGHT); //stick the camera up on screen....
#endif
    
    if(drawAudio){
        audioManager.draw();
    }
   
	int now = ofGetElapsedTimeMillis();
	if(twitter.getSimulator().take_screenshot && now > take_screenshot_on) {	
		
		rtt::Tweet tweet;
		tweet = command.tweet;
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
		take_screenshot_on = 0;
		twitter.getSimulator().take_screenshot = false;
	}
		
    if(currentScene != NULL && currentScene->mustDrawGui) {
		gui.draw();
        
        // draw some stats about the app...
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor(255, 130);
        ofRect(0, ofGetHeight()-90, 255, 90);
        ofSetColor(0);
        string info;
        info += ofToString(ofGetFrameRate(), 0)+" fps\n";
        if(currentScene) info += "scene "+currentScene->name+"\n";
        ofDrawBitmapString(info, 10, ofGetHeight()-70);
	}
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
    switch (key) { 
        case 'a': 
			drawAudio = !drawAudio;
			break; 
        case OF_KEY_RIGHT:  //scene changing
			sceneIndex++;
			draw_gui = true;
			break;              
        case OF_KEY_LEFT:
			sceneIndex--;
			draw_gui = true;
			break;              
        case 'e': // for exporting a screen grab
			ofSaveScreen("exports/"+ofToString(ofGetUnixTime())+".png");
			break;            
        case 'f':
			ofToggleFullscreen();
			break;
//		case 'g': {
			//draw_gui = !draw_gui;
//			break;
//		}
        case 'm':{
            if(showMouse){
                ofHideCursor();
            }else{
                ofShowCursor();
            }
            showMouse = !showMouse;
            break;
        }
		
		
#ifdef USE_VEINS		
		case 'v': {
			float cx = ofGetWidth() * 0.5;
			float cy = ofGetHeight() * 0.5;
			for(int i = 0; i < 300; ++i) {
				float l = ofRandom(0,300);
				float a = ofRandom(0,TWO_PI);
				float x = cx + cos(a) * l;
				float y = cy + sin(a) * l;
				rxParticle* p = new rxParticle(ofVec3f(x,y,0),1);
				veins.addSource(p);
			}
			break;
		}
		
		case 'o': {
			float cx = ofGetWidth() * 0.5;
			float cy = ofGetHeight() * 0.5;
			float l = 300;
			float a = ofRandom(0,TWO_PI);
			float x = cx + cos(a) * l;
			float y = cy + sin(a) * l;
			rxParticle* p = new rxParticle(ofVec3f(x,y,0),1);
			veins.addRoot(p);
			break;			
		}
		case 's': {
			veins.step();
			break;
		}
		case 'u': {
			veins.update();
			break;
		}
#endif		
		default:
			break;
	}  
    
    sceneIndex %= (int)scenes.size();
    if(sceneIndex < 0) sceneIndex = scenes.size()-1;
    
    changeScene(sceneIndex);
    
    if(currentScene!=NULL)currentScene->keyPressed(key);
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
void App::audioIn(float * input, int bufferSize, int nChannels){
    audioManager.audioIn(input, bufferSize, nChannels);
    
}

void App::exit(){
    audioManager.exit();
}



// E F F E C T S    S E T T I N G S 
//------------------------------------------------------------------------------
void App::onGuiUpdateSettings(bool& on) {
	if(on) {
		fx.pixelate_x = fx_pixelate_x;
		fx.pixelate_y= fx_pixelate_y;
		fx.wave_displace = fx_wave_displace;
		fx.wave_num = fx_wave_num;
		fx.wave_speed = fx_wave_speed;
		fx.shake_number = fx_shake_number;
		fx.shake_amplitude = fx_shake_amplitude;
		fx.shake_duration = fx_shake_duration;
		fx.swirl_radius = fx_swirl_radius;
		fx.swirl_angle = fx_swirl_angle;
	}
}

void App::setupEffectsGui() {
	int gui_w = 500;
	draw_gui = false;
	gui.setSize(gui_w, 200);
	gui.setup("app", "app.xml", CUBE_SCREEN_WIDTH-(gui.getWidth()+20), 20);
	gui.add(fx_duration.setup("Duration of effect (sec)", 20, 1, 500,gui_w));
	gui.add(delay_between_fx_and_screenshot.setup("Delay between fx and screengrab (millis)", 1000, 0, 5000,gui_w));
 	gui.add(fx_pixelate_x.setup("FX: Pixelate X",10,1,50,gui_w));
	gui.add(fx_pixelate_y.setup("FX: Pixelate Y",10,1,50,gui_w));
	gui.add(fx_wave_displace.setup("FX: Wave displace", 0.01, 0,1, gui_w));
	gui.add(fx_wave_speed.setup("FX: Wave speed",1.4, 1, 5, gui_w));
	gui.add(fx_wave_num.setup("FX: Wave amount", 14, 0, 32, gui_w));
	gui.add(fx_shake_number.setup("FX: Shake amount", 16,0,32,gui_w));
	gui.add(fx_shake_amplitude.setup("FX: Shake amplitude",0.05,0,1,gui_w));	
	gui.add(fx_shake_duration.setup("FX: Shake duration (sec)",14,0,25,gui_w));	
	gui.add(fx_swirl_radius.setup("FX: Swirl radius",0.23,0,1,gui_w));		
	gui.add(fx_swirl_angle.setup("FX: Swirl angle",5.6,0,TWO_PI	,gui_w));		
	gui.add(fx_love_scale_x.setup("FX: Heart scale X",0.8,-20,20, gui_w));		
	gui.add(fx_love_scale_y.setup("FX: Heart scale Y",0.8,-20,20, gui_w));		
	gui.add(fx_love_x.setup("FX: Heart x",0.0,-4,4, gui_w));		
	gui.add(fx_love_y.setup("FX: Heart y",0.0,-4,4, gui_w));		
	gui.add(fx_center_x.setup("Center X",0.5,0,0.55,gui_w));		
	gui.add(fx_center_y.setup("Center Y",0.5,0,0.7,gui_w));		
	gui.add(fx_flip_adjust_y.setup("Flip adjust Y",0.0,-0.5,0.5,gui_w));		
	gui.add(fx_reset_automatically.setup("Reset effects automatically",false,gui_w));
	gui.add(fx_test_ripple.setup("Ripple", gui_w));
	gui.add(fx_test_shake.setup("Shake", gui_w));
	gui.add(fx_toggle_pixelate.setup("Pixelate", false, gui_w));
	gui.add(fx_toggle_swirl.setup("Swirl", false,  gui_w));
	gui.add(fx_toggle_wave.setup("Wave", false, gui_w));
	gui.add(fx_toggle_mirror.setup("Mirror", false, gui_w));
	gui.add(fx_toggle_reflect.setup("Reflect", false, gui_w));
	gui.add(fx_toggle_invert.setup("Invert", false, gui_w));
	gui.add(fx_toggle_posterize.setup("Posterize", false, gui_w));
	gui.add(fx_toggle_flip.setup("Flip", false, gui_w));
	gui.add(fx_crack.setup("Crack", false, gui_w));
	gui.add(fx_heart.setup("Heart", false, gui_w));
	
	fx_test_shake.addListener(this, &App::onGuiTestShake);
	fx_test_ripple.addListener(this, &App::onGuiTestRipple);
	fx_toggle_pixelate.addListener(this, &App::onGuiTogglePixelate);
	fx_pixelate_x.addListener(this,&App::onGuiPixelateX);
	fx_pixelate_y.addListener(this,&App::onGuiPixelateY);
	fx_toggle_wave.addListener(this, &App::onGuiToggleWave);
	fx_wave_displace.addListener(this, &App::onGuiWaveDisplace);
	fx_wave_speed.addListener(this, &App::onGuiWaveSpeed);
	fx_wave_num.addListener(this, &App::onGuiWaveNum);
	fx_toggle_swirl.addListener(this, &App::onGuiToggleSwirl);
	fx_swirl_radius.addListener(this, &App::onGuiSwirlRadius);
	fx_swirl_angle.addListener(this, &App::onGuiSwirlAngle);
	fx_toggle_mirror.addListener(this, &App::onGuiMirror);
	fx_toggle_reflect.addListener(this, &App::onGuiReflect);
	fx_toggle_invert.addListener(this, &App::onGuiInvert);
	fx_toggle_posterize.addListener(this, &App::onGuiPosterize);
	fx_toggle_flip.addListener(this, &App::onGuiFlip);
	fx_crack.addListener(this, &App::onGuiCrack);
	fx_heart.addListener(this, &App::onGuiHeart);

	
	gui.loadFromFile("app.xml");
	bool b = true;
	onGuiUpdateSettings(b);
}

void App::onGuiCrack(bool& on) {
	fx.crack(on);
}

void App::onGuiTestShake(bool& on) {
	onGuiUpdateSettings(on);
	if(on) {
		fx.applyEffect("shake");
	}
}

void App::onGuiPixelateX(float& v) {
	bool b = true;
	onGuiUpdateSettings(b);
	fx.pixelate(fx_toggle_pixelate, v, fx.pixelate_y);
}

void App::onGuiPixelateY(float& v) {
	bool b = true;
	onGuiUpdateSettings(b);
	fx.pixelate(fx_toggle_pixelate, fx.pixelate_x, v);
}		

void App::onGuiTogglePixelate(bool& on) {
	bool b = true;
	onGuiUpdateSettings(b);
	fx.pixelate(on, fx.pixelate_x, fx.pixelate_y);
}

void App::onGuiWaveSpeed(float& v) {
	bool b = true;
	onGuiUpdateSettings(b);
	fx.wave(fx_toggle_wave, fx.wave_speed,fx.wave_displace, fx.wave_num);
}

void App::onGuiWaveNum(float& v) {
	bool b = true;
	onGuiUpdateSettings(b);
	fx.wave(fx_toggle_wave, fx.wave_speed,fx.wave_displace, v);
}

void App::onGuiWaveDisplace(float& v) {
	bool b = true;
	onGuiUpdateSettings(b);
	fx.wave(fx_toggle_wave, fx.wave_speed,fx.wave_displace, fx.wave_num);
}

void App::onGuiToggleWave(bool& on) {
	onGuiUpdateSettings(on);
	fx.wave(on, fx.wave_speed,fx.wave_displace, fx.wave_num);
}

void App::onGuiToggleSwirl(bool& on) {
	onGuiUpdateSettings(on);
	fx.swirl(on, fx.swirl_radius, fx.swirl_angle);
}
	
void App::onGuiSwirlRadius(float& v) {
	bool b = true;
	onGuiUpdateSettings(b);
	fx.swirl(fx_toggle_swirl, fx.swirl_radius, fx.swirl_angle);
}

void App::onGuiSwirlAngle(float& v) {
	bool b = true;
	onGuiUpdateSettings(b);
	fx.swirl(fx_toggle_swirl, fx.swirl_radius, fx.swirl_angle);
}
	
void App::onGuiTestRipple(bool& on) {
	onGuiUpdateSettings(on);
	if(on) {
		fx.applyEffect("ripple");
	}
}

void App::onGuiInvert(bool& on) {
	fx.invert(on);
}

void App::onGuiMirror(bool& on) {	
	fx.mirror(on);
}

void App::onGuiReflect(bool& on) {
	fx.reflect(on);
}

void App::onGuiHeart(bool& on) {
	fx.love(on);
}

void App::onGuiPosterize(bool& on) {
	fx.posterize(on);
}

void App::onGuiFlip(bool& on) {
	fx.flip(on);
}

