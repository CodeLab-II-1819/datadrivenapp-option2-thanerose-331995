/*
 Starter code for Data Driven App assignment CodeLab II
 Based on basic example of ofxTwitter by Christopher Baker
 <https://christopherbaker.net>
 SPDX-License-Identifier:    MIT
 */


#pragma once


#include "ofMain.h"
#include "ofxTwitter.h"
#include "ofxGui.h"
#include "ofxTextSuite.h"
#include <vector>
#include <string>


class ofApp: public ofBaseApp
{
public:

    //declare app functions
    void setup();
    void draw();
    void onStatus(const ofxTwitter::Status& status);
    void onError(const ofxTwitter::Error& error);
    void onException(const std::exception& exception);
	void onMessage(const ofJson& json);
    
	//declare twitter search client
    ofxTwitter::SearchClient client;

	stringstream tweetInfo;
	stringstream ss;
	string searchTerm;

	//gui properties
	ofImage background;
	ofImage card;
	ofxPanel panel;

	ofxTextBlock myfont;
	TextBlockAlignment alignment;

	vector<string> tweets;

	int basex = 100;
	int basey = 0;

	//declare variables
	int count = 0;
    int countMissed = 0;



};
