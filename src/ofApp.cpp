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
	//GENERAL SETUP
	ofSetFrameRate(30);

	//SEARCH SETUP
	client.registerSearchEvents(this);

	//CREDENTIALS LOAD
	client.setCredentialsFromFile("credentials.json");

	//POLLING TO GET NEW TWEETS
	client.setPollingInterval(6000);

	//TEXT
	myfont.init("JustAnotherHand-Regular.ttf", 50);

	//GUI
	btn.set(ofGetWindowWidth() - basex, basey, 150, 100);
	idCheck.set(ofGetWindowWidth() / 2, (ofGetWindowHeight() / 2) + 100, 500, 100);
	searchbar.set(basex, basey, 350, 100);

	// This starts a simple search for an emoticon.
	client.search(":)");
	// Tweets are retured in the callbacks onStatus(..), onError(...), etc.]

	//SEARCH TERMS
	//cout << "input search critera" << endl;
	//getline(cin,searchTerm);
	//client.search(searchTerm);

	state = HOME;
}

void ofApp::draw()
{
	cout << "draw" << endl;
	if (state == LOGIN) {
		ofBackground(193, 106, 7);

		myfont.setColor(1, 1, 1, 255);
		myfont.setHtmlText("Enter User ID");
		myfont.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);

		ofSetColor(255, 255, 255);
		ofDrawRectangle(idCheck);

		ofSetColor(1, 1, 1);
		myfont.setHtmlText(login.str());
		myfont.draw((ofGetWindowWidth() / 2) + 20, (ofGetWindowHeight() / 2) + 100);

	}
	else if (state == HOME) {

		//BACKGROUND
		ofImage background;
		background.load("wood-background.jpg");
		ofSetColor(255, 255);
		background.draw(0, 0, ofGetScreenWidth(), ofGetScreenHeight());

		//SIDEBAR
		//
		ofSetColor(255, 255, 255);
		ofDrawRectangle(btn);

		//searchbar
		ofDrawRectangle(searchbar);
		ofSetColor(1, 1, 1);
		myfont.setHtmlText(searchstring.str());
		myfont.draw(basex + 20, basey);

		//TWEETS
		for (int i = 0; i < tweets.size(); i++) {
			//TWEET BG
			ofImage card;
			card.load("card.png");
			ofSetColor(255, 255);
			card.draw(basex + 450, basey + (i * 350), 1000, 250);

			//TWEET TXT
			myfont.setColor(1, 1, 1, 255);
			myfont.setHtmlText("User: " + tweets[i].userName + "\n" + tweets[i].tweetText);
			myfont.wrapTextX(1000, false);
			myfont.draw(basex + 460, basey + (10 + (i * 350)));
		}
		ofDrawBitmapString(xmouse, xmouse + 15, ymouse - 30);
		ofDrawBitmapString(ymouse, xmouse + 15, ymouse - 20);
	}
}

//This function is called everytime the a new tweet is recieved
void ofApp::onStatus(const ofxTwitter::Status& status)
{
	Tweet newTweet(status.user()->name(), status.text(), status.entities(), status.user()->screenName());

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
	if (btn.inside(x, y)) {
		std::cout << "Clicked button 2" << endl;
		tweets.clear();
		client.search(searchTerm);
	}
}

Tweet::Tweet(string userName, string tweetText, ofxTwitter::Entities entities, string screenName) {
	this->userName = userName;
	this->tweetText = tweetText;
	this->entities = entities;
	this->screenName = screenName;
}

void ofApp::keyPressed(int key) {
	if (state == LOGIN) {
		if (GetKeyState(VK_RETURN)) {
			
		}
		else {
			cout << char(key) << endl;
			login << char(key);
		}
	}
	else if (state == HOME) {
		if (key == ' ') {
			cout << "debug" << endl;
			cout << searchstring.str() << endl;
			tweets.clear();
			client.search(searchstring.str());
			searchstring.clear();
		}
		else {
			searchstring << char(key);
		}
	}
}
/*
void checkKey(int key) {

	char l;

	switch (key) {
		case
	}
}
*/