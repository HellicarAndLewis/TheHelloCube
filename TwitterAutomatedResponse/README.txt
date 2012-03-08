TwitterAutomatedResponse
------------------------
When this application is running it will send an message to the user who
mentions the user who signed the user. With "signed" I mean the proces of 
authorizing a twitter account to send messages on their behalf.

How to sign the application for a Twitter account
--------------------------------------------------
We store the information related to the authorization in "twitter_thehellocube.txt".
These steps below show you how to sign for i.e. roxlutest:

1. Open the project 
2. in testApp::setup(), change the name of the tokens_file; i.e. "twitter_roxlutest.txt"
3. make sure there is no file already in data/ with the same name
4. run the application

5. the application opens a browser where you need to sign in with your twitter 
account. Because I'm signing the application as "roxlutest" I login as roxlutest.

6. after pushing the "Authorize" button you see a numerical code. You need to
enter this code after the colon in the console of XCode. After entering the
code press enter.

8. close & reopen the application again.

