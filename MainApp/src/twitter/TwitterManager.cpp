#include "TwitterManager.h"
#include "AppAssets.h"

TwitterManager::TwitterManager()
	:simulator(*this)
	,verbose(false)
{
}

TwitterManager::~TwitterManager() {
}

void TwitterManager::init() {
	// simulator + remote gui
	simulator.setup(ofToDataPath("twitter/simulator.ini",true));
		
	// bad words
	reloadBadWords();
	
	// alllowed commands.
	allowed_commands.setup(
		ofToDataPath("twitter/allowed_commands.txt", true)
		,ofToDataPath("twitter/allowed_colours.xml", true)
	);
	allowed_commands.load();
	
	// load messages we send when someone is not using a command
	if(!loadUnhandledCommandMessages(ofToDataPath("twitter/no_command_found_replies.txt",true))) {
		printf("Error: cannot load the twitter/no_command_found_replies.txt file. Please update git or create it.\n");
		exit(0);
	}
	
	// twitter mentions
#ifndef USE_TEST_TWITTERACCOUNT
	twitter_user = "thehellocube";
#else
	twitter_user = "roxlutest";
#endif

	string tokens_file = ofToDataPath("twitter/twitter_" +twitter_user +".txt",true);
	twitter_thread.setup(tokens_file);
	twitter_thread.startThread(false, false);
	
	// file with sentences we use for replies.
	string replies_file = ofToDataPath("twitter/reply_beginnings.txt",true); 
	
	// photo uploader + reply
	uploader_thread.setup(
		 twitter_thread.getTwitter().getConsumerKey()
		,twitter_thread.getTwitter().getConsumerSecret()
		,tokens_file
		,replies_file
	);
	uploader_thread.setReplyEndings(allowed_commands.getCommands());	
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

void TwitterManager::parseTweet(rtt::Tweet& tweet, bool isFake) {
	string command;
	string lower = tweet.getText();
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	lower = trim(lower);
	
	string bw = "";
	if(bad_words.containsBadWord(lower, bw)) {
		printf("Found bad word: %s in:%s\n", bw.c_str(), lower.c_str());
		return;
	}
	
	if(verbose) {
		printf("Mention: %s - screenname: %s\n", lower.c_str(), tweet.getScreenName().c_str());
	}
		
	// Check if it's a correct search term:
	string match = "^@" +twitter_user +" (.*)$";
	pcrecpp::RE re(match.c_str());
	re.FullMatch(lower, &command);
	if(command.length()) {
		StringTokenizer tokens(command, " ",Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		if(tokens.count() > 0) {
			set<string> tokens_copy(tokens.begin(), tokens.end());
			set<string> found_commands;
			set<string> found_scenes;
			map<string, ofColor> found_colours;
			
			bool must_handle = allowed_commands.filterCommands(
				 tokens_copy
				,found_commands
				,found_colours
				,found_scenes
			);
			
			if(verbose) {
				printf("Handle tweet: %c\n", (must_handle) ? 'y' : 'n');
			}
			
			if(must_handle) {
				if(tweet.getScreenName() == twitter_user) {
					if(verbose) {
						printf("We're not handling this command because it's send from the same account as we're using to fetch mentions. This would result in a infinite loop\n");
					}
				}
				else {
					TwitterCommand cmd(
						 tweet
						,found_commands
						,found_colours
						,found_scenes
						,isFake
					);
					if(verbose) {
						cmd.print();
					}
					commands.push(cmd);
				}
			}
			else {
				size_t num_messages = unhandled_commands_messages.size();
				unsigned int rnd_dx = (rand() * num_messages) % num_messages;
				string rnd_message = "@" +tweet.getScreenName() +" " +unhandled_commands_messages[rnd_dx];
				uploader_thread.sendMessage(rnd_message, tweet);
				if(verbose) {
					printf("Tweet not handled, but we send back a general message: %s.\n", rnd_message.c_str());
				}
			}
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

bool TwitterManager::loadUnhandledCommandMessages(const string& filepath) {
	std::ifstream ifs(filepath.c_str());
	if(!ifs.is_open()) {
		return false;
	}
	string msg;
	while(std::getline(ifs, msg)) {
		printf("Loaded mesage: %s\n", msg.c_str());
		unhandled_commands_messages.push_back(msg);
	}
	return true;
}
