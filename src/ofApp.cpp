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
#include <fstream>
using namespace std;

//initial app setup
void ofApp::setup()
{
	//setup variables
	basex = ofGetScreenWidth() / 20;
	basey = ofGetScreenHeight() / 20;
	sstate = ARCHIVE;
	int count = 0;
	tweety = basey - 100;
	//set bools
	getTweets = true;
	ispopular = false;
	process = true;
	hashtagclicked = false;
	mediaonly = false;

	//GENERAL SETUP
	ofSetFrameRate(30);

	//SEARCH SETUP
	client.registerSearchEvents(this);

	//CREDENTIALS LOAD
	client.setCredentialsFromFile("assets/credentials.json");
	
	//POLLING TO GET NEW TWEETS
	client.setPollingInterval(6000);
	getfiledata();
	ofxTwitter::SearchQuery query(lastSearch);
	client.search(query);
	
	//GUI SETUP
	//text
	myfont.init("assets/JustAnotherHand-Regular.ttf", basex/5);
	//home
	//so many fuckin buttons!!!
	exit.set(ofGetWindowWidth() - (basex / 2), 0, basex / 2, basex / 2);
	exiticon.load("assets/exit.png");
	//search settings
	searchbar.set(basex, basey, basex * 2, basey);
	searchbtn.set(basex * 1.25, basey * 2.5, basex * 1.5, basey / 2.2);
	//archive/live
	livebtn.set(basex, basey * 3.5, basex * 0.95, basey / 2);
	archivebtn.set(basex * 2, basey * 3.5, basex * 0.95, basey / 2);
	//rightsidebar
	popularbtn.set(basex * 11, basey * 4.45, basex, basey / 2);
	recentbtn.set(basex * 11, basey * 4.95, basex, basey / 2);
	mixedbtn.set(basex * 11, basey * 5.45, basex, basey / 2);
	location1.set(basex * 11, basey * 8, basex, basey / 2);
	location2.set(basex * 11, basey * 8.5, basex, basey / 2);
	location3.set(basex * 11, basey * 9, basex, basey / 2);
	location4.set(basex * 11, basey * 9.5, basex, basey / 2);
	//media
	mediacheck.set(ofGetWindowWidth() - (basex * 2.7), basey*2.75, basex, basex / 5);
	//leftsidebar
	hashtagsearch.set(basex*1.3, basey * 5.75, basex * 1.3, basey/2.7);
	add.load("assets/add.png");
	addhashtag.set(basex*2.4, basey * 5.78, basex / 6, basex / 6);
	clearhash.set(basex*1.6, basey * 7.5, basex * 0.5, basey / 2.7);

}

void ofApp::draw()
{
	//BACKGROUND
	background.load("assets/wood-background.jpg");
	ofSetColor(255, 255);
	background.draw(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
	background.draw(0, 1800, ofGetScreenWidth(), ofGetScreenHeight());
	
	ofSetColor(255, 255);
	exiticon.draw(exit);
	coffee.load("assets/coffee.png");
	coffee.draw(basex * 8.2, 0 - (basex * 1.5), basex * 3.5, basex * 3.5);
	office.load("assets/officeitems.png");
	office.draw(basex - (basex * 3.8), basey * 8.7, basex * 6.7, basex * 6.5);

	//SIDEBAR
	//meida
	ofSetColor(255, 255, 255);
	ofDrawRectangle(mediacheck); //button
	ofSetColor(1, 1, 1, 255);
	mediaonly == true ? ofDrawBitmapString("Media Only (ACTIVE)", ofGetWindowWidth() - (basex * 2.62), basey * 2.95) : ofDrawBitmapString("Media Only (INACTIVE)", ofGetWindowWidth() - (basex * 2.62), basey * 2.95);

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
	ofDrawRectangle(location1);
	ofDrawRectangle(location2);
	ofDrawRectangle(location3);
	ofDrawRectangle(location4);

	paper.load("assets/paper.png");
	ofSetColor(255, 255);
	paper.draw(basex * 9.5, basey * 3.5, basex*5.5, basey*10);

	myfont.setHtmlText("Search By:");
	myfont.draw(basex * 11, basey * 3.8);
	ispopular == true ? myfont.setHtmlText("> most popular") : myfont.setHtmlText("  most popular");
	myfont.draw(basex * 11, basey * 4.45);
	isrecent == true ? myfont.setHtmlText("> most recent") : myfont.setHtmlText("  most recent");
	myfont.draw(basex * 11, basey * 4.95);
	ismixed == true ? myfont.setHtmlText("> most mixed") : myfont.setHtmlText("  most mixed");
	myfont.draw(basex * 11, basey * 5.45);

	//locations
	map.load("assets/map.png");
	ofSetColor(255, 255);
	map.draw(basex * 10, basey * 9, basex*5.5, basey * 11);

	myfont.setHtmlText("Search In:");
	myfont.setColor(1, 1, 1, 255);
	myfont.draw(basex * 11, basey * 7.3);

	//bath
	myfont.setColor(1, 1, 1, 255);
	myfont.setHtmlText("- bath");
	myfont.draw(basex * 11, basey * 8);
	//london
	myfont.setHtmlText("- london");
	myfont.draw(basex * 11, basey * 8.5);
	//usa
	myfont.setHtmlText("- USA");
	myfont.draw(basex * 11, basey * 9);
	//golbal
	myfont.setHtmlText("- global");
	myfont.draw(basex * 11, basey * 9.5);

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
	hashtagclicked == false ? ofDrawBitmapString("click here to search", basex*1.4, basey * 6) : ofDrawBitmapString(hashtagsearchstr.str(), basex*1.4, basey * 6);
	//add button
	if (hashtagclicked) {
		ofSetColor(255, 255);
		add.draw(addhashtag);
	}
	//previous hashtags
	for (int u = 0; u < hashtags.size(); u++) {
		ofDrawBitmapString(hashtags[u], basex * 1.4, (basey * 6.7) + (u * 10));
	}
	ofSetColor(255, 255, 255);
	ofDrawRectangle(clearhash);
	ofSetColor(1, 1, 1);
	ofDrawBitmapString("clear", basex * 1.75, basey * 7.7);

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
	ofDrawBitmapString("SEARCHING FOR: " + lastSearch, ofGetWindowWidth() / 2.25, basey - 50);
	ofDrawBitmapString(xmouse, xmouse + 15, ymouse - 30);
	ofDrawBitmapString(ymouse, xmouse + 15, ymouse - 20);
	
}

void ofApp::update(){
	
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
			cout << "count of tweets: " + to_string(count) << endl;
			count++;

			if (status.extendedEntities().mediaEntities().size()) { //check there is a media attached

				for (auto e : status.extendedEntities().mediaEntities()) //check for media type
				{
					if (e.type() == ofxTwitter::MediaEntity::Type::PHOTO || e.type() == ofxTwitter::MediaEntity::Type::ANIMATED_GIF) //if media is photo
					{
						string filename = e.mediaFilename();
						mediaFilename = filename; //so the push_back can call out of the for
						ofSaveURLAsync(e.mediaURL(), filename); //save the file
						ofLogNotice("ofApp::onStatus") << "Downloading" << filename;

						Tweet newTweet(status.user()->name(), status.text(), status.user()->screenName(), mediaFilename, status.replyCount(), status.retweetCount(), status.favoriteCount(), status.createdAt()); //construct tweet
						if (sstate == LIVE) {
							liveTweets.push_back(newTweet);
						}
						else if (sstate == ARCHIVE) {
							archiveTweets.push_back(newTweet);
						}
					}
					else {
						cout << "media cant download, url: " + status.url() << endl; //cout url so i can check empty tweet
					}
				}
			}
			else { //no media attached
				if (!mediaonly) {
					Tweet newTweet(status.user()->name(), status.text(), status.user()->screenName(), "0", status.replyCount(), status.retweetCount(), status.favoriteCount(), status.createdAt()); //construct tweet

					if (sstate == LIVE) {
						liveTweets.push_back(newTweet);
					}
					else if (sstate == ARCHIVE) {
						archiveTweets.push_back(newTweet);
					}
				}
			}
			if (status.place() != 0) {
				cout << "locatation of tweet: " + status.place()->name() << endl;
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

	//exit
	if (exit.inside(x, y)) {
		savefiledata();
		ofExit();
	}

	//press search
	if (searchbtn.inside(x, y)) {
		lastSearch = searchstring.str();
		if (searchstring.str().length() > 0) { //only runs if theres actually a search
			search(searchstring.str());
			searchstring.str("");
		}
	}
	
	//SEARCH BY
	//media
	if (mediacheck.inside(x,y)) {
		if (mediaonly) {
			mediaonly = false;
		}
		else {
			mediaonly = true;
		}
		search(lastSearch);
	}
	//popular
	if (popularbtn.inside(x, y)) {
		ispopular = true;
		isrecent = false;
		ismixed = false;
		search(lastSearch);
	}
	//recent
	if (recentbtn.inside(x, y)) {
		cout << "recent clicked" << endl;
		ispopular = false;
		isrecent = true;
		ismixed = false;
		search(lastSearch);
	}
	//mixed
	if (mixedbtn.inside(x, y)) {
		cout << "popular clicked" << endl;
		ispopular = false;
		isrecent = false;
		ismixed = true;
		search(lastSearch);
	}

	//HASHTAGS
	//press hashtag
	if (hashtagsearch.inside(x, y)) {
		hashtagclicked = true;
	}
	else {
		hashtagsearchstr.str("");
		hashtagclicked = false;
	}
	//press add
	if (addhashtag.inside(x, y)) {
		if (hashtagclicked) {
			hashtags.push_back(hashtagsearchstr.str());
			allhashtags << " #" + hashtagsearchstr.str();
			hashtagsearchstr.str("");
			search(allhashtags.str());
		}
	}
	//clear hashtag
	if (clearhash.inside(x, y)) {
		hashtagsearchstr.str("");
	}

	//LOCATIONS
	if (location1.inside(x, y)) {
		location = 1;
		checklocation = true;
		search(lastSearch);
	}
	if (location2.inside(x, y)) {
		location = 2;
		checklocation = true;
		search(lastSearch);
	}
	if (location3.inside(x, y)) {
		location = 3;
		checklocation = true;
		search(lastSearch);
	}
	if (location4.inside(x, y)) {
		location = 4;
		checklocation = true;
		search(lastSearch);
	}

	//TWEET TYPE
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

	cout << char(key);

	if (GetKeyState(VK_DOWN)) {
		tweety -= 50;
	}
	else if (GetKeyState(VK_UP)) {
		tweety += 50;
	}		
	else {
		if (hashtagclicked) {
			hashtagsearchstr << char(key);
			cout << (key);
		}
		else {
			if (GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) {
				searchstring << char(key); //push pressed key into search stringstream
			}
			else {
				if (key == 64) {
					searchstring << "@";
				}					
			}
		}
	}
}

void ofApp::drawTweets(vector<Tweet> temp) {
	for (int j = 0; j < temp.size(); j++) {
		searchResults.push_back(Tweet(temp[j]));
	}
}

void ofApp::search(string thisSearch) {

	lastSearch = thisSearch;
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

		//initialise search
		ofxTwitter::SearchQuery query(thisSearch);

		//CONDITIONS

		//search by
		if (ispopular) {
			query.setResultType(ofxTwitter::SearchQuery::ResultType::POPULAR);
		}
		else if (isrecent) {
			query.setResultType(ofxTwitter::SearchQuery::ResultType::RECENT);
		}
		else if (ismixed) {
			query.setResultType(ofxTwitter::SearchQuery::ResultType::MIXED);
		}

		//location check
		if (checklocation) {
			switch (location) {
			case 1:
				query.setGeoCode(51.38250, -2.36699, 10, ofxTwitter::SearchQuery::UNITS_MILES);
				break;
			case 2:
				query.setGeoCode(51.51770, -0.11352, 45, ofxTwitter::SearchQuery::UNITS_MILES);
				break;
			case 3:
				query.setGeoCode(51.43811, 5.97519, 1000, ofxTwitter::SearchQuery::UNITS_MILES);
				break;
			default:
				break;
			}

		}

		//RUN SEARCH
		client.search(query);
	}
	searchstring.str(""); //clearstring
}

void ofApp::getfiledata() {
	ifstream inFile;
	inFile.open("savefile.txt");
	if (inFile.good()) { // check file has opened
		getline(inFile, lastSearch);
		cout << "retrieved: " + lastSearch << endl;
		inFile.close(); // close file
	}
}

void ofApp::savefiledata() {
	ofstream outfile;
	outfile.open("savefile.txt");
	if (outfile.good()) { //check file is open
		cout << "saving: " + lastSearch << endl;
		outfile << lastSearch << endl;
		outfile.close(); // close file
	}
}