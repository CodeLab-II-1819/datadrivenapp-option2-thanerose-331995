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
	//setup variables
	basex = ofGetScreenWidth() / 20;
	basey = ofGetScreenHeight() / 20;
	dstate = HOME;
	sstate = ARCHIVE;
	int count = 0;
	getTweets = true;
	tweety = basey - 100;
	ispopular = false;
	process = true;
	hashtagclicked = false;

	//GENERAL SETUP
	ofSetFrameRate(30);

	//SEARCH SETUP
	client.registerSearchEvents(this);

	//CREDENTIALS LOAD
	client.setCredentialsFromFile("assets/credentials.json");
	
	//POLLING TO GET NEW TWEETS
	client.setPollingInterval(6000);
	ofxTwitter::SearchQuery query("exo");
	client.search(query);
	lastSearch = "exo";
	
	//GUI SETUP
	//text
	myfont.init("assets/JustAnotherHand-Regular.ttf", basex/5);
	//login
	idCheck.set(ofGetWindowWidth() / 2, (ofGetWindowHeight() / 2) + basey, basex*5, basey);
	//home
	//so many fuckin buttons!!!
	//search settings
	searchbar.set(basex, basey, basex * 2, basey);
	searchbtn.set(basex * 1.25, basey * 2.5, basex * 1.5, basey / 2.2);
	//archive/live
	livebtn.set(basex, basey * 3.5, basex * 0.95, basey / 2);
	archivebtn.set(basex * 2, basey * 3.5, basex * 0.95, basey / 2);
	//rightsidebar
	popularbtn.set(basex * 11, basey * 4.8, basex, basey / 2);
	recentbtn.set(basex * 11, basey * 5.6, basex, basey / 2);
	mixedbtn.set(basex * 11, basey * 6.4, basex/2, basey / 4.5);
	//leftsidebar
	hashtagsearch.set(basex*1.3, basey * 5.75, basex * 1.3, basey/2.7);

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

	//live/archive buttons
	//live
	sstate == LIVE ? ofSetColor(200, 200, 200) : ofSetColor(255, 255, 255);
	ofDrawRectangle(livebtn);
	ofSetColor(1, 1, 1);
	ofDrawBitmapString("Live Tweets", basex * 1.25, basey * 3.8);
	//archive
	sstate == ARCHIVE ? ofSetColor(200, 200, 200) : ofSetColor(255, 255, 255);
	ofDrawRectangle(archivebtn);
	ofSetColor(1, 1, 1);
	ofDrawBitmapString("Archive Tweets", basex * 2.15, basey * 3.8);

	//searchby
	ofDrawRectangle(popularbtn);
	ofDrawRectangle(recentbtn);
	ofDrawRectangle(mixedbtn);

	paper.load("assets/paper.png");
	ofSetColor(255, 255);
	paper.draw(basex * 9.5, basey * 3.5, basex*5.5, basey*10);

	myfont.setHtmlText("Search By:");
	myfont.draw(basex * 11, basey * 3.8);
	myfont.setHtmlText("> most popular");
	myfont.draw(basex * 11, basey * 4.45);
	myfont.setHtmlText("> most recent");
	myfont.draw(basex * 11, basey * 4.95);
	myfont.setHtmlText("> mixed");
	myfont.draw(basex * 11, basey * 5.45);

	//hashtags
	note.load("assets/note.png");
	ofSetColor(255, 255);
	note.draw(basex * 0.75, basey * 4.5, basex * 2.25, basey*4);
	myfont.setColor(1, 1, 1, 255);
	myfont.setHtmlText("Add Hashtags:");
	myfont.draw(basex*1.5, basey * 5.1);
	//hash search
	ofSetColor(255, 255, 255);
	ofDrawRectangle(hashtagsearch);
	ofSetColor(1, 1, 1);
	hashtagclicked == false ? ofDrawBitmapString("click here to search", basex*1.5, basey * 6) : ofDrawBitmapString(hashtagsearchstr.str(), basex*1.5, basey * 6);
	
	int thistweety = tweety;

	searchResults.clear();
	//changing array to draw array
	if (sstate == LIVE) {
		drawTweets(liveTweets);
	}
	else if(sstate == ARCHIVE) {
		drawTweets(archiveTweets);
	}

	//DRAWTWEETS
	for (int i = 0; i < searchResults.size(); i++) {

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
				like.load("assets/like.png");
				like.draw(cardx*1.05, cardy + 200, basex / 5, basex / 5);
				ofDrawBitmapString( likes, cardx * 1.125, cardy + 220);
				//retweets
				rt.load("assets/rt.png");
				rt.draw(cardx*1.2, cardy + 200, basex / 5, basex / 5);
				ofDrawBitmapString(rts, cardx * 1.275, cardy + 220);
				//replies
				rply.load("assets/reply.png");
				rply.draw(cardx*1.35, cardy + 200, basex / 5, basex / 5);
				ofDrawBitmapString(rts, cardx * 1.425, cardy + 220);
				//date
				ofDrawBitmapString(searchResults[i].date, cardx * 1.5, cardy + 220);

				thistweety += basey * 3;
			}
			else {
				int polaroidx = basex * 3.5;
				int polaroidy = basey + thistweety;
				int polaroidw = basex * 5;
				int polaroidh = basey * 10;

				//draw polaroid bg
				polaroid.load("assets/polaroid.jpg");
				ofSetColor(255, 255);
				polaroid.draw(polaroidx, polaroidy, polaroidw, polaroidh);

				//draw photo
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
				myfont.wrapTextX(polaroidw * 0.9, false);
				myfont.draw(polaroidx * 1.05, polaroidy + 910);

				//TWT INFO
				string likes = to_string(searchResults[i].likes);
				string rts = to_string(searchResults[i].retweets);
				string rpls = to_string(searchResults[i].replies);
				//likes
				like.load("assets/like.png");
				like.draw(polaroidx * 1.05, polaroidy + 1030, basex / 5, basex / 5);
				ofDrawBitmapString(likes, polaroidx * 1.125, polaroidy + 1050);
				//retweets
				rt.load("assets/rt.png");
				rt.draw(polaroidx * 1.2, polaroidy + 1030, basex / 5, basex / 5);
				ofDrawBitmapString(rts, polaroidx * 1.275, polaroidy + 1050);
				//replies
				rply.load("assets/reply.png");
				rply.draw(polaroidx * 1.35, polaroidy + 1030, basex / 5, basex / 5);
				ofDrawBitmapString(rts, polaroidx * 1.425, polaroidy + 1050);
				//date
				ofDrawBitmapString(searchResults[i].date, polaroidx * 1.55, polaroidy + 1050);

				thistweety += basey * 11.5;
			}
		}
		//cout << tweety << endl;
	}

	ofSetColor(255, 255, 255);
	ofDrawBitmapString("LOADING TWEETS", ofGetWindowWidth() / 2.25, basey + thistweety);
	ofDrawBitmapString(xmouse, xmouse + 15, ymouse - 30);
	ofDrawBitmapString(ymouse, xmouse + 15, ymouse - 20);
	}
}

void ofApp::update(){
	//count < 10 ? getTweets = true : getTweets = false;
}

//This function is called everytime the a new tweet is recieved
void ofApp::onStatus(const ofxTwitter::Status& status)
{
	if (getTweets) { //if we are still getting tweets
		process = true;
		if (status.text()[0] == 'R' && status.text()[1] == 'T') { //and its not a retweet
			process = false;
		}
		if (status.language() != "en") { //only if language is english
			process = false;
		}
		
		if (process) {
			cout << "count: " + to_string(count) << endl;
			count++;

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
					
					if (sstate == LIVE) {
						liveTweets.push_back(newTweet);
					}
					else if (sstate == ARCHIVE) {
						archiveTweets.push_back(newTweet);
					}
				}
			}
			else { //no media attached
				Tweet newTweet(status.user()->name(), status.text(), status.user()->screenName(), "0", status.replyCount(), status.retweetCount(), status.favoriteCount(), status.createdAt()); //construct tweet
				
				if (sstate == LIVE) {
					liveTweets.push_back(newTweet);
				}
				else if (sstate == ARCHIVE) {
					cout << status.text() << endl;
					archiveTweets.push_back(newTweet);
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

	//press search
	if (searchbtn.inside(x, y)) {
		lastSearch = searchstring.str();
		if (searchstring.str().length() > 0) { //only runs if theres actually a search
			if (hashtagclicked) {
				search(searchstring.str() + " #" + hashtagsearchstr.str());
			}
			search(searchstring.str());
			searchstring.str("");
		}
	}

	//SEARCH BY
	//press popular
	if (popularbtn.inside(x, y)) {
		cout << "popular clicked" << endl;
		ispopular = true;
		search(lastSearch);
	}
	//press hashtag
	if (hashtagsearch.inside(x, y)) {
		hashtagclicked = true;
	}
	else {
		hashtagsearchstr.str("");
		hashtagclicked = false;
	}

	//press archive
	if (archivebtn.inside(x, y)) {
		if (sstate != ARCHIVE) {
			sstate = ARCHIVE;
			search(lastSearch);
		}
	}
	//press live
	if (livebtn.inside(x, y)) {
		if (sstate != LIVE) {
			sstate = LIVE;
			search(lastSearch);
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
	this->date = to_string(datetime.day()) + "/" + to_string(datetime.month()) + "/" + to_string(datetime.year()) + "   " + to_string(datetime.hour()) + ":" +to_string(datetime.minute());


}

void ofApp::keyPressed(int key) {
	
	if (dstate == LOGIN) {
		if (!GetKeyState(VK_RETURN)) {
			loginString << char(key);
		}
		else {
		}
	}
	else if (dstate == HOME) {

		
		if (GetKeyState(VK_DOWN)) {
			tweety -= 50;
		}
		else if (GetKeyState(VK_UP)) {
			tweety += 50;
		}		
		else {
			if (hashtagclicked) {
				hashtagsearchstr << char(key);
			}
			else {
				searchstring << char(key); //push pressed key into search stringstream
			}
		}
		cout << char(key);
	}
}

void ofApp::drawTweets(vector<Tweet> temp) {
	for (int j = 0; j < temp.size(); j++) {
		searchResults.push_back(Tweet(temp[j]));
	}
}

void ofApp::search(string thisSearch) {

	cout << "search: " + thisSearch << endl;
	//CLEAR EVERYTHING
	count = 0;
	tweety = basey - 100;
	liveTweets.clear();
	archiveTweets.clear();
	searchResults.clear();
	
	//IF LIVE SEARCH CLIENT
	if (sstate == LIVE) {
		client.search(thisSearch);
	}
	//IF ARCHIVE SEARCH QUERY
	else if (sstate == ARCHIVE) {

		ofxTwitter::SearchQuery query(thisSearch);
		if (ispopular) {
			query.setResultType(ofxTwitter::SearchQuery::ResultType::MIXED);
		}
		client.search(query);
	}

}