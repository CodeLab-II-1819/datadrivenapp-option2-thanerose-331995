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
	void search(string thisSearch);
	void getfiledata();
	void savefiledata();

	//declare twitter search client
	ofxTwitter::SearchClient client;
	ofxTwitter::Entities entities;

	//GUI
	ofRectangle searchbar, searchbtn, livebtn, archivebtn, popularbtn, recentbtn, mixedbtn, hashtagsearch, addhashtag, clearhash, location1, location2, location3, location4, mediacheck, exit;
	ofImage paper, background, card, like, rt, rply, polaroid, photo, note, add, map, exiticon, coffee, pens, office;

	ofxTextBlock myfont;
	TextBlockAlignment alignment;

	//vectors
	vector<Tweet> liveTweets, archiveTweets, searchResults;
	vector<string> hashtags;
	
	enum searchStates { LIVE, ARCHIVE };
	searchStates sstate;

	//stringstreams
	string lastSearch, mediaFilename;
	stringstream searchstring, hashtagsearchstr, allhashtags, shiftstring;

	int basex, basey, tweety, location;
	bool getTweets, ispopular, ismixed, isrecent, process, hashtagclicked, checklocation, mediaonly;

	//declare variables
	int xmouse, ymouse;
	int count = 0;
	int countMissed = 0;
};