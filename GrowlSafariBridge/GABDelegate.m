#import "GABDelegate.h"
#import "GABConstants.h"

@implementation GABDelegate
- (NSDictionary *)registrationDictionaryForGrowl
{
	NSBundle * bundle = [NSBundle bundleForClass:[self class]];
	NSDictionary * info = [bundle infoDictionary]; 
	NSString * name = [info objectForKey:@"CFBundleExecutable"];
	NSString * identifier = [info objectForKey:@"CFBundleIdentifier"];
	NSString * iconPath = [bundle pathForResource:@"Plugin" ofType:@"icns"];
	NSImage * image = [[[NSImage alloc] initWithContentsOfFile:iconPath] autorelease];

	NSArray * desc = [NSArray arrayWithObject:kGABNotitification];
	return [NSDictionary dictionaryWithObjectsAndKeys:
				desc, GROWL_NOTIFICATIONS_ALL,
				desc, GROWL_NOTIFICATIONS_DEFAULT,
				[image TIFFRepresentation], GROWL_APP_ICON,
				name, GROWL_APP_NAME,
				identifier, GROWL_APP_ID,
				nil];
}

@end

