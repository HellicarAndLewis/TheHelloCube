/**
 * Checks for new mentions with commands we need to parse.
 * 
 * 
 * 
 * @date	2012.03.xx
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 */
#ifndef TWITTER_THREADH
#define TWITTER_THREADH

#include "ofMain.h"
#include "Twitter.h"
#include "TwitterTypes.h"
#include <sstream>


using std::string;

class TwitterThread : public ofThread {
public:	
	TwitterThread();
	~TwitterThread();
	void setup(const string& tokensFile);
	void threadedFunction();
	bool copyMentions(vector<rtt::Tweet>& mentions);
	rt::Twitter& getTwitter();
	string getTokensFile();
	void setVerbose(bool v);
private:
	void initializeTwitter();
	void fetchMentions();
	
	rt::Twitter twitter;
	string tokens_file;
	bool verbose;
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
	
};

inline rt::Twitter& TwitterThread::getTwitter() {
	return twitter;
}

inline string TwitterThread::getTokensFile() {
	return tokens_file;
}

inline void TwitterThread::setVerbose(bool v) {
	verbose = v;
}
#endif