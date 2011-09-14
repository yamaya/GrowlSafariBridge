#pragma once
#include "pluginbase.h"
#include <string>

class GrowlSafariBridgePlugin : public nsPluginInstanceBase
{
	typedef nsPluginInstanceBase	super;
	NPP npp_;
	NPObject * scriptable_object_;
	std::string description_;
public:
	explicit GrowlSafariBridgePlugin(NPP npp);
	~GrowlSafariBridgePlugin();

	virtual NPBool init(NPWindow * aWindow);
	virtual void shut();
	virtual NPBool isInitialized();

	virtual NPError GetValue(NPPVariable variable, void * value);

protected:
	NPObject * GetScriptableObject();
};
