/*
 Starter code for Data Driven App assignment CodeLab II
 Based on basic example of ofxTwitter by Christopher Baker
 <https://christopherbaker.net>
 SPDX-License-Identifier:	MIT
*/


#include "ofApp.h"
#include <string>
using namespace std;

//initial app setup
void ofApp::setup()
{
	//GENERAL SETUP
    ofSetFrameRate(30);

	//SEARCH SETUP
    client.registerSearchEvents(this);
    client.setCredentialsFromFile("credentials.json");

	//POLLING TO GET NEW TWEETS
    client.setPollingInterval(6000);

	//SEARCH TERMS
	cout << "input search critera" << endl;
	getline(cin,searchTerm);
	client.search(searchTerm);
}

void ofApp::draw()
{
    //sets background to black
    ofBackground(0);

    //counts number of tweets
    int total = count + countMissed;

    //string stream used to display number of tweets recived
    
    ss << "  Received: " << count << endl;
    ss << "    Missed: " << countMissed << endl;
    ss << "     Total: " << total << endl;
    
	
    ofDrawBitmapString(ss.str(), 10, 14);
	for (int x = 0; x < tweets.size(); x++) {
		ofDrawBitmapString(tweets[x], 150, 14);
	}
}

//This function is called everytime the a new tweet is recieved
void ofApp::onStatus(const ofxTwitter::Status& status)
{
    //increase tweet count
    count++;
    
    //output the tweet author and text
    tweetInfo << "User: " << status.user()->name() << endl;
	tweetInfo << "Tweet: " << status.text() << endl;
	tweetInfo << "\n -----------------------------\n" << endl;
	tweets.push_back(tweetInfo.str());
    

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
