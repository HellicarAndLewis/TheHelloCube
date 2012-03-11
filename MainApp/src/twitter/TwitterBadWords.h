/**
 * 
 * Used to filter bad words from a tweet.
 * 
 * 
 * @date	2012.03.08
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 */
#ifndef TWITTER_BAD_WORDSH
#define TWITTER_BAD_WORDSH

#include <iterator>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include "pcrecpp.h"

using std::string;
using std::set;


class TwitterBadWords {

public:

	void setBadWords(const set<string>& words);
	bool reloadWordsFile(const string& filePath);
	void cleanup(string& text, const string& replacement);
	bool containsBadWord(const string& text, string& foundWord);
	
private:

	set<string> bad_words;
};

#endif