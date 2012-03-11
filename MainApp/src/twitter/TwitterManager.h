#ifndef TWITTER_MANAGERH
#define TWITTER_MANAGERH

#include "TwitterTypes.h"
#include "TwitterThread.h"
#include "TwitterSimulator.h"
#include "TwitterCommands.h"
#include "TwitterPhotoUploaderThread.h"
#include "TwitterBadWords.h"
#include "pcrecpp.h"
#include <vector>
#include <queue>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>

using std::queue;
using std::vector;
using Poco::trim;
using Poco::StringTokenizer;

class TwitterManager {
public:

	TwitterManager();
	~TwitterManager();
	void init();
	void update();
	void parseTweet(rtt::Tweet& tweet);
	bool hasNewCommands();
	bool getNextCommand(TwitterCommand& result);
	void reloadBadWords();
	TwitterThread& getThread();
	TwitterCommands& getCommands();
	TwitterPhotoUploaderThread& getUploader();
	TwitterSimulator& getSimulator();
	TwitterBadWords& getBadWords();

private:
	TwitterCommands allowed_commands;
	TwitterSimulator simulator;
	TwitterThread twitter_thread;
	TwitterPhotoUploaderThread uploader_thread;
	TwitterBadWords bad_words;
	queue<TwitterCommand> commands;
	string twitter_user;
};

inline TwitterThread& TwitterManager::getThread() {
	return twitter_thread;
}

inline TwitterCommands& TwitterManager::getCommands() {
	return allowed_commands;
}

inline TwitterPhotoUploaderThread& TwitterManager::getUploader() {
	return uploader_thread;
}

inline TwitterSimulator& TwitterManager::getSimulator() {
	return simulator;
}

inline TwitterBadWords& TwitterManager::getBadWords() {
	return bad_words;
}

/*
mkdir TheHelloCube
git clone git@github.com:roxlu/openFrameworks.git .
git remote add upstream https://github.com/openframeworks/openFrameworks.git
git fetch upstream

# THIS WILL LOOSE ALL YOUR LOCAL CHANGES!!!
git reset --hard upstream/develop

cd TheHelloCube 
git submodule add -f git@github.com:vanderlin/TheHelloCube.git apps/TheHelloCube
git submodule init  # initializes repository
git submodule update # clones the other repository
 
git submodule add -f git@github.com:roxlu/roxlu.git apps/TheHelloCube/addons/roxlu


from openFrameworks/
cd projects
cd TheHelloCube
git submodule add git@github.com:roxlu/roxlu.git projects/TheHelloCube/addons/roxlu

*/
#endif