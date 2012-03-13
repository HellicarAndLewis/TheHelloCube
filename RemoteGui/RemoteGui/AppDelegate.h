#import <Cocoa/Cocoa.h>
#import "Osculator.h"
#import "Plot.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
	Osculator* oscu;
}
- (IBAction)onSendFakeTweet:(id)sender;
- (IBAction)onReloadCommands:(id)sender;
- (IBAction)onReloadBadWords:(id)sender;
- (IBAction)onShakeAmplitude:(id)sender;
- (IBAction)onShakeNumber:(id)sender;
- (IBAction)onShakeButton:(id)sender;
- (void) updateShake;

@property (assign) IBOutlet Plot *plot;
@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSTextField *fake_tweet;
@property (assign) IBOutlet NSButton *send_tweet_button;
@property (assign) IBOutlet NSButton *reload_commands_button;
@property (assign) IBOutlet NSButton *reload_badwords_button;
@property (assign) IBOutlet NSSlider *shake_amplitude;
@property (assign) IBOutlet NSSlider *shake_number;
@property (assign) IBOutlet NSButton *shake_button;
@property (assign) IBOutlet NSSlider *shake_duration;
@property (assign) IBOutlet NSSlider *swirl_radius;
@property (assign) IBOutlet NSSlider *swirl_angle;
@end
