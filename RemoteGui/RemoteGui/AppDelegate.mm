#import "AppDelegate.h"


@implementation AppDelegate

@synthesize plot;
@synthesize window = _window;
@synthesize fake_tweet;
@synthesize send_tweet_button;
@synthesize reload_commands_button;
@synthesize reload_badwords_button;
@synthesize bounce_amplitude;
@synthesize bounce_number;
@synthesize bounce_button;
@synthesize bounce_duration;

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

- (IBAction)onBounceAmplitude:(id)sender {
	[self updateBounce];	
}

- (IBAction)onBounceNumber:(id)sender {
	[self updateBounce];
}

- (IBAction)onBounceButton:(id)sender {
	[oscu sendSliderChanged:"fx_bounde_duration" sender:bounce_duration];
	[oscu sendSliderChanged:"fx_bounce_number" sender:bounce_number];
	[oscu sendSliderChanged:"fx_bounce_amplitude" sender:bounce_amplitude];
	[oscu sendButtonChanged:"bounce_fx" sender:bounce_button];
}

-(void) updateBounce {
	float amp = [bounce_amplitude floatValue]/[bounce_amplitude maxValue];
	[plot set:amp number:[bounce_number floatValue]];
	[plot setNeedsDisplay:YES];

}
@end
