#include "ScriptablePluginObjectBase.h"

class GrowlObject : public ScriptablePluginObjectBase
{
	typedef ScriptablePluginObjectBase super;
	void * delegate_;
public:
	explicit GrowlObject(NPP npp);
	~GrowlObject();

	virtual bool HasMethod(NPIdentifier identifier);
	virtual bool Invoke(NPIdentifier identifier, NPVariant const * args, uint32_t argCount, NPVariant * result);

	static bool isGrowlRunning();

protected:
	void notify(NPString const& title, NPString const& description, NPObject const * options);
};
