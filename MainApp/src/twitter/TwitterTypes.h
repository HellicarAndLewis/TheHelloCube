#ifndef TWITTER_TYPESH
#define TWITTER_TYPESH

#include "Twitter.h"
#include <set>
#include <string>

using std::set;
using std::string;

struct TwitterMentionSorter {
	TwitterMentionSorter() {}
	bool operator()(const rtt::Tweet& a, const rtt::Tweet& b) {
		return a.created_at_timestamp > b.created_at_timestamp;
	}
};

struct TwitterCommand {
	
	TwitterCommand(rtt::Tweet tweet, set<string> tokens)	
		:tweet(tweet)
		,tokens(tokens)
	{
	}
	
	void print() {
		set<string>::iterator it = tokens.begin();
		while(it != tokens.end()) {
			printf("%s\n", (*it).c_str());
			++it;
		}
	}
	
	rtt::Tweet tweet;
	set<string> tokens;
};


#endif
