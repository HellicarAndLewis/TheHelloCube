//
//  Plot.h
//  RemoteGui
//
//  Created by Diederick Huijbers on 3/12/12 (12).
//  Copyright (c) 2012 ApolloMedia. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface Plot : NSView {
	float amplitude;
	float number;
}
- (void) set:(float)amplitude number:(float)number;
@end
