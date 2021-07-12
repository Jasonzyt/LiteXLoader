#include "APIhelp.h"
#include "LxlAPI.h"
#include "EngineGlobalData.h"
#include "EngineOwnData.h"
#include <string>
#include <filesystem>
using namespace std;
using namespace script;

Local<Value> LxlGetVersion(const Arguments& args)
{
    try{
        Local<Object> ver = Object::newObject();
        ver.set("major", LXL_VERSION_MAJOR);
        ver.set("minor", LXL_VERSION_MINOR);
        ver.set("build", LXL_VERSION_BUILD);
        ver.set("isBeta", LXL_VERSION_IS_BETA);
        return ver;
    }
    CATCH("Fail in LxlGetVersion!")
}


extern void LoadScriptFile(const std::string& filePath);

Local<Value> LxlLoadPlugin(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    string newPlugin;
    try
    {
        newPlugin = args[0].toStr();
        if(!filesystem::path(newPlugin).has_filename())
        {
            ERROR("Fail to load " + newPlugin + "!\n");
            return Boolean::newBoolean(false);
        }

        LoadScriptFile(newPlugin);
        return Boolean::newBoolean(true);
    }
    catch(Exception& e)
    {
        EngineScope enter(lxlModules.back().get());
        ERROR("Fail to load " + newPlugin + "!\n");
        ERRPRINT(e);
        ExitEngineScope exit;
        lxlModules.pop_back();
    }
    catch(std::exception& e)
    {
        ERROR("Fail to load " + newPlugin + "!");
        ERROR(e.what());
    }
    catch(...)
    {
        ERROR("Fail to load " + newPlugin + "!");
    }
    return Boolean::newBoolean(false);
}

Local<Value> LxlListPlugins(const Arguments& args)
{
    try
    {
        Local<Array> plugins = Array::newArray();
        for(auto engine : lxlModules)
        {
            plugins.add(String::newString(ENGINE_GET_DATA(engine)->pluginName));
        }
        return plugins;
    }
    CATCH("Fail in LxlListPlugins!")
}