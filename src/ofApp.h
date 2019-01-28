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

class Tweet {
public:
	string userName;
	string tweetText;
	ofxTwitter::Entities entities;
	string screenName;

	Tweet(string userName, string tweetText, ofxTwitter::Entities entities, string screenName);
};

class ofApp : public ofBaseApp
{
public:

	//declare app functions
	void setup();
	void draw();
	void onStatus(const ofxTwitter::Status& status, ofxTwitter::Entities& ent);
	void onError(const ofxTwitter::Error& error);
	void onException(const std::exception& exception);
	void onMessage(const ofJson& json);
	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);

	//declare twitter search client
	ofxTwitter::SearchClient client;
	ofxTwitter::User user;
	ofxTwitter::BaseUser baseuser;
	ofxTwitter::Entities entities;

	//GUI
	ofxPanel sidebar;
	ofRectangle idCheck, searchbar, searchbtn;

	ofxTextBlock myfont;
	TextBlockAlignment alignment;

	//vector
	vector<Tweet> tweets;

	enum states { LOGIN, HOME };
	states state;

	//stringstreams
	stringstream login;
	stringstream searchstring;

	int basex = 100;
	int basey = 100;

	//declare variables
	int xmouse, ymouse;
	int count = 0;
	int countMissed = 0;
};
