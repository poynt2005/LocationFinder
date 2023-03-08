#include "NodeBinding.h"
#include <string>

Napi::Value Node_Finder(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong Argument, excepted one string").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(env, "Wrong Argument, position 0 excepted one string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto findType = info[0].As<Napi::String>().Utf8Value();
    auto resultArr = Napi::Array::New(env, 0);

    // auto consoleLog = [&env](const Napi::Value &jsElem) -> decltype(auto)
    // {
    //     auto console = env.Global().Get("console").As<Napi::Object>();
    //     auto log = console.Get("log").As<Napi::Function>();
    //     log.Call(env.Global(), {jsElem});
    // };

    auto arrayPush = [&env](const Napi::Array &inputArr, const Napi::Value &jsElem) -> decltype(auto)
    {
        int currentLength = inputArr.Get("length").As<Napi::Number>().Int32Value();
        auto jsPushFuncRawVal = inputArr.Get("push");
        if (!jsPushFuncRawVal.IsFunction())
        {
            return false;
        }

        auto jsPushFunc = jsPushFuncRawVal.As<Napi::Function>();
        auto jsPushFuncExecResult = jsPushFunc.Call(inputArr, {jsElem}).As<Napi::Number>().Int32Value();

        if (jsPushFuncExecResult <= currentLength)
        {
            return false;
        }

        return true;
    };

    InstallPath installPath;

    if (findType == "visual_studio")
    {
        installPath.type = VISUAL_STUDIO;
    }
    else if (findType == "python")
    {
        installPath.type = PYTHON;
    }
    else if (findType == "node_js")
    {
        installPath.type = NODE_JS;
    }
    else
    {
        return resultArr;
    }

    bool isFinderSucceed = static_cast<bool>(GetInstallPath(&installPath));

    if (!isFinderSucceed)
    {
        return resultArr;
    }

    for (int i = 0; i < installPath.versions; i++)
    {
        arrayPush(resultArr, Napi::String::New(env, installPath.path[i]));
    }

    ReleaseInstallPath(&installPath);

    return resultArr;
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "Finder"),
        Napi::Function::New(env, Node_Finder));

    return exports;
}

NODE_API_MODULE(LocationFinder, Initialize)