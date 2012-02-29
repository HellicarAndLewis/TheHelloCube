#pragma once
#include "ofMain.h"

class AppAssets {
    
private:
    static AppAssets * instance;
public:
    
    ~AppAssets();
    static AppAssets * inst();
    ofTrueTypeFont     appFont;
};
