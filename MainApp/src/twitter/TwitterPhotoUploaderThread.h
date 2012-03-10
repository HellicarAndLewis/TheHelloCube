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


using std::queue;
using std::string;

//const string URL_TWITTER_UPLOADER = "http://hellocube.localhost/images/";
const string URL_TWITTER_UPLOADER = "http://www.thehellocube.com/images/";
enum TwitterPhotoUploaderTaskType {
	TASK_UPLOAD_PHOTO
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


class TwitterPhotoUploaderThread : public ofThread {
public:
	TwitterPhotoUploaderThread();
	~TwitterPhotoUploaderThread();
	void threadedFunction();
	
	void setup(const string& consumerKey, const string& consumerSecret, const string& tokensPath);	
	void uploadScreenshot(unsigned char* pixels, int w, int h, rtt::Tweet tweet);

private:
	rc::Curl uploader_curl;
	rt::Twitter twitter;
	queue<TwitterPhotoUploaderTask*> tasks;
};

#endif