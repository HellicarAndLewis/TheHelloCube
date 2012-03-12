#import <Cocoa/Cocoa.h>
#import "Osculator.h"
#import "Plot.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
	Osculator* oscu;
}
- (IBAction)onSendFakeTweet:(id)sender;
- (IBAction)onReloadCommands:(id)sender;
- (IBAction)onReloadBadWords:(id)sender;
- (IBAction)onBounceAmplitude:(id)sender;
- (IBAction)onBounceNumber:(id)sender;
- (IBAction)onBounceUpdate:(id)sender;
- (IBAction)onBounceButton:(id)sender;
- (void) updateBounce;
@property (assign) IBOutlet Plot *plot;
@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSTextField *fake_tweet;
@property (assign) IBOutlet NSButton *send_tweet_button;
@property (assign) IBOutlet NSButton *reload_commands_button;
@property (assign) IBOutlet NSButton *reload_badwords_button;
@property (assign) IBOutlet NSSlider *bounce_amplitude;
@property (assign) IBOutlet NSSlider *bounce_number;
@property (assign) IBOutlet NSButton *bounce_button;
@property (assign) IBOutlet NSSlider *bounce_duration;
@end
