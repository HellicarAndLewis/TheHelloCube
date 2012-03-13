#import "Plot.h"

@implementation Plot

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
	}
    
    return self;
}

- (void)set:(float)amp number:(float)num {
	amplitude = amp;
	number = num;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // background
	[[NSColor whiteColor] setFill];
	NSRectFill(dirtyRect);
	
	float steps = 1000;
	float width = dirtyRect.size.width;
	float height = dirtyRect.size.height;
	float hy = height * 0.5;
	float step_x = width/steps;
	float amp = (amplitude * hy) * 0.9;
	NSPoint p = NSMakePoint(0, hy);
	NSBezierPath* path = [NSBezierPath bezierPath];
	[path setLineWidth:1.0]; 
	[path moveToPoint:p];
	for(float i = 0; i <= steps; ++i) {
		float perc = (i/steps);
		[path lineToPoint:p];
		p.y = hy + cos(perc*number*3.14) * amp * sin(perc*3.14);
		p.x += step_x;
	}
	[path stroke];
}

@end
