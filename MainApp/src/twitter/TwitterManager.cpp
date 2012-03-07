#include "TwitterManager.h"

TwitterManager::TwitterManager()
	:simulator(*this)
{
}

TwitterManager::~TwitterManager() {
}

void TwitterManager::init() {
	allowed_commands.setup(ofToDataPath("allowed_commands.txt", true));
	allowed_commands.load();
	string tokens_file = ofToDataPath("twitter.txt",true);
	twitter_thread.setup(tokens_file);
	twitter_thread.startThread(false, false);
	
	uploader_thread.setup(
		twitter_thread.getTwitter().getConsumerKey()
		,twitter_thread.getTwitter().getConsumerSecret()
		,tokens_file
	);
	uploader_thread.startThread(false, false);
}

void TwitterManager::update() {
	simulator.update();
	
	vector<rtt::Tweet> mentions;
	if(twitter_thread.copyMentions(mentions)) {
		vector<rtt::Tweet>::reverse_iterator it = mentions.rbegin();
		while(it != mentions.rend()) {
			rtt::Tweet& tweet = *it;	
			parseTweet(tweet);
			++it;
		}
	}
}

void TwitterManager::parseTweet(rtt::Tweet& tweet) {
	string command;
	string lower = tweet.getText();
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	
	lower = trim(lower);
	printf("Mention: %s\n", lower.c_str());

	// Check if it's a correct search term:
	pcrecpp::RE re("^@thehellocube (.*)$");
	re.FullMatch(lower, &command);
	if(command.length()) {
		StringTokenizer tokens(command, " ",Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		if(tokens.count() > 0) {
			set<string> tokens_copy(tokens.begin(), tokens.end());
			set<string> filtered;
			allowed_commands.filterCommands(tokens_copy, filtered);
			TwitterCommand cmd(tweet, filtered);
			commands.push(cmd);
		}
	}
}

bool TwitterManager::hasNewCommands() {
	return commands.size() > 0;
}

bool TwitterManager::getNextCommand(TwitterCommand& result)  {	
	if(!hasNewCommands()) {
		return false;
	}	
	result = commands.front();
	commands.pop();
	return true;
}
