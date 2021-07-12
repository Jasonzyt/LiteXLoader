#include "APIhelp.h"
#include "BaseAPI.h"
#include "PlayerAPI.h"
#include "ItemAPI.h"
#include "GuiAPI.h"
#include "EngineOwnData.h"
#include "EngineGlobalData.h"
#include <Kernel/Player.h>
#include <Kernel/Entity.h>
#include <Kernel/Gui.h>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
using namespace script;

//////////////////// Class Definition ////////////////////

ClassDefine<PlayerClass> PlayerClassBuilder =
    defineClass<PlayerClass>("Player")
        .constructor(nullptr)
        .instanceProperty("name", &PlayerClass::getName)
        .instanceProperty("pos", &PlayerClass::getPos)
        .instanceProperty("realName", &PlayerClass::getRealName)
        .instanceProperty("xuid", &PlayerClass::getXuid)
        .instanceProperty("uuid", &PlayerClass::getUuid)
        .instanceProperty("ip", &PlayerClass::getIP)
        .instanceProperty("permLevel", &PlayerClass::getPermLevel)
        .instanceProperty("gameMode", &PlayerClass::getGameMode)
        .instanceProperty("maxHealth", &PlayerClass::getMaxHealth)
        .instanceProperty("health", &PlayerClass::getHealth)
        .instanceProperty("maxStrength", &PlayerClass::getMaxStrength)
        .instanceProperty("strength", &PlayerClass::getStrength)
        .instanceProperty("inAir", &PlayerClass::getInAir)
        .instanceProperty("sneaking", &PlayerClass::getSneaking)

        .instanceFunction("isOP", &PlayerClass::isOP)
        .instanceFunction("setPermLevel", &PlayerClass::setPermLevel)
        .instanceFunction("setGameMode", &PlayerClass::setGameMode)

        .instanceFunction("runcmd", &PlayerClass::runcmd)
        .instanceFunction("teleport", &PlayerClass::teleport)
        .instanceFunction("kill", &PlayerClass::kill)
        .instanceFunction("kick", &PlayerClass::kick)
        .instanceFunction("disconnect", &PlayerClass::kick)
        .instanceFunction("tell", &PlayerClass::tell)
        .instanceFunction("sendText", &PlayerClass::tell)
        .instanceFunction("getHand", &PlayerClass::getHand)
        .instanceFunction("getOffHand", &PlayerClass::getOffHand)
        .instanceFunction("getPack", &PlayerClass::getPack)
        .instanceFunction("rename", &PlayerClass::rename)
        .instanceFunction("addLevel", &PlayerClass::addLevel)
        .instanceFunction("transServer", &PlayerClass::transServer)
        .instanceFunction("crash", &PlayerClass::crash)

        .instanceFunction("getScore", &PlayerClass::getScore)
        .instanceFunction("setScore", &PlayerClass::setScore)
        .instanceFunction("addScore", &PlayerClass::addScore)
        .instanceFunction("removeScore", &PlayerClass::removeScore)
        .instanceFunction("setSidebar", &PlayerClass::setSidebar)
        .instanceFunction("removeSidebar", &PlayerClass::removeSidebar)
        .instanceFunction("setBossBar", &PlayerClass::setBossBar)
        .instanceFunction("removeBossBar", &PlayerClass::removeBossBar)

        .instanceFunction("sendSimpleForm", &PlayerClass::sendSimpleForm)
        .instanceFunction("sendModalForm", &PlayerClass::sendModalForm)
        .instanceFunction("sendForm", &PlayerClass::sendForm)

        .instanceFunction("setExtraData", &PlayerClass::setExtraData)
        .instanceFunction("getExtraData", &PlayerClass::getExtraData)
        .instanceFunction("delExtraData", &PlayerClass::delExtraData)
        .build();


//////////////////// Classes ////////////////////

//生成函数
Local<Object> PlayerClass::newPlayer(Player *p)
{
    auto newp = new PlayerClass(p);
    return newp->getScriptObject();
}
Local<Object> PlayerClass::newPlayer(WPlayer p)
{
    return PlayerClass::newPlayer(p.v);
}
Player* PlayerClass::extractPlayer(Local<Value> v)
{
    if(EngineScope::currentEngine()->isInstanceOf<PlayerClass>(v))
        return EngineScope::currentEngine()->getNativeInstance<PlayerClass>(v)->get();
    else
        return nullptr;
}

//公用API
Local<Value> GetPlayer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        string target = args[0].toStr();
        auto playerList = Raw_GetOnlinePlayers();
        for(Player *p : playerList)
        {
            if(Raw_GetXuid(p) == target || Raw_GetPlayerName(p) == target)
                return PlayerClass::newPlayer(p);
        }
        return Local<Value>();    //Null
    }
    CATCH("Fail in GetPlayer!")
}

Local<Value> GetOnlinePlayers(const Arguments& args)
{
    try{
        auto players = Raw_GetOnlinePlayers();
        Local<Array> list = Array::newArray();
        for(auto p : players)
            list.add(PlayerClass::newPlayer(p));
        return list;
    }
    CATCH("Fail in GetOnlinePlayers!")
}

Local<Value> Broadcast(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)

    try {
        TextType type = TextType::RAW;
        if (args.size() >= 2 && args[1].isNumber())
        {
            int newType = args[1].asNumber().toInt32();
            if (newType >= 0 && newType <= 9)
                type = (TextType)newType;
        }
        return Boolean::newBoolean(Raw_Broadcast(args[0].toStr(), type));
    }
    CATCH("Fail in Broadcast!")
}

//成员函数
Local<Value> PlayerClass::getName()
{ 
    try{
        return String::newString(Raw_GetPlayerName(player));
    }
    CATCH("Fail in getPlayerName!")
}

Local<Value> PlayerClass::getPos()
{ 
    try{
        return FloatPos::newPos(Raw_GetPlayerPos(player));
    }
    CATCH("Fail in getPlayerPos!")
}

Local<Value> PlayerClass::getXuid()
{
    try{
        return String::newString(Raw_GetXuid(player));
    }
    CATCH("Fail in getXuid!")
}

Local<Value> PlayerClass::getUuid()
{
    try{
        return String::newString(Raw_GetUuid(player));
    }
    CATCH("Fail in getXuid!")
}

Local<Value> PlayerClass::getRealName()
{
    try{
        return String::newString(Raw_GetRealName(player));
    }
    CATCH("Fail in getRealName!")
}

Local<Value> PlayerClass::getIP()
{
    try{
        return String::newString(Raw_GetIP(player));
    }
    CATCH("Fail in GetIP!")
}

Local<Value> PlayerClass::getPermLevel()
{
    try {
        return Number::newNumber(Raw_GetPlayerPermLevel(player));
    }
    CATCH("Fail in getPlayerPermLevel!")
}

Local<Value> PlayerClass::getGameMode()
{
    try {
        return Number::newNumber(Raw_GetGameMode(player));
    }
    CATCH("Fail in getGameMode!")
}

Local<Value> PlayerClass::getSneaking()
{
    try{
        return Boolean::newBoolean(Raw_GetSneaking(player));
    }
    CATCH("Fail in getSneaking!")
}

Local<Value> PlayerClass::getMaxHealth()
{
    try{
        return Number::newNumber(Raw_GetMaxHealth((Actor*)player));
    }
    CATCH("Fail in GetMaxHealth!")
}

Local<Value> PlayerClass::getHealth()
{
    try{
        return Number::newNumber(Raw_GetHealth((Actor*)player));
    }
    CATCH("Fail in GetHealth!")
}

Local<Value> PlayerClass::getMaxStrength()
{
    try {
        return Number::newNumber(Raw_GetMaxHealth((Actor*)player));
    }
    CATCH("Fail in GetMaxHealth!")
}

Local<Value> PlayerClass::getStrength()
{
    try {
        return Number::newNumber(Raw_GetHealth((Actor*)player));
    }
    CATCH("Fail in GetHealth!")
}

Local<Value> PlayerClass::getInAir()
{
    try{
        return Boolean::newBoolean(Raw_GetIsInAir((Actor*)player));
    }
    CATCH("Fail in GetInAir!")
}

Local<Value> PlayerClass::teleport(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    
    try{
        FloatPos *pos = FloatPos::extractPos(args[0]);
        if(!pos)
            return Local<Value>();
        
        return Boolean::newBoolean(Raw_TeleportPlayer(player,*pos));
    }
    CATCH("Fail in TeleportPlayer!")
}

Local<Value> PlayerClass::kill(const Arguments& args)
{
    try{
        return Boolean::newBoolean(Raw_KillPlayer(player));
    }
    CATCH("Fail in KillPlayer!")
}

Local<Value> PlayerClass::isOP(const Arguments& args)
{
    try{
        return Boolean::newBoolean(Raw_GetPlayerPermLevel(player) >= 1);
    }
    CATCH("Fail in IsOP!")
}

Local<Value> PlayerClass::setPermLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kNumber)
    
    try{
        bool res = false;
        int newPerm = args[0].asNumber().toInt32();
        if(newPerm>=0 || newPerm<=4)
            res = Raw_SetPlayerPermLevel(player,newPerm);
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in setPlayerPermLevel!")
}

Local<Value> PlayerClass::setGameMode(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber)

    try {
        bool res = false;
        int newMode = args[0].asNumber().toInt32();
        if (newMode >= 0 || newMode <= 3)
            res = Raw_SetGameMode(player, newMode);
        return Boolean::newBoolean(res);
    }
    CATCH("Fail in setGameMode!")
}

Local<Value> PlayerClass::runcmd(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        return Boolean::newBoolean(Raw_RuncmdAs(player,args[0].toStr()));
    }
    CATCH("Fail in runcmd!")
}

Local<Value> PlayerClass::kick(const Arguments& args)
{
    if(args.size() >= 1)
        CHECK_ARG_TYPE(args[0],ValueKind::kString);

    try{
        string msg="正在从服务器断开连接";
        if(args.size() >= 1)
            msg = args[0].toStr();
        
        return Boolean::newBoolean(Raw_KickPlayer(player,msg));
    }
    CATCH("Fail in KickPlayer!")
}

Local<Value> PlayerClass::tell(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    try{
        TextType type = TextType::RAW;
        if(args.size() >= 2 && args[1].isNumber())
        {
            int newType = args[1].asNumber().toInt32();
            if(newType >= 0 && newType <= 9)
                type = (TextType)newType;
        }
        return Boolean::newBoolean(Raw_Tell(player,args[0].toStr(),type));
    }
    CATCH("Fail in Tell!")
}

Local<Value> PlayerClass::getHand(const Arguments& args)
{
    try{
        return ItemClass::newItem(Raw_GetHand(player));
    }
    CATCH("Fail in GetHand!")
}

Local<Value> PlayerClass::getOffHand(const Arguments& args)
{
    try{
        return ItemClass::newItem(Raw_GetOffHand(player));
    }
    CATCH("Fail in getOffHand!")
}

Local<Value> PlayerClass::getPack(const Arguments& args)
{
    try{
        auto res = Raw_GetPack(player);
        Local<Array> pack = Array::newArray();
        for(ItemStack* item : res)
        {
            pack.add(ItemClass::newItem(item));
        }
        return pack;
    }
    CATCH("Fail in GetPack!")
}

Local<Value> PlayerClass::rename(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        return Boolean::newBoolean(Raw_RenamePlayer(player,args[0].toStr()));
    }
    CATCH("Fail in RenamePlayer!")
}

Local<Value> PlayerClass::addLevel(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 1)
    CHECK_ARG_TYPE(args[0], ValueKind::kNumber)

    try {
        return Boolean::newBoolean(Raw_AddLevel(player, args[0].toInt()));
    }
    CATCH("Fail in addLevel!")
}

Local<Value> PlayerClass::transServer(const Arguments& args)
{
    CHECK_ARGS_COUNT(args, 2)
    CHECK_ARG_TYPE(args[0], ValueKind::kString)
    CHECK_ARG_TYPE(args[1], ValueKind::kNumber)

    try {
        return Boolean::newBoolean(Raw_TransServer(player, args[0].toStr(), (short) args[1].toInt()));
    }
    CATCH("Fail in transServer!")
}

Local<Value> PlayerClass::crash(const Arguments& args)
{
    try {
        return Boolean::newBoolean(Raw_CrashPlayer(player));
    }
    CATCH("Fail in crashPlayer!")
}

Local<Value> PlayerClass::getScore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        return Number::newNumber(Raw_GetScore(player,args[0].toStr()));
    }
    CATCH("Fail in getScore!")
}

Local<Value> PlayerClass::setScore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)
    
    try{
        return Boolean::newBoolean(Raw_SetScore(player,args[0].toStr(),args[1].toInt()));
    }
    CATCH("Fail in getScore!")
}

Local<Value> PlayerClass::addScore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)
    
    try{
        return Boolean::newBoolean(Raw_AddScore(player,args[0].toStr(),args[1].toInt()));
    }
    CATCH("Fail in addScore!")
}

Local<Value> PlayerClass::removeScore(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    
    try{
        return Boolean::newBoolean(Raw_RemoveScore(player,args[0].toStr()));
    }
    CATCH("Fail in removeScore!")
}

Local<Value> PlayerClass::setSidebar(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kObject)
    
    try
    {
        std::vector<std::pair<std::string, int>> data;
        auto source = args[1].asObject();
        auto keys = source.getKeyNames();
        for (auto& key : keys)
        {
            data.push_back(make_pair(key, source.get(key).toInt()));
        }

        return Boolean::newBoolean(Raw_SetSidebar(player,args[0].toStr(),data));
    }
    CATCH("Fail in setSidebar!")
}

Local<Value> PlayerClass::removeSidebar(const Arguments& args)
{
    try{
        return Boolean::newBoolean(Raw_RemoveSidebar(player));
    }
    CATCH("Fail in removeSidebar!")
}

Local<Value> PlayerClass::setBossBar(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kNumber)
    
    try{
        int percent = args[1].toInt();
        if(percent < 0)
            percent = 0;
        else if(percent > 100)
            percent = 100;
        
        float value = (float)percent / 100;
        return Boolean::newBoolean(Raw_SetBossBar(player,args[0].toStr(),value));
    }
    CATCH("Fail in setBossBar!")
}

Local<Value> PlayerClass::removeBossBar(const Arguments& args)
{
    try{
        return Boolean::newBoolean(Raw_RemoveBossBar(player));
    }
    CATCH("Fail in removeBossBar!")
}


Local<Value> PlayerClass::sendSimpleForm(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,4)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    CHECK_ARG_TYPE(args[2],ValueKind::kArray)
    CHECK_ARG_TYPE(args[3],ValueKind::kFunction)

    try
    {   
        auto source = args[2].asArray();
        if (source.size() == 0 || !source.get(0).isString())
            return Local<Value>();

        vector<string> buttons;
        for (int i = 0; i < source.size(); ++i)
            buttons.push_back(source.get(i).toStr());

        int formId = Raw_SendSimpleForm(player, args[0].toStr(), args[1].toStr(), buttons);
        engineGlobalData->formCallbacks[(unsigned)formId] = { EngineScope::currentEngine(),Global<Function>(args[3].asFunction()) };
        //###!!!!####### 全局变量不同步？？ ###!!!!#######

        return Number::newNumber(formId);
    }
    CATCH("Fail in sendSimpleForm!")
}

Local<Value> PlayerClass::sendModalForm(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,5)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kString)
    CHECK_ARG_TYPE(args[2],ValueKind::kString)
    CHECK_ARG_TYPE(args[3],ValueKind::kString)
    CHECK_ARG_TYPE(args[4],ValueKind::kFunction)

    try{
        int formId = Raw_SendModalForm(player, args[0].toStr(), args[1].toStr(), args[2].toStr(), args[3].toStr());
        engineGlobalData->formCallbacks[formId] = { EngineScope::currentEngine(),Global<Function>(args[4].asFunction()) };
        
        return Number::newNumber(formId);
    }
    CATCH("Fail in sendModalForm!")
}

Local<Value> PlayerClass::sendForm(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    //CHECK_ARG_TYPE(args[0],ValueKind::kString)
    CHECK_ARG_TYPE(args[1],ValueKind::kFunction)

    try{
        string data;
        try{
            if(args[0].getKind() == ValueKind::kString)
            {
                // Json格式
                data = JSON_VALUE::parse(args[0].toStr()).get<string>();
            }
            else
            {
                // Form对象
                auto jsonForm = FormClass::extractForm(args[0]);
                if(jsonForm == nullptr)
                    throw "Unknown Type of Parameter!";
                data = jsonForm->dump();
            }
        }catch(...){ 
            ERROR("Fail to parse Json string in sendForm!");
            return Local<Value>();
        }
        
        int formId = Raw_SendCustomForm(player,data);
        engineGlobalData->formCallbacks[formId] = { EngineScope::currentEngine(),Global<Function>(args[1].asFunction()) };
        
        return Number::newNumber(formId);
    }
    CATCH("Fail in sendCustomForm!")
}

Local<Value> PlayerClass::setExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,2)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    string key = args[0].toStr();
    if(key.empty())
        return Boolean::newBoolean(false);
    
    ENGINE_OWN_DATA()->playerDataDB[Raw_GetPlayerName(player) + "-" + key] = args[1];
    return Boolean::newBoolean(true);
}

Local<Value> PlayerClass::getExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    string key = args[0].toStr();
    if(key.empty())
        return Boolean::newBoolean(false);

    try
    {
        return ENGINE_OWN_DATA()->playerDataDB.at(Raw_GetPlayerName(player) + "-" + key).get();
    }
    catch(...)
    {
        return Local<Value>();  //Null
    }
    
}

Local<Value> PlayerClass::delExtraData(const Arguments& args)
{
    CHECK_ARGS_COUNT(args,1)
    CHECK_ARG_TYPE(args[0],ValueKind::kString)

    string key = args[0].toStr();
    if(key.empty())
        return Boolean::newBoolean(false);
    
    ENGINE_OWN_DATA()->playerDataDB.erase(Raw_GetPlayerName(player) + "-" + key);
    return Boolean::newBoolean(true);
}
