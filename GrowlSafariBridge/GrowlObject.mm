#include "GrowlObject.hpp"
#include "npruntime.h"
#include <cstring>
#import <Growl/GrowlApplicationBridge.h>
#import "GABConstants.h"
#import "GABDelegate.h"

GrowlObject::GrowlObject(NPP npp) : super(npp), delegate_(NULL)
{
	if (delegate_ == NULL) {
		GABDelegate * delegate = [GABDelegate new];
		[GrowlApplicationBridge setGrowlDelegate:delegate];
		delegate_ = delegate;
	}
	//NSLog(@"isGrowlInstalled:%d", [GrowlApplicationBridge isGrowlInstalled]);
}

GrowlObject::~GrowlObject()
{
	if (delegate_ != NULL) {
		[GrowlApplicationBridge setGrowlDelegate:nil];
		GABDelegate * delegate = reinterpret_cast<GABDelegate *>(delegate_);
		delegate_ = nil;
		[delegate release], delegate = nil;
	}
	//NSLog(@"GrowlObject destructed");
}

bool GrowlObject::HasMethod(NPIdentifier identifier)
{
	char const * method_name = NPN_UTF8FromIdentifier(identifier);
	if (std::strcmp("notify", method_name) == 0) return true;
	return false;
}

bool GrowlObject::Invoke(NPIdentifier identifier, NPVariant const * args, uint32_t argCount, NPVariant * result)
{
	char const * method_name = NPN_UTF8FromIdentifier(identifier);
	if (std::strcmp("notify", method_name) == 0) {
		NPString title = {0};
		NPString description = {0};
		NPObject * options = NULL;
		if (argCount > 0 && NPVARIANT_IS_STRING(args[0])) {
			title = NPVARIANT_TO_STRING(args[0]);
		}
		if (argCount > 1 && NPVARIANT_IS_STRING(args[1])) {
			description = NPVARIANT_TO_STRING(args[1]);
		}
		if (argCount > 2 && NPVARIANT_IS_OBJECT(args[2])) {
			options = NPVARIANT_TO_OBJECT(args[2]);
		}
		notify(title, description, options);
		return true;
	}
	return false;
}

static NSString * NSStringFromNPString(NPString const& nps)
{
	NSData * data = [NSData dataWithBytes:nps.UTF8Characters length:nps.UTF8Length];
	return [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
}

void GrowlObject::notify(NPString const& title, NPString const& description, NPObject const * options)
{
	BOOL sticky = NO;
	int priority = 0; 
	NSData * iconData = nil;
	if (options != NULL) {
		NPVariant variant;
		NPN_GetProperty(npp_, const_cast<NPObject *>(options), NPN_GetStringIdentifier("sticky"), &variant);
		if (NPVARIANT_IS_BOOLEAN(variant)) {
			sticky = NPVARIANT_TO_BOOLEAN(variant);
		}
		variant.type = NPVariantType_Void;
		NPN_GetProperty(npp_, const_cast<NPObject *>(options), NPN_GetStringIdentifier("priority"), &variant);
		if (NPVARIANT_IS_INT32(variant)) {
			priority = NPVARIANT_TO_INT32(variant);
		}
		variant.type = NPVariantType_Void;
		NPN_GetProperty(npp_, const_cast<NPObject *>(options), NPN_GetStringIdentifier("imageUrl"), &variant);
		if (NPVARIANT_IS_STRING(variant)) {
			NPString nps = NPVARIANT_TO_STRING(variant);
			NSURL * url = [NSURL URLWithString:NSStringFromNPString(nps)];
			NSImage * image = [[NSImage alloc] initWithContentsOfURL:url];
			if (image != nil) {
				iconData = [image TIFFRepresentation];
			}
		}
	}
	[GrowlApplicationBridge notifyWithTitle:NSStringFromNPString(title)
                                description:NSStringFromNPString(description)
                           notificationName:kGABNotitification
                                   iconData:iconData
                                   priority:priority
                                   isSticky:sticky
                               clickContext:nil];
	//NSLog(@"Growl '%@' completed", NSStringFromNPString(title));
}

bool GrowlObject::isGrowlRunning()
{
	return [GrowlApplicationBridge isGrowlRunning];
}
