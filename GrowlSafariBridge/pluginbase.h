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
 * ***** END LICENSE BLOCK ***** */

#ifndef pluginbase_h_
#define pluginbase_h_

#include "npplat.h"

struct nsPluginCreateData
{
  NPP instance;
  NPMIMEType type; 
  uint16_t mode; 
  int16_t argc; 
  char** argn; 
  char** argv; 
  NPSavedData* saved;
};

class nsPluginInstanceBase
{
public:
  // these three methods must be implemented in the derived
  // class platform specific way
  virtual NPBool init(NPWindow* aWindow) = 0;
  virtual void shut() = 0;
  virtual NPBool isInitialized() = 0;

  // implement all or part of those methods in the derived 
  // class as needed
  virtual NPError SetWindow(NPWindow* pNPWindow)                    { return NPERR_NO_ERROR; }
  virtual NPError NewStream(NPMIMEType type, NPStream* stream, 
                            NPBool seekable, uint16_t* stype)       { return NPERR_NO_ERROR; }
  virtual NPError DestroyStream(NPStream *stream, NPError reason)   { return NPERR_NO_ERROR; }
  virtual void    StreamAsFile(NPStream* stream, const char* fname) { return; }
  virtual int32_t WriteReady(NPStream *stream)                      { return 0x0fffffff; }
  virtual int32_t Write(NPStream *stream, int32_t offset, 
                        int32_t len, void *buffer)                  { return len; }
  virtual void    Print(NPPrint* printInfo)                         { return; }
  virtual uint16_t HandleEvent(void* event)                         { return 0; }
  virtual void    URLNotify(const char* url, NPReason reason, 
                            void* notifyData)                       { return; }
  virtual NPError GetValue(NPPVariable variable, void *value)       { return NPERR_NO_ERROR; }
  virtual NPError SetValue(NPNVariable variable, void *value)       { return NPERR_NO_ERROR; }
};

// creation and destruction of the object of the derived class
nsPluginInstanceBase * NS_NewPluginInstance(nsPluginCreateData * aCreateDataStruct);
void NS_DestroyPluginInstance(nsPluginInstanceBase * aPlugin);

// global plugin initialization and shutdown
NPError NS_PluginInitialize();
void NS_PluginShutdown();

char* NS_GetMIMEDescription();

#endif // pluginbase_h_
