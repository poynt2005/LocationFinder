#ifndef __NODE_BINDING_H__
#define __NODE_BINDING_H__

#include <LocationFinder.h>
#include <Napi.h>

Napi::Value Node_Finder(const Napi::CallbackInfo &);
Napi::Object Initialize(Napi::Env, Napi::Object);

#endif