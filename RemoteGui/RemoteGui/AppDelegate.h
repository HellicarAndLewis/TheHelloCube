#import <Cocoa/Cocoa.h>
#import "Osculator.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
	Osculator* oscu;
}
- (IBAction)onSendFakeTweet:(id)sender;
- (IBAction)onReloadCommands:(id)sender;
- (IBAction)onReloadBadWords:(id)sender;

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSTextField *fake_tweet;
@property (assign) IBOutlet NSButton *send_tweet_button;
@property (assign) IBOutlet NSButton *reload_commands_button;
@property (assign) IBOutlet NSButton *reload_badwords_button;

@end
