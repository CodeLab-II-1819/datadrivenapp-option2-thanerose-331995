/*
 Starter code for Data Driven App assignment CodeLab II
 Based on basic example of ofxTwitter by Christopher Baker
 <https://christopherbaker.net>
 SPDX-License-Identifier:    MIT
*/


#include "ofApp.h"


int main()
{
    //create small GUI window
    ofSetupOpenGL(2560, 2000, OF_WINDOW);
    //run the app from the ofApp class
    return ofRunApp(std::make_shared<ofApp>());
}
