#include "TwitterManager.h"

TwitterManager::TwitterManager()
	:simulator(*this)
{
}

TwitterManager::~TwitterManager() {
}

void TwitterManager::init() {
	// simulator + remote gui
	simulator.setup(ofToDataPath("twitter/simulator.ini",true));
	simulator.loadSettings();
	
	// bad words
	reloadBadWords();
	
	// alllowed commands.
	allowed_commands.setup(ofToDataPath("twitter/allowed_commands.txt", true));
	allowed_commands.load();
	
	// twitter mentions
	string tokens_file = ofToDataPath("twitter/twitter_roxlutest.txt",true);
	twitter_thread.setup(tokens_file);
	//twitter_thread.startThread(false, false);
	
	// photo uploader + reply
	uploader_thread.setup(
		twitter_thread.getTwitter().getConsumerKey()
		,twitter_thread.getTwitter().getConsumerSecret()
		,tokens_file
	);
	
	//uploader_thread.startThread(false, false);	
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
	
	string bw = "";
	if(bad_words.containsBadWord(lower, bw)) {
		printf("Found bad word: %s in:%s\n", bw.c_str(), lower.c_str());
		return;
	}
	
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

void TwitterManager::reloadBadWords() {
	bad_words.reloadWordsFile(ofToDataPath("twitter/bad_words.txt", true));
}
