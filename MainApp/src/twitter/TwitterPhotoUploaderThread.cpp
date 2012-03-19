#include "TwitterPhotoUploaderThread.h"
TwitterPhotoUploaderThread::TwitterPhotoUploaderThread()
	:verbose(false)
{
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
				// CREATE IMAGE FILE 
				// -----------------
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


				// UPLOAD IMAGE
				// ------------
				rc::Request req(URL_TWITTER_UPLOADER);
				req.getParams().addString("act", "upload");
				req.getParams().addFile("photo", ofToDataPath(filepath, false));

				string response;
				req.doPost(uploader_curl, response, true);
				if(verbose) {
					printf("Upload response: %s\n", response.c_str());
				}
				
				
				// PARSE RESULT
				// ------------
				bool correctly_uploaded = false;
				string upload_message;
				json_t *root;
				json_error_t error;
				root = json_loads(response.c_str(), 0, &error);
				if(!root) {
					printf("Error: cannot upload image to remote server. The result we get is:\n%s\n", response.c_str());
					json_decref(root);
				}
				else {
					json_t* node = json_object_get(root, "result");
					if(!json_is_boolean(node)) {
						printf("Error: cannot parse the result from the remote server for uploading. \n");
						correctly_uploaded = false;
					}
					else {
						correctly_uploaded = json_is_true(node);
						node = json_object_get(root, "msg");
						upload_message = "no message found.";
						if(json_is_string(node)) {
							upload_message = json_string_value(node);						
						}
					}
				}
				
				if(!correctly_uploaded) {
					printf("Warning: got message form server %s\n", upload_message.c_str());
					printf("Warning: we cannot upload the image; we get an unexpected error from the remote server.\n");
					printf("Warning: we do not remove this item from the queue; we will wait and retry\n");
					ofSleepMillis(5000);
					json_decref(root);
					continue;
				}
				
				// get filepath
				string created_file = "";
				json_t* filepath_node = json_object_get(root, "created_file");
				if(filepath_node != NULL && json_is_string(filepath_node)) {
					created_file = json_string_value(filepath_node);
				}
				
				string photo_url = URL_TWITTER_UPLOADER +"uploads/"  +created_file;					
				string message = "@" +up->tweet.getScreenName() +" your result ... See here " +photo_url;
				if(verbose) {
					printf(">>>>>>> %s\n", message.c_str());
				}
				twitter.statusesUpdate(message);
				
				json_decref(root);
			}
			delete task;
		}
		ofSleepMillis(5000);
	}
}



void TwitterPhotoUploaderThread::uploadScreenshot(unsigned char* pixels, int w, int h, rtt::Tweet tweet) {
	TwitterPhotoUploaderTask_Upload* up = new TwitterPhotoUploaderTask_Upload(pixels, w, h, tweet);
	lock();
		tasks.push(up);
	unlock();
}

void TwitterPhotoUploaderThread::setVerbose(bool v) {
	verbose = v;
}
