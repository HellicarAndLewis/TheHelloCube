#pragma once
#include "ofMain.h"
#include "AppAssets.h"
#include "CellScene.h"
#include "DrawnScene.h"
#include "TextureScene.h"
#include "SpotsScene.h"

class App : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int                 sceneIndex;
    vector <BaseScene*> scenes;
    BaseScene *         currentScene;
    CellScene           cellSC;
    DrawnScene          drawnSC;
    TextureScene        textureSC;
    SpotScene           spotsSC;
    
};
