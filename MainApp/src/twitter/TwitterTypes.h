/**
 * Shared twitter types.
 * 
 * 
 * 
 * @date	2012.03.xx
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 */
#ifndef TWITTER_TYPESH
#define TWITTER_TYPESH

#include "ofMain.h"
#include "Twitter.h"
#include <map>
#include <set>
#include <string>

using std::map;
using std::set;
using std::string;

struct TwitterMentionSorter {
	TwitterMentionSorter() {}
	bool operator()(const rtt::Tweet& a, const rtt::Tweet& b) {
		return a.created_at_timestamp > b.created_at_timestamp;
	}
};

struct TwitterCommand {
	TwitterCommand() {
	}
	
	TwitterCommand(rtt::Tweet tweet, set<string> tokens, map<string, ofColor> colours)	
		:tweet(tweet)
		,tokens(tokens)
		,colours(colours)
	{
	}
	
	void print() {
		set<string>::iterator it = tokens.begin();
		while(it != tokens.end()) {
			printf("command: %s\n", (*it).c_str());
			++it;
		}
		
		map<string, ofColor>::iterator mit = colours.begin();
		while(mit != colours.end()) {
			printf("colour: %s\n", mit->first.c_str());
			++mit;
		}
	}
	
	rtt::Tweet tweet;
	set<string> tokens;
	map<string, ofColor> colours;
};


#endif
