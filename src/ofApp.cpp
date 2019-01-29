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
#include <Poco/DateTime.h>
using namespace std;

//initial app setup
void ofApp::setup()
{
	basex = ofGetScreenWidth() / 20;
	basey = ofGetScreenHeight() / 20;
	dstate = HOME;
	sstate = ARCHIVE;
	int count = 0;
	firstLoad = true;
	getTweets = false;
	tweety = basey - 100;

	//GENERAL SETUP
	ofSetFrameRate(30);

	//SEARCH SETUP
	client.registerSearchEvents(this);

	//CREDENTIALS LOAD
	client.setCredentialsFromFile("assets/credentials.json");
	
	//POLLING TO GET NEW TWEETS
	client.setPollingInterval(6000);
	//client.search("exo cbx"); //example search

	ofxTwitter::SearchQuery query("ihlfsaldfijdsalfjlesakfnesahdfgh");
	client.search(query);

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
	if (dstate == LOGIN) {
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
	else if (dstate == HOME) {

	//BACKGROUND
	ofImage background;
	background.load("assets/wood-background.jpg");
	ofSetColor(255, 255);
	background.draw(0, 0, ofGetScreenWidth(), ofGetScreenHeight());

	//SIDEBAR

	//search
	ofSetColor(255, 255, 255);
	ofDrawRectangle(searchbtn); //button

	ofSetColor(1, 1, 1);
	ofDrawBitmapString("SEARCH", basex * 1.8, basey * 2.8); //search text

	ofSetColor(255, 255, 255);
	ofDrawRectangle(searchbar); //bar

	ofSetColor(1, 1, 1);
	myfont.setHtmlText(searchstring.str()); //search string
	myfont.draw(basex * 1.2, basey);


	int thistweety = tweety;

	//changing array to draw array
	sstate == LIVE ? drawTweets(liveTweets) : drawTweets(archiveTweets);

	//DRAWTWEETS
	for (int i = 0; i < searchResults.size(); i++) {
		cout << searchResults[i].day << endl;
		if (searchResults[i].tweetText.size() == 0) {
			cout << "empty tweet" << endl;
		}
		else { //draw tweets
			if (searchResults[i].mediaID == "0") { //text only tweets
				
				//initilising individual tweet values
				int cardx = basex * 3.5;
				int cardy = basey + thistweety;
				int cardw = basex * 5;
				int cardh = basey * 2.25;

				//TWEET BG
				ofImage card;
				card.load("assets/card.png");
				ofSetColor(255, 255);
				card.draw(cardx, cardy, cardw, cardh);

				//TWEET TXT
				myfont.setColor(1, 1, 1, 255);
				//username
				myfont.setHtmlText("User: " + searchResults[i].screenName);
				myfont.draw(cardx * 1.05, cardy + 10);
				//tweet
				myfont.setHtmlText(searchResults[i].tweetText);
				myfont.wrapTextX(cardx * 1.35, false);
				myfont.draw(cardx * 1.05, cardy + 60);

				//TWT INFO
				string likes = to_string(searchResults[i].likes);
				string rts = to_string(searchResults[i].retweets);
				string rpls = to_string(searchResults[i].replies);
				//likes
				ofImage like;
				like.load("assets/like.png");
				like.draw(cardx*1.05, cardy + 200, basex / 5, basex / 5);
				ofDrawBitmapString( likes, cardx * 1.125, cardy + 220);
				//retweets
				ofImage rt;
				rt.load("assets/rt.png");
				rt.draw(cardx*1.2, cardy + 200, basex / 5, basex / 5);
				ofDrawBitmapString(rts, cardx * 1.275, cardy + 220);
				//replies
				ofImage rply;
				rply.load("assets/reply.png");
				rply.draw(cardx*1.35, cardy + 200, basex / 5, basex / 5);
				ofDrawBitmapString(rts, cardx * 1.425, cardy + 220);

				thistweety += basey * 3;
			}
			else {
				int polaroidx = basex * 3.5;
				int polaroidy = basey + thistweety;
				int polaroidw = basex * 5;
				int polaroidh = basey * 10;

				//draw polaroid bg
				ofImage polaroid;
				polaroid.load("assets/polaroid.jpg");
				ofSetColor(255, 255);
				polaroid.draw(polaroidx, polaroidy, polaroidw, polaroidh);

				//draw photo
				ofImage photo;
				photo.load(searchResults[i].mediaID);
				ofSetColor(255, 255);
				photo.draw(polaroidx * 1.05, polaroidy + 20, polaroidw * 0.925, basey * 7.75);
				
				//TWEET TXT
				myfont.setColor(1, 1, 1, 255);
				//username
				myfont.setHtmlText("User: " + searchResults[i].screenName);
				myfont.draw(polaroidx * 1.05, polaroidy + 860);
				//tweet
				myfont.setHtmlText(searchResults[i].tweetText);
				myfont.wrapTextX(polaroidx * 0.5, false);
				myfont.draw(polaroidx * 1.05, polaroidy + 910);

				//TWT INFO
				string likes = to_string(searchResults[i].likes);
				string rts = to_string(searchResults[i].retweets);
				string rpls = to_string(searchResults[i].replies);
				//likes
				ofImage like;
				like.load("assets/like.png");
				like.draw(polaroidx*1.05, polaroidy + 200, basex / 5, basex / 5);
				ofDrawBitmapString(likes, polaroidx * 1.125, polaroidy + 1070);
				//retweets
				ofImage rt;
				rt.load("assets/rt.png");
				rt.draw(polaroidx*1.2, polaroidy + 200, basex / 5, basex / 5);
				ofDrawBitmapString(rts, polaroidx * 1.275, polaroidy + 1070);
				//replies
				ofImage rply;
				rply.load("assets/reply.png");
				rply.draw(polaroidx*1.35, polaroidy + 200, basex / 5, basex / 5);
				ofDrawBitmapString(rts, polaroidx * 1.425, polaroidy + 220);

				thistweety += basey * 11.5;
			}
		}
		//cout << tweety << endl;
	}

	ofDrawBitmapString(xmouse, xmouse + 15, ymouse - 30);
	ofDrawBitmapString(ymouse, xmouse + 15, ymouse - 20);
	}
}

void ofApp::update(){
	count < 10 ? getTweets = true : getTweets = false;
}

//This function is called everytime the a new tweet is recieved
void ofApp::onStatus(const ofxTwitter::Status& status)
{
	cout << archiveTweets.size() << endl;

	if (getTweets) { //if we are still getting tweets
		if (status.text()[0] == 'R' && status.text()[1] == 'T') { //and its not a retweet

		}
		else {

			if (status.language() == "en") {

				if (status.extendedEntities().mediaEntities().size()) { //check there is a media attached
					for (auto e : status.extendedEntities().mediaEntities()) //check for media type
					{
						if (e.type() == ofxTwitter::MediaEntity::Type::PHOTO) //if media is photo
						{
							string filename = e.mediaFilename();
							mediaFilename = filename; //so the push_back can call out of the for
							ofSaveURLAsync(e.mediaURL(), filename); //save the file
							ofLogNotice("ofApp::onStatus") << "Downloading" << filename;
						}
						else {
							cout << status.url() << endl; //cout url so i can check empty tweet
						}
						Tweet newTweet(status.user()->name(), status.text(), status.user()->screenName(), mediaFilename, status.replyCount(), status.retweetCount(), status.favoriteCount(), status.createdAt()); //construct tweet
						//sstate == LIVE ? liveTweets.push_back(newTweet) : archiveTweets.push_back(newTweet);
						if (sstate == LIVE) {
							liveTweets.push_back(newTweet);
						}
						else if (sstate == ARCHIVE) {
							archiveTweets.push_back(newTweet);
						}
						count++;
					}
				}
				else { //no media attached
					Tweet newTweet(status.user()->name(), status.text(), status.user()->screenName(), "0", status.replyCount(), status.retweetCount(), status.favoriteCount(), status.createdAt()); //construct tweet
					//sstate == LIVE ? liveTweets.push_back(newTweet) : archiveTweets.push_back(newTweet);
					if (sstate == LIVE) {
						liveTweets.push_back(newTweet);
					}
					else if (sstate == ARCHIVE) {
						archiveTweets.push_back(newTweet);
					}
					count++;
				}
				if (status.place() != 0) {
					cout << status.place()->name() << endl;
				}
			}
		}
	}
	
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
			//debug
			cout << "Clicked button 2" << endl;

			liveTweets.clear();
			archiveTweets.clear();
			count = 0;
			tweety = basey - 100;

			//if search is a live search
			if (sstate == LIVE) {

				cout << "search: " + searchstring.str() << endl;
				liveTweets.clear();
				client.search(searchstring.str());
				searchstring.str("");
			}
			//else if archive
			else if (sstate == ARCHIVE) {
				cout << "search: " + searchstring.str() << endl;
				ofxTwitter::SearchQuery query(searchstring.str());
				client.search(query);
			}
		}
	}
}

Tweet::Tweet(string userName, string tweetText, string screenName, string mediaID, int replies, int retweets, int likes, Poco::DateTime &datetime) {

	//username
	this->userName = userName;
	//screenname
	this->screenName = screenName;
	//actual tweet text
	this->tweetText = tweetText;
	//media if any
	this->mediaID = mediaID;
	cout << "downloaded: " + mediaID << endl;
	//replies
	if (replies <= 0) {
		this->replies = 0;
	}
	else {
		this->replies = replies;
	}
	//retweets
	if (retweets <= 0) {
		this->retweets = 0;
	}
	else {
		this->retweets = retweets;
	}
	//likes
	if (likes <= 0) {
		this->likes = 0;
	}
	else {
		this->likes = likes;
	}
	//datetime
	this->date = to_string(datetime.day()) + "/" + to_string(datetime.month()) + "/" + to_string(datetime.year());


}

void ofApp::keyPressed(int key) {
	
	if (dstate == LOGIN) {
		if (!GetKeyState(VK_RETURN)) {
			loginString << char(key);
		}
		else {
			//APICall = ("https://api.twitter.com/1.1/statuses/user_timeline.json?" + loginString.str() + "=twitterapi&count=2");
			//login(APICall);
		}
	}
	else if (dstate == HOME) {
		if (GetKeyState(VK_SPACE)) {
			if (sstate == LIVE) {
				sstate = ARCHIVE;
				cout << sstate << endl;
			}
			else {
				sstate = LIVE;
				cout << sstate << endl;
			}
		}
		/*
		if (GetKeyState(VK_DOWN)) {
			tweety -= 50;
		}
		if (GetKeyState(VK_UP)) {
			tweety += 50;
		}
		*/
		cout << char(key);
		searchstring << char(key); //push pressed key into search stringstream
	}
	

}

void ofApp::drawTweets(vector<Tweet> temp) {
	for (int j = 0; j < temp.size(); j++) {
		searchResults.push_back(Tweet(temp[j]));
	}
}