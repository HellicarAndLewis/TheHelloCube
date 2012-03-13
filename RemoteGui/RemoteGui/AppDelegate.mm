#import "AppDelegate.h"


@implementation AppDelegate

@synthesize plot;
@synthesize window = _window;
@synthesize fake_tweet;
@synthesize send_tweet_button;
@synthesize reload_commands_button;
@synthesize reload_badwords_button;
@synthesize shake_amplitude;
@synthesize shake_number;
@synthesize shake_button;
@synthesize shake_duration;
@synthesize swirl_radius;
@synthesize swirl_angle;

- (void)dealloc {
    [super dealloc];
}
	
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	oscu = [[Osculator alloc] init];
	[oscu generateFromWindow:[self window] port:4000];
	[fake_tweet setStringValue:@"@thehellocube"];
	[oscu sendSliderChanged:"fx_swirl_radius" sender:swirl_radius];
	[oscu sendSliderChanged	:"fx_swirl_angle" sender:swirl_angle];
	[self updateShake];
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

- (IBAction)onShakeAmplitude:(id)sender {
	[self updateShake];	
}

- (IBAction)onShakeNumber:(id)sender {
	[self updateShake];
}

- (IBAction)onShakeButton:(id)sender {
	[oscu sendSliderChanged:"fx_shake_duration" sender:shake_duration];
	[oscu sendSliderChanged:"fx_shake_number" sender:shake_number];
	[oscu sendSliderChanged:"fx_shake_amplitude" sender:shake_amplitude];
	[oscu sendButtonChanged:"shake_fx" sender:shake_button];
}

-(void) updateShake {
	float amp = [shake_amplitude floatValue]/[shake_amplitude maxValue];
	[plot set:amp number:[shake_number floatValue]];
	[plot setNeedsDisplay:YES];

}
@end
