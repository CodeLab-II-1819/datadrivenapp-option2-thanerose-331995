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
#include "ofxJSON.h"
#include <vector>
#include <string>

class Tweet {
public:
	string userName;
	string tweetText;
	string screenName;
	string mediaID;

	Tweet(string userName, string tweetText, string screenName, string mediaID);
};

class ofApp : public ofBaseApp
{
public:

	//declare app functions
	void setup();
	void draw();
	void onStatus(const ofxTwitter::Status& status);
	void onError(const ofxTwitter::Error& error);
	void onException(const std::exception& exception);
	void onMessage(const ofJson& json);
	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	void login(string APICall);

	//declare twitter search client
	ofxTwitter::SearchClient client;
	//ofxTwitter::
	ofxTwitter::User user;
	ofxTwitter::BaseUser baseuser;
	ofxTwitter::Entities entities;
	//json
	ofxJSONElement json;
	string APIKey;
	string APICall;
	bool parseSuccess;

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
	string searchTerm;
	string mediaFilename;
	stringstream loginString;
	stringstream searchstring;


	int basex;
	int basey;
	int tweety;
	bool firstLoad;

	//declare variables
	int xmouse, ymouse;
	int count = 0;
	int countMissed = 0;
};