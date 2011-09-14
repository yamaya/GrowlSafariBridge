#include "GrowlSafariBridgePlugin.hpp"
#include "GrowlObject.hpp"

#pragma mark <<< GrowlObject Glue >>>

static NPObject * AllocateGrowlObject(NPP npp, NPClass * unused)
{
	return new GrowlObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(GrowlObject, AllocateGrowlObject);

#pragma mark <<< GrowlSafariBridgePlugin >>>

GrowlSafariBridgePlugin::GrowlSafariBridgePlugin(NPP npp) :
	npp_(npp),
	scriptable_object_(NULL)
{
}

GrowlSafariBridgePlugin::~GrowlSafariBridgePlugin()
{
	if (scriptable_object_ != NULL) {
		NPN_ReleaseObject(scriptable_object_);
	}
}

NPBool GrowlSafariBridgePlugin::init(NPWindow * aWindow)
{
	// 特にやることなし
	return true;
}

void GrowlSafariBridgePlugin::shut()
{
	// 特にやることなし
}

NPBool GrowlSafariBridgePlugin::isInitialized()
{
	return true;
}

NPError GrowlSafariBridgePlugin::GetValue(NPPVariable variable, void * value)
{
	NPError error = NPERR_NO_ERROR;

	switch (variable) {
		case NPPVpluginScriptableNPObject:
			*(NPObject **)value = GetScriptableObject();
			break;
		case NPPVpluginDescriptionString: {
			description_.assign("GrowlSafariBridge enables any javascript to notify via Growl.\n");
			description_.append("Growl is ");
			if (!GrowlObject::isGrowlRunning()) {
				description_.append(" not ");
			}
			description_.append("running.");
			*((char const **)value) = description_.c_str();
			break;
		}
		default:
			error = super::GetValue(variable, value);
	}

	return error;
}

NPObject * GrowlSafariBridgePlugin::GetScriptableObject()
{
	if (scriptable_object_ == NULL) {
		scriptable_object_ = NPN_CreateObject(npp_, GET_NPOBJECT_CLASS(GrowlObject));
	}
	if (scriptable_object_ != NULL) {
		NPN_RetainObject(scriptable_object_);
	}
	return scriptable_object_;
}

#pragma mark <<< Plugin Global Functions >>>

NPError NS_PluginInitialize()
{
	return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{
}

char * NS_GetMIMEDescription()
{
	return NULL;
}

nsPluginInstanceBase * NS_NewPluginInstance(nsPluginCreateData * cd)
{
	if (cd == NULL) {
		return NULL;
	}

	GrowlSafariBridgePlugin * plugin = new GrowlSafariBridgePlugin(cd->instance);
	NPN_SetValue(cd->instance, NPPVpluginWindowBool, NULL);
	return plugin;
}

void NS_DestroyPluginInstance(nsPluginInstanceBase * plugin)
{
	delete (GrowlSafariBridgePlugin *)plugin;
}
