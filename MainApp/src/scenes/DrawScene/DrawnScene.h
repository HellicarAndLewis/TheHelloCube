//
//  DrawScene.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "BaseScene.h"


class DrawnScene : public BaseScene {
    
private:
protected:
public:
    
    void setup();
	void handleCommands(TwitterCommand& cmd, Effects& fx);    
};