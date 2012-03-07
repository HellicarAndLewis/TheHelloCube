#include "TwitterPhotoUploaderThread.h"
TwitterPhotoUploaderThread::TwitterPhotoUploaderThread() {
}

TwitterPhotoUploaderThread::~TwitterPhotoUploaderThread() {
	lock();
	
	//queue<TwitterPhotoUploaderTask*>::iterator it = tasks.begin();
	while(!tasks.empty()) {
		TwitterPhotoUploaderTask* task = tasks.front();
		tasks.pop();
		delete task;
	}
	
	unlock();
}


void TwitterPhotoUploaderThread::setup(
					 const string& consumerKey
					,const string& consumerSecret
					,const string& tokensPath
) 
{
	twitter.setConsumerKey(consumerKey);
	twitter.setConsumerSecret(consumerSecret);
	if(!twitter.loadTokens(tokensPath)) {
		printf("Error: cannot load tokens file for photo uploader: '%s'...\n", tokensPath.c_str());
		exit(0);
	}
}

void TwitterPhotoUploaderThread::threadedFunction() {
	TwitterPhotoUploaderTask* task = NULL;
	
	while(true) {
		lock();
		bool has_tasks = tasks.size() > 0;
		unlock();
		
		if(has_tasks) {
			lock();
				task = tasks.front();
				tasks.pop();
			unlock();
			
			if(task->type == TASK_UPLOAD_PHOTO) {
			
				string dirname = "thumbs/" +ofGetTimestampString("%m-%d");
				ofDirectory dir(dirname);
				dir.create(true);

				string filename = ofGetTimestampString() +"_" +ofToString(ofGetFrameNum()) +".jpg";
				string filepath(dirname);	
				filepath.append("/");
				filepath.append(filename);

				TwitterPhotoUploaderTask_Upload* up = static_cast<TwitterPhotoUploaderTask_Upload*>(task);
				
				ofImage img;
				img.setUseTexture(false);
				img.setFromPixels(up->pixels, up->w, up->h, OF_IMAGE_COLOR);
				img.saveImage(filepath);
				
				rc::Request req(URL_TWITTER_UPLOADER);
				req.getParams().addString("act", "upload");
				req.getParams().addFile("photo", ofToDataPath(filepath, false));
				//req.getParams().addString("user", username);
				//req.getParams().addString("message", "message");

				string response;
				req.doPost(uploader_curl, response, true);
				printf("Upload response: %s\n", response.c_str());

			}
			delete task;
		}
		sleep(5);
	}
}



void TwitterPhotoUploaderThread::uploadScreenshot(unsigned char* pixels, int w, int h, rtt::Tweet tweet) {
	TwitterPhotoUploaderTask_Upload* up = new TwitterPhotoUploaderTask_Upload(pixels, w, h, tweet);
	lock();
		tasks.push(up);
	unlock();
}
