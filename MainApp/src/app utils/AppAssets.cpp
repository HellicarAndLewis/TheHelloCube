#include "AppAssets.h"

AppAssets * AppAssets::instance = NULL;

// ----------------------------------------------------
AppAssets * AppAssets::inst() {
    if(instance == NULL) {
        instance = new AppAssets();
    }
    return instance;
}

// ----------------------------------------------------
AppAssets::~AppAssets() {
    if(instance) {
        delete instance;
        instance = NULL;
    }
}