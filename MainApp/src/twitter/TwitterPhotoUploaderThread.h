/**
 * Uploads images to a remote webserver
 * 
 * 
 * 
 * @date	2012.03.xx
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 */
#ifndef TWITTER_PHOTOUPLOADER_THREADH
#define TWITTER_PHOTOUPLOADER_THREADH

#include "Twitter.h"
#include "ofMain.h"
#include "Curl.h"
#include "Request.h"
#include "curl.h"
#include "jansson.h"
#include <string>
#include <queue>
#include <fstream>
#include <set>
#include <vector>

using std::set;
using std::queue;
using std::string;
using std::vector;

//const string URL_TWITTER_UPLOADER = "http://hellocube.localhost/images/";
const string URL_TWITTER_UPLOADER = "http://www.thehellocube.com/images/";
enum TwitterPhotoUploaderTaskType {
	 TASK_UPLOAD_PHOTO
	,TASK_GENERAL_MESSAGE
};

class TwitterPhotoUploaderTask {
public:
	TwitterPhotoUploaderTask(int t)
		:type(t)
	{
	}
	int type;
};

class TwitterPhotoUploaderTask_Upload : public TwitterPhotoUploaderTask {
public:
	TwitterPhotoUploaderTask_Upload(unsigned char* pixels, int w, int h, rtt::Tweet tweet)
		:TwitterPhotoUploaderTask(TASK_UPLOAD_PHOTO)
		,pixels(pixels)
		,w(w)
		,h(h)
		,tweet(tweet)
	{
	}
	
	~TwitterPhotoUploaderTask_Upload() {
		printf("~TwitterPhotoUploaderTask_Upload");
		delete[] pixels;
	}
	
	int w;
	int h;
	unsigned char* pixels;
	rtt::Tweet tweet;
};


class TwitterPhotoUploaderTask_GeneralMessage : public TwitterPhotoUploaderTask {
public:
	TwitterPhotoUploaderTask_GeneralMessage(const string& msg, rtt::Tweet tweet)
		:TwitterPhotoUploaderTask(TASK_GENERAL_MESSAGE)
		,message(msg)
	{
	}
	
	string message;
	rtt::Tweet tweet;
};

class TwitterPhotoUploaderThread : public ofThread {
public:
	TwitterPhotoUploaderThread();
	~TwitterPhotoUploaderThread();
	void threadedFunction();
	
	void setup(
		 const string& consumerKey
		,const string& consumerSecret
		,const string& tokensPath
		,const string& replyBeginningsPath
	);
		
	void uploadScreenshot(unsigned char* pixels, int w, int h, rtt::Tweet tweet);
	void sendMessage(const string& msg, rtt::Tweet);
	void setVerbose(bool v);
	void setReplyEndings(const set<string>& cmd);
	string generateMessage(const string& username, const string& photourl);
private:
	bool loadReplyBeginnings(const string& file);
	bool verbose;
	rc::Curl uploader_curl;
	rt::Twitter twitter;
	queue<TwitterPhotoUploaderTask*> tasks;
	vector<string> reply_beginnings;
	vector<string> reply_endings;
};



#endif