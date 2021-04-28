﻿// Ignore error below
#include "ScriptX.h"
using namespace script;
#include "LiteLoader/headers/lbpch.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <filesystem>
#include <exception>
#include <thread>
#include <chrono>
#include "Configs.h"
#include "API/APIhelp.h"
#include "API/ConfigHelp.h"
#include "API/BaseAPI.h"

using EnginePtr = std::shared_ptr<ScriptEngine>;
//主引擎表
std::list<EnginePtr> modules;
//消息分发引擎
std::shared_ptr<ScriptEngine> globalEngine;
//调试引擎
std::shared_ptr<ScriptEngine> debugEngine;
bool globalDebug = false;
//前置声明
void LoadScript(const std::string& path);
void BindAPIs(EnginePtr engine);
void RegisterDebug(EnginePtr engine);
void InitGlobalData();

void entry()
{
    INFO(std::string("====== LiteXLoader Script Plugin Loader for ") + LXL_SCRIPT_LANG_TYPE +" ======");
    INFO(std::string("Version ") + LXL_VERSION);
    std::filesystem::directory_iterator files
        (conf::getString("Main","PluginsDir",LXL_DEF_LOAD_PATH));

    INFO("Loading plugins...");
    for (auto& i : files) {
        if (i.is_regular_file() && i.path().extension() == LXL_PLUGINS_SUFFIX)
        {
            try
            {
                LoadScript(i.path().string());
                INFO(i.path().filename().string() + " loaded.");
            }
            catch(Exception& e)
            {
                EngineScope enter(modules.back().get());
                ERROR("Fail to load " + i.path().filename().string() + "!\n");
                ERRPRINT(e);
                //modules.pop_back();
            }
            catch(std::exception& e)
            {
                ERROR("Fail to load " + i.path().filename().string() + "!");
                ERROR(e.what());
            }
            catch(...)
            {
                ERROR("Fail to load " + i.path().filename().string() + "!");
            }
        }
    }
    InitGlobalData();
}


/////////////////////////////////////////////

void LoadScript(const std::string& filePath)
{
    std::ifstream scriptFile(filePath);
    if(!scriptFile)
    {
        throw std::exception("Fail to open file!");
        return;
    }
    std::string scripts((std::istreambuf_iterator<char>(scriptFile)),
        std::istreambuf_iterator<char>());
    scriptFile.close();

    //启动引擎
    EnginePtr engine = NewEngine();
    modules.push_back(engine);
    EngineScope enter(engine.get());

    //绑定API
    try{
        BindAPIs(engine);
    }
    catch(Exception& e)
    {
        ERROR("Fail in Binding APIs!\n");
        throw;
    }

    //加载基础库
    std::ifstream scriptBaseLib(LXL_SCRIPT_BASE_LIB_PATH);
    if(scriptBaseLib)
    {
        std::string baseLibs((std::istreambuf_iterator<char>(scriptBaseLib)),
            std::istreambuf_iterator<char>());
        scriptBaseLib.close();
        engine->eval(baseLibs);
    }
    //加载脚本
    engine->eval(scripts);
}

void InitGlobalData()
{
    // 主消息循环
    globalEngine = NewEngine();
    std::thread([]() {
        EngineScope enter(globalEngine.get());
        globalEngine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopAndWait);
    }).detach();

    // GC循环
    std::thread([]() {
        std::this_thread::sleep_for(std::chrono::seconds(conf::getInt("Advanced","GCInterval",20)));
        for(auto engine : modules)
        {
            EngineScope enter(engine.get());
            engine->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopOnce);
        }
    }).detach();

    //后台调试
    debugEngine = NewEngine();
    EngineScope enter(debugEngine.get());
    try{
        BindAPIs(debugEngine);
    }
    catch(Exception& e)
    {
        ERROR("Fail in Binding APIs!\n");
        throw;
    }
}