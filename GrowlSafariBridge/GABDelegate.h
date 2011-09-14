#import <Growl/GrowlApplicationBridge.h>

@interface GABDelegate : NSObject <GrowlApplicationBridgeDelegate>
- (NSDictionary *)registrationDictionaryForGrowl;
@end
