#pragma once
#include "Poco/DateTime.h"
#include "Poco/Timestamp.h"
#include "ofMain.h"
#include "Twitter.h"

using Poco::DateTime;
using Poco::Timestamp;

struct TwitterMentionSorter {
	TwitterMentionSorter() {}
	bool operator()(const rtt::Tweet& a, const rtt::Tweet& b) {
		return a.created_at_timestamp > b.created_at_timestamp;
	}
};

class testApp : public ofBaseApp{

	public:
		testApp();
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void fetchMentions();
		
		ofTrueTypeFont font;
		rt::Twitter twitter;
		int fetch_again_on;
		
		string tokens_file;
		bool is_first_request;
		int rate_remaining;
		int rate_limit;
		int rate_reset;
		uint64_t last_timestamp;
		vector<rtt::Tweet> queried_mentions;
		vector<rtt::Tweet> newer_mentions;
		vector<rtt::Tweet> mentions;
		
		rcp::Collection mentions_params;
		rtt::Tweet last_mention;
		DateTime online_date;
		string last_tweet;


};
