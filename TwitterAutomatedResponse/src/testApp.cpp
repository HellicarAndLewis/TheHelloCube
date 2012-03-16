#include "testApp.h"

testApp::testApp()
	:online_date(2012, 3, 23)
{
}

//--------------------------------------------------------------
void testApp::setup() {
    ofBackgroundGradient(ofColor(40, 60, 70), ofColor(10,10,10));
    ofSetFrameRate(60);
	
	is_first_request = true;
	rate_limit = 0;
	rate_reset = 0;
	rate_remaining = 0;
	last_timestamp = 0;
	mentions_params["count"] = 10;
	
	
	bool use_test_account = false;
	if(use_test_account) {
		// test account.
		twitter.setConsumerKey("jVENEu6j6bEPnY3LQsPYA");
		twitter.setConsumerSecret("IGP9D4Ov2BoJYAhCqEY0iOgEoFARLQlk3qUuuCJ8");
		tokens_file = ofToDataPath("twitter_roxlutest_responder.txt", true);
	}
	else {
		// thehellocube account, TheHelloCubeResponder app.
		twitter.setConsumerKey("jVENEu6j6bEPnY3LQsPYA");
		twitter.setConsumerSecret("IGP9D4Ov2BoJYAhCqEY0iOgEoFARLQlk3qUuuCJ8");
		tokens_file = ofToDataPath("twitter_thehellocube_responder.txt", true);
	}
	
	if(!twitter.loadTokens(tokens_file)) {
		string auth_url;
		twitter.requestToken(auth_url);
		twitter.handlePin(auth_url);
		twitter.accessToken();
		twitter.saveTokens(tokens_file);
	}
	fetch_again_on = ofGetElapsedTimeMillis() +1000;
	font.loadFont("font.otf", 30);
	last_tweet = "...";
}

//--------------------------------------------------------------
void testApp::update(){
	int now = ofGetElapsedTimeMillis();
	int diff = fetch_again_on - now;
	if(diff < 0) {
		fetchMentions();
	}
}

void testApp::fetchMentions() {
	int millis = ofGetElapsedTimeMillis();
	DateTime now;
	Poco::Timespan time_to_online = online_date - now;
	string time_to_online_str = ofToString(time_to_online.days()) +"d "  +ofToString(time_to_online.hours()) +"h " +ofToString(time_to_online.minutes()) +"m!";

	// FETCH MENTIONS
	// -----------------------
	twitter.statusesMentions(&mentions_params);
	queried_mentions.clear();
	newer_mentions.clear();
	twitter.getJSON().parseStatusArray(twitter.getResponse(), queried_mentions);
	std::sort(queried_mentions.begin(), queried_mentions.end(), TwitterMentionSorter());
	
	// filter out mentions which are newer then previous		
	for(int i = 0; i < queried_mentions.size(); ++i) {	
		rtt::Tweet& tw = queried_mentions[i];
		if(tw.created_at_timestamp > last_timestamp) {
			newer_mentions.push_back(tw);
			printf(">> %s\n", tw.getText().c_str());
		}
	}
	
	string str_remaining;
	std::stringstream ss_remaining;
	if(twitter.getResponseHeader("x-ratelimit-remaining", str_remaining)) {
		ss_remaining << str_remaining;
		ss_remaining >> rate_remaining;
	}
	
	string str_limit;
	std::stringstream ss_limit;
	if(twitter.getResponseHeader("x-ratelimit-limit", str_limit)) {
		ss_limit << str_limit;
		ss_limit >> rate_limit;
	}

	time_t ts;	
	string str_reset;
	std::stringstream ss_reset;
	if(twitter.getResponseHeader("x-ratelimit-reset", str_reset)) {
		ss_reset << str_reset;
		ss_reset >> ts;
		ss_reset << str_reset;
		ss_reset >> rate_reset;
	}
	
	if(str_remaining == "0") {
		time_t curr_ts = time(NULL);
		time_t diff_tt = ts - curr_ts;
		fetch_again_on = millis + (int)diff_tt * 1000;
		return;
		
	}
	
	if(newer_mentions.size() <= 0) {
		printf("No new mentions found.\n");
		fetch_again_on = millis + 30 * 1000;
		return;
	}


	// HANDLE NEW MENTIONS
	// --------------------
	last_mention = newer_mentions.at(0);			
	mentions_params["since_id"] = last_mention.getTweetID();
	std::copy(newer_mentions.begin(), newer_mentions.end(), std::back_inserter(mentions));
	
	if(!is_first_request) {
		
		vector<rtt::Tweet>::iterator it = mentions.begin();
		while(it != mentions.end()) {
			rtt::Tweet& tweet = *it;
			 http://bit.ly/xrx1xP
			// ok.. not optimized .... 
			vector<string> messages;
			messages.push_back("@" +tweet.getScreenName() +" INTERACT with me 23-25 March @Tate Modern. Tweet commands in only " +time_to_online_str +" #InfiniteKusama http://bit.ly/xrx1xP"); 
			messages.push_back("@" +tweet.getScreenName() +" INTERACT with me 23-25 March @Tate Modern. Tweet Commands in only " +time_to_online_str +" #InfiniteKusama http://bit.ly/xrx1xP"); 
			messages.push_back("@" +tweet.getScreenName() +" INTERACT with me 23-25 March @Tate Modern. Tweet Commands in only " +time_to_online_str +" #InfiniteKusama http://bit.ly/xrx1xP"); 
			messages.push_back("@" +tweet.getScreenName() +" INTERACT with me 23-25 March @Tate Modern. Tweet Commands in only " +time_to_online_str +" #InfiniteKusama http://bit.ly/xrx1xP"); 
			string message = messages.at((int)ofRandom(0,messages.size()-1));
			printf("< %s\n", tweet.getText().c_str());
			printf("> %s\n", message.c_str());
			twitter.statusesUpdate(message);
			last_tweet = tweet.getText();
			printf("response:\n-------------\n%s\n-------------\n\n", twitter.getResponse().c_str());
			
			++it;
		}
	
	}
	fetch_again_on = millis + 60 * 1000;
	
	mentions.clear();
	is_first_request = false;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor(40, 60, 70), ofColor(10,10,10));
	int now = ofGetElapsedTimeMillis();
	int diff = fetch_again_on - now;
	string msg = "Check for new tweets in: " +ofToString(diff/1000) + " seconds";
	glColor3f(1,1,1);
	font.drawString(msg, 50,100);
	glColor3f(1.0, 0.72, 0.0);
	font.drawString(last_tweet, 50,150);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}