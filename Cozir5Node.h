/**
 * \file Cozir5Node.h
 *
 *  Created by Scott Erholm on 7/08/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 */

#ifndef __Cozir5Node__
#define __Cozir5Node__

#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>
#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include "Cozir5Drv.h"

namespace cozir5 {
    
class Cozir5Node : public node::ObjectWrap {
 
public:
    static void Init(v8::Local<v8::Object> exports);
    
    static void getDeviceName(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getDeviceVersion(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getDeviceNumValues (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getTypeAtIndex (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getNameAtIndex (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void isDeviceActive (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getValueAtIndexSync (const v8::FunctionCallbackInfo<v8::Value>& args);
    static void getValueAtIndex (const v8::FunctionCallbackInfo<v8::Value>& args);
    
private:
    
    explicit Cozir5Node(std::string devfile = "/dev/ttyO2") {}
    
    ~Cozir5Node() {}
    
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    
    static void WorkAsync(uv_work_t *req);
    static void WorkAsyncComplete(uv_work_t *req,int status);
    
    static v8::Persistent<v8::Function> constructor;
    
    static Cozir5Drv *driver;
    
    struct Work {
        uv_work_t  request;
        v8::Persistent<v8::Function> callback;
        
        int valueIndex;
        std::string value;
    };

    
};

    
} // namespace


#endif /* defined(__Cozir5Node__) */
