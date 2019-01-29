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
#include <Poco/DateTime.h>

class Tweet {
public:
	string userName;
	string tweetText;
	string screenName;
	string mediaID;
	int replies;
	int retweets;
	int likes;
	string date;

	Tweet(string userName, string tweetText, string screenName, string mediaID, int replies, int retweets, int likes, Poco::DateTime &datetime);
};

class ofApp : public ofBaseApp
{
public:

	//declare app functions
	void setup();
	void draw();
	void update();
	void drawTweets(vector<Tweet> temp);
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
	ofxTwitter::FilterQuery filterQuery;
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
	vector<Tweet> liveTweets, archiveTweets, searchResults;
	bool getTweets;

	enum drawStates { LOGIN, HOME };
	enum searchStates { LIVE, ARCHIVE };
	drawStates dstate;
	searchStates sstate;

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