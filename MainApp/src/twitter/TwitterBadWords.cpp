#include "TwitterBadWords.h"


// You can also simply pass a vector of bad words.
void TwitterBadWords::setBadWords(const set<string>& words) {
	bad_words.clear();
	std::copy(words.begin(), words.end(), std::inserter(bad_words, bad_words.begin()));
}

// Use a textfile to load bad words (one per line)
bool TwitterBadWords::reloadWordsFile(const string& filePath) {
	bad_words.clear();
	
	std::ifstream ifs(filePath.c_str());
	if(!ifs.is_open()) {
		printf("Error: cannot open '%s'\n", filePath.c_str());
		return false;
	}	
	
	string line;
	while(std::getline(ifs,line)) {
		if(line.length()) {
			bad_words.insert(line);
		}
	}
	ifs.close();
	return true;
}

// Removes all found instances of bad_words with the given replacement.
void TwitterBadWords::cleanup(string& text, const string& replacement) {
	pcrecpp::RE_Options re_opts;
	re_opts.set_caseless(true).set_extended(false).set_multiline(true);	
	set<string>::iterator it = bad_words.begin();
	while(it != bad_words.end()) {
		pcrecpp::RE(*it, re_opts).Replace(replacement, &text);
		++it;
	}	
}

// Check if the given text contains a bad word.
bool TwitterBadWords::containsBadWord(const string& text, string& foundWord) {
	pcrecpp::RE_Options re_opts;
	re_opts.set_caseless(true)
			.set_extended(false)
			.set_multiline(true);	
	set<string>::iterator it = bad_words.begin();
	while(it != bad_words.end()) {
		if(pcrecpp::RE((*it), re_opts).PartialMatch(text)) {
			foundWord = (*it);
			return true;
		}
		++it;
	}
	return false;
}

