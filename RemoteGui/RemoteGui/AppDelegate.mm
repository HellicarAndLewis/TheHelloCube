#import "AppDelegate.h"


@implementation AppDelegate

@synthesize window = _window;
@synthesize fake_tweet;
@synthesize send_tweet_button;
@synthesize reload_commands_button;
@synthesize reload_badwords_button;

- (void)dealloc {
    [super dealloc];
}
	
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	oscu = [[Osculator alloc] init];
	[oscu generateFromWindow:[self window] port:4000];
	[fake_tweet setStringValue:@"@thehellocube"];
}

- (IBAction)onSendFakeTweet:(id)sender {
	[oscu sendTextfieldChanged:"fake_tweet" sender:fake_tweet];
	[oscu sendButtonChanged:"simulate_tweet" sender:send_tweet_button];
	
}

- (IBAction)onReloadCommands:(id)sender {
	[oscu sendButtonChanged:"reload_commands" sender:reload_commands_button];
}

- (IBAction)onReloadBadWords:(id)sender {
	[oscu sendButtonChanged:"reload_badwords" sender:reload_badwords_button];
}
@end
