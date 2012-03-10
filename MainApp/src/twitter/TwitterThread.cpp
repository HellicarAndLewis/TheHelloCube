#include "TwitterThread.h"

TwitterThread::TwitterThread()
	:is_first_request(true)
	,rate_limit(0)
	,rate_reset(0)
	,rate_remaining(0)
	,last_timestamp(0)
{
	mentions_params["count"] = 10;
}

TwitterThread::~TwitterThread() {
}

void TwitterThread::threadedFunction() {
	initializeTwitter();
	
	while(true) {
		fetchMentions();
		sleep(145);
	}
}

void TwitterThread::setup(const string& tokensFile) {
	tokens_file = tokensFile;
}

void TwitterThread::fetchMentions() {
	// FETCH MENTIONS
	// -----------------------
	twitter.statusesMentions(&mentions_params);
	long resp_code = twitter.getHTTPResponseCode();
		
	if(resp_code != 200) {
		printf("mentions: wrong response code: %lu\n", resp_code);		
		return;
	}
	
	queried_mentions.clear();
	newer_mentions.clear();
	twitter.getJSON().parseStatusArray(twitter.getResponse(), queried_mentions);
	std::sort(queried_mentions.begin(), queried_mentions.end(), TwitterMentionSorter());
	
	// filter out mentions which are newer then previous		
	for(int i = 0; i < queried_mentions.size(); ++i) {	
		rtt::Tweet& tw = queried_mentions[i];
		if(tw.created_at_timestamp > last_timestamp) {
			newer_mentions.push_back(tw);
		}
	}
	
	// PARSE RATE LIMITS
	// -----------------------
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
	
	string str_reset;
	std::stringstream ss_reset;
	if(twitter.getResponseHeader("x-ratelimit-reset", str_reset)) {
		ss_reset << str_reset;
		ss_reset >> rate_reset;
	}
	
	if(newer_mentions.size() <= 0) {
		printf("No new mentions found.\n");
		return;
	}


	// STORE NEW MENTIONS
	// ------------------
	last_mention = newer_mentions.at(0);			
	mentions_params["since_id"] = last_mention.getTweetID();
	lock();
		std::copy(newer_mentions.begin(), newer_mentions.end(), std::back_inserter(mentions));
	unlock();

	is_first_request = false;
}

void TwitterThread::initializeTwitter() {
	twitter.setConsumerKey("e0vURm6xhSYaS0nsS97pQ");
	twitter.setConsumerSecret("R7HfL0vgy2FvQsnYaPAaPy1P1QokzeaBSLXCyboNYo");
	
	if(!twitter.loadTokens(tokens_file)) {
		printf("LOAD ...\n");
		string auth_url;
		twitter.requestToken(auth_url);
		twitter.handlePin(auth_url);
		twitter.accessToken();
		printf("oaded..\n");
		twitter.saveTokens(tokens_file);
	}
}


bool TwitterThread::copyMentions(vector<rtt::Tweet>& result) {
	bool found = false;
	lock();
		found = mentions.size() > 0;
	unlock();
	
	if(!found) {
		return false;
	}
	
	lock();
		std::copy(mentions.begin(), mentions.end(), std::back_inserter(result));
		mentions.clear();
	unlock();
	
	return true;
}