﻿// Ignore error below
#include <ScriptX/ScriptX.h>
#include <API/APIhelp.h>
#include <API/EngineGlobalData.h>
#include <API/EngineOwnData.h>
#include <Kernel/Db.h>
#include <Kernel/System.h>
#include <windows.h>
#include <string>
#include <exception>
#include <thread>
#include <chrono>
#include <memory>
#include <filesystem>
#include <Configs.h>
using namespace script;
using namespace std;

//主引擎表
std::list<std::shared_ptr<ScriptEngine>> lxlModules;
// 配置文件
INI_ROOT iniConf;
// 日志等级
int lxlLogLevel = 1;
// 国际化
LangPack LangP;

extern void LoadBaseLib();
extern void LoadDepends();
extern void LoadPlugins();
extern void BindAPIs(std::shared_ptr<ScriptEngine> engine);
extern void LoadDebugEngine();

void Welcome()
{
    cout << R"(     _       _  _         __   __  _                        _             )" << endl;
    cout << R"(    | |     (_)| |        \ \ / / | |                      | |            )" << endl;
    cout << R"(    | |      _ | |_  ___   \ V /  | |      ___    __ _   __| |  ___  _ __ )" << endl;
    cout << R"(    | |     | || __|/ _ \   > <   | |     / _ \  / _` | / _` | / _ \| '__|)" << endl;
    cout << R"(    | |____ | || |_|  __/  / . \  | |____| (_) || (_| || (_| ||  __/| |   )" << endl;
    cout << R"(    |______||_| \__|\___| /_/ \_\ |______|\___/  \__,_| \__,_| \___||_|   )" << endl;

    cout << "\n\n      =========   LiteXLoader Script Plugin Loader   =========\n" << endl;
}

void LoaderInfo()
{
    INFO(std::string("LXL for ") + LXL_SCRIPT_LANG_TYPE + " loaded");
    INFO(std::string("Version ") + to_string(LXL_VERSION_MAJOR) + "." + to_string(LXL_VERSION_MINOR) + "."
        + to_string(LXL_VERSION_BUILD) + (LXL_VERSION_IS_BETA ? " Beta" : ""));
}

void entry()
{
    Raw_DirCreate(std::filesystem::path(LXL_CONFIG_PATH).remove_filename().u8string());
    iniConf = Raw_IniOpen(LXL_CONFIG_PATH);
    if (!iniConf)
        ERROR(_TRS("init.loadConfig.fail"));
    lxlLogLevel = Raw_IniGetInt(iniConf,"Main","LxlLogLevel",1);

    //初始化全局数据
    bool isFirstInstance;
    InitEngineGlobalData(&isFirstInstance);

    //欢迎
    if(isFirstInstance)
        Welcome();
    LoaderInfo();

    //国际化
    LangP.load(LXL_LANGPACK_DIR + Raw_IniGetString(iniConf, "Main", "Language", "en_US") + ".json");

    //初始化经济系统
    Raw_InitEcnonmicSystem();

    //预加载库
    LoadBaseLib();
    LoadDepends();
    
    //加载插件
    LoadPlugins();

    //注册后台调试
    LoadDebugEngine();

    //GC循环
    int gcTime = Raw_IniGetInt(iniConf, "Advanced", "GCInterval", 20);
    std::thread([gcTime]() {
        std::this_thread::sleep_for(std::chrono::seconds(gcTime));
        for (auto engine : lxlModules)
        {
            EngineScope enter(engine.get());
            engine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopOnce);
        }
    }).detach();    //############## loadPlugin加锁 ################

    Raw_IniClose(iniConf);
}