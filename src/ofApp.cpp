/*
 Starter code for Data Driven App assignment CodeLab II
 Based on basic example of ofxTwitter by Christopher Baker
 <https://christopherbaker.net>
 SPDX-License-Identifier:	MIT
*/

#include "ofApp.h"
#include "ofxGui.h"
#include "ofxTextSuite.h"
#include <string>
using namespace std;

//initial app setup
void ofApp::setup()
{
	basex = ofGetScreenWidth() / 20;
	basey = ofGetScreenHeight() / 20;
	state = HOME;
	int count = 0;
	firstLoad = true;

	//GENERAL SETUP
	ofSetFrameRate(30);

	//SEARCH SETUP
	client.registerSearchEvents(this);

	//CREDENTIALS LOAD
	client.setCredentialsFromFile("assets/credentials.json");
	/*
	APIKey = "&APPID=JtaV8RWoDfnbOqhgHyVVhm1fH";
	APICall = "https://api.twitter.com/1.1/statuses/home_timeline.json";

	parseSuccess = json.open(APICall);
	if (parseSuccess) {
		cout << json.getRawString(true) << endl;
	}
	else {
		cout << "error" << endl;
	}
	login(APICall);
	*/

	//POLLING TO GET NEW TWEETS
	client.setPollingInterval(6000);
	client.search("exo cbx");

	//TEXT
	myfont.init("assets/JustAnotherHand-Regular.ttf", basex/5);

	//GUI SETUP
	//login
	idCheck.set(ofGetWindowWidth() / 2, (ofGetWindowHeight() / 2) + basey, basex*5, basey);
	//home
	searchbar.set(basex, basey, basex * 2, basey);
	searchbtn.set(basex * 1.25, basey * 2.5, basex * 1.5, basey / 2);
}

void ofApp::draw()
{
	if (state == LOGIN) {
		//BACKGROUND
		ofBackground(193, 106, 7);

		//LOGIN TEXT
		myfont.setColor(1, 1, 1, 255);
		myfont.setHtmlText("Enter User ID");
		myfont.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);

		//ENTERBOX
		ofSetColor(255, 255, 255);
		ofDrawRectangle(idCheck);

		//USERTEXT
		ofSetColor(1, 1, 1);
		myfont.setHtmlText(loginString.str());
		myfont.draw((ofGetWindowWidth() / 2) + (basex*0.2), (ofGetWindowHeight() / 2) + basey);

	}
	else if (state == HOME) {

		//BACKGROUND
		ofImage background;
		background.load("assets/wood-background.jpg");
		ofSetColor(255, 255);
		background.draw(0, 0, ofGetScreenWidth(), ofGetScreenHeight());

		//SIDEBAR
		//search
		ofSetColor(255, 255, 255);
		ofDrawRectangle(searchbtn);

		ofSetColor(1, 1, 1);
		ofDrawBitmapString("SEARCH", basex * 1.8, basey * 2.8);

		ofSetColor(255, 255, 255);
		ofDrawRectangle(searchbar);

		ofSetColor(1, 1, 1);
		myfont.setHtmlText(searchstring.str());
		myfont.draw(basex * 1.2, basey);

		//TWEETS
		if (firstLoad) {
			tweety = basey - 100;
		}

		for (int i = 0; i < tweets.size(); i++) {
			if (tweets[i].tweetText.size() == 0) {
				cout << "empty tweet" << endl;
			}
			else {
				if (tweets[i].mediaID.size() == 0) {
					//TWEET BG
					ofImage card;
					card.load("assets/card.png");
					ofSetColor(255, 255);
					card.draw(basex * 3.5, basey + tweety, basex * 5, basey * 2.25);

					//TWEET TXT
					myfont.setColor(1, 1, 1, 255);
					//username
					myfont.setHtmlText("User: " + tweets[i].screenName);
					myfont.wrapTextX(basex * 4.5, false);
					myfont.draw(basex * 3.75, basey + (10 + tweety));
					//tweet
					myfont.setHtmlText(tweets[i].tweetText);
					myfont.wrapTextX(basex * 4.5, false);
					myfont.draw(basex * 3.75, basey + (60 + tweety));

					tweety += basey * 3;
				}
				else {
					ofImage polaroid;
					polaroid.load("assets/polaroid.jpg");
					ofSetColor(255, 255);
					polaroid.draw(basex * 3.5, basey + tweety, basex * 5, basey * 10);

					ofImage photo;
					photo.load(tweets[i].mediaID);
					ofSetColor(255, 255);
					photo.draw(basex * 3.75, basey + (20 + tweety), basex * 4.5, basey * 7.75);

					tweety += basey * 11.5;
				}
			}
			//cout << tweety << endl;
		}
		ofDrawBitmapString(xmouse, xmouse + 15, ymouse - 30);
		ofDrawBitmapString(ymouse, xmouse + 15, ymouse - 20);
	}
}

//This function is called everytime the a new tweet is recieved
void ofApp::onStatus(const ofxTwitter::Status& status)
{
	//if(RETWEET)
	for (auto e : status.extendedEntities().mediaEntities()) 
	{
		if (e.type() == ofxTwitter::MediaEntity::Type::PHOTO) 
		{
			string filename = e.mediaFilename();
			ofSaveURLAsync(e.mediaURL(), filename); //save the file
			mediaFilename = filename;
			ofLogNotice("ofApp::onStatus") << "Downloading" << filename;
		}		
		else {
			cout << status.url() << endl;
		}
	}
	Tweet newTweet(status.user()->name(), status.text(), status.user()->screenName(), mediaFilename);
	tweets.push_back(newTweet);
}

//returns an error message if error encountered recieving tweets
void ofApp::onError(const ofxTwitter::Error& error)
{
	std::cout << "Error: " << error.code() << " " << error.message();
}

//returns an exception message if exception encountered recieving tweets
void ofApp::onException(const std::exception& notice)
{
	std::cout << "Exception: " << notice.what();
}

void ofApp::onMessage(const ofJson& json)
{
	// This is the raw message json and is ignored here.
}

void ofApp::mouseMoved(int x, int y) {
	xmouse = x;
	ymouse = y;
}

void ofApp::mousePressed(int x, int y, int button) {
	if (searchbtn.inside(x, y)) {
		if (searchstring.str().length() > 0) { //only runs if theres actually a search
			cout << "Clicked button 2" << endl;
			tweets.clear();
			cout << tweets.size() << endl;
			cout << searchstring.str() << endl;
			client.search(searchstring.str());
			searchstring.str("");
		}
	}
}

Tweet::Tweet(string userName, string tweetText, string screenName, string mediaID) {
	this->userName = userName;
	this->tweetText = tweetText;
	this->screenName = screenName;
	this->mediaID = mediaID;
}

void ofApp::keyPressed(int key) {
	if (state == LOGIN) {
		if (!GetKeyState(VK_RETURN)) {
			cout << char(key) << endl;
			loginString << char(key);
		}
		else {
			APICall = ("https://api.twitter.com/1.1/statuses/user_timeline.json?" + loginString.str() + "=twitterapi&count=2");
			login(APICall);
		}
	}
	else if (state == HOME) {
		if (GetKeyState(VK_DOWN)) {
			cout << "down" << endl;
			basey -= 10;
		}
		if (GetKeyState(VK_UP)) {
			cout << "up" << endl;
			basey += 10;
		}
		cout << char(key);
		searchstring << char(key);
	}

}

void ofApp::login(string APICall) {
}