/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * ***** END LICENSE BLOCK ***** */

// Main plugin entry point implementation - exports from the plugin library

#include "npplat.h"
#include "pluginbase.h"

NPNetscapeFuncs * NPNFuncs;

static NPError fillPluginFunctionTable(NPPluginFuncs* aNPPFuncs)
{
	if (!aNPPFuncs)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	// Set up the plugin function table that Netscape will use to call us.
	aNPPFuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
	aNPPFuncs->newp          = NPP_New;
	aNPPFuncs->destroy       = NPP_Destroy;
	aNPPFuncs->setwindow     = NPP_SetWindow;
	aNPPFuncs->newstream     = NPP_NewStream;
	aNPPFuncs->destroystream = NPP_DestroyStream;
	aNPPFuncs->asfile        = NPP_StreamAsFile;
	aNPPFuncs->writeready    = NPP_WriteReady;
	aNPPFuncs->write         = NPP_Write;
	aNPPFuncs->print         = NPP_Print;
	aNPPFuncs->event         = NPP_HandleEvent;
	aNPPFuncs->urlnotify     = NPP_URLNotify;
	aNPPFuncs->getvalue      = NPP_GetValue;
	aNPPFuncs->setvalue      = NPP_SetValue;

	return NPERR_NO_ERROR;
}

NP_EXPORT(NPError)
NP_Initialize(NPNetscapeFuncs* aNPNFuncs)
{
	if (aNPNFuncs == NULL) {
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	std::printf("NP_Initialize version=0x%x\n", aNPNFuncs->version);
	std::fflush(stdout);
	if (HIBYTE(aNPNFuncs->version) > NP_VERSION_MAJOR) {
		return NPERR_INCOMPATIBLE_VERSION_ERROR;
	}

	NPNFuncs = aNPNFuncs;

	return NS_PluginInitialize();
}

NP_EXPORT(NPError)
NP_GetEntryPoints(NPPluginFuncs * pFuncs)
{
	return fillPluginFunctionTable(pFuncs);
}

NP_EXPORT(NPError)
NP_Shutdown()
{
	NS_PluginShutdown();
	return NPERR_NO_ERROR;
}

NP_EXPORT(const char*)
NP_GetMIMEDescription(void)
{
	return NS_GetMIMEDescription();
}

NP_EXPORT(NPError)
NP_GetValue(void *npp, NPPVariable aVariable, void *aValue)
{
	return NPP_GetValue((NPP)npp, aVariable, aValue);
}
