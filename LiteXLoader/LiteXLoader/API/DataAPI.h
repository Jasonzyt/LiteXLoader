#pragma once
#include <ScriptX/ScriptX.h>
using namespace script;
#include <Kernel/Data.h>

#include <string>
#include <memory>

//////////////////// Classes ////////////////////

class DbClass : public ScriptClass
{
private:
    DB_ROOT kvdb;

public:
	explicit DbClass(const string &dir);
    ~DbClass();
    static DbClass* constructor(const Arguments& args);

    bool isValid()
    { return kvdb != nullptr; }

    Local<Value> get(const Arguments& args);
    Local<Value> set(const Arguments& args);
    Local<Value> del(const Arguments& args);
    Local<Value> close(const Arguments& args);
    Local<Value> listKey(const Arguments& args);

    //For Compatibility
    static Local<Value> newDb(const string& dir);
};


class ConfBaseClass
{
protected:
    std::string confPath;
    virtual bool flush() = 0;
    virtual bool close() = 0;
    virtual bool reload() = 0;

public:
    explicit ConfBaseClass(const string& dir);

    virtual Local<Value> reload(const Arguments& args) = 0;
    virtual Local<Value> close(const Arguments& args) = 0;
    Local<Value> getPath(const Arguments& args);
    Local<Value> read(const Arguments& args);
    virtual Local<Value> write(const Arguments& args) = 0;
};


class ConfJsonClass : public ScriptClass, public ConfBaseClass
{
private:
    JSON_ROOT jsonConf;
    bool flush() override;
    bool close() override;
    bool reload() override;

public:
    explicit ConfJsonClass(const string& path, const string& defContent);
    ~ConfJsonClass();
    static ConfJsonClass* constructor(const Arguments& args);

    Local<Value> init(const Arguments& args);
    Local<Value> get(const Arguments& args);
    Local<Value> set(const Arguments& args);
    Local<Value> del(const Arguments& args);
    virtual Local<Value> reload(const Arguments& args) override;
    virtual Local<Value> close(const Arguments& args) override;
    virtual Local<Value> write(const Arguments& args) override;

    //For Compatibility
    static Local<Value> newConf(const string& path, const string& defContent = "");
};


class ConfIniClass : public ScriptClass, public ConfBaseClass
{
private:
    INI_ROOT iniConf;
    bool flush() override;
    bool close() override;
    bool reload() override;

public:
    explicit ConfIniClass(const string& path, const string& defContent);
    ~ConfIniClass();
    static ConfIniClass* constructor(const Arguments& args);

    bool isValid()
    {
        return iniConf != nullptr;
    }

    Local<Value> init(const Arguments& args);
    Local<Value> set(const Arguments& args);
    Local<Value> getStr(const Arguments& args);
    Local<Value> getInt(const Arguments& args);
    Local<Value> getFloat(const Arguments& args);
    Local<Value> getBool(const Arguments& args);
    Local<Value> del(const Arguments& args);
    virtual Local<Value> reload(const Arguments& args) override;
    virtual Local<Value> close(const Arguments& args) override;
    virtual Local<Value> write(const Arguments& args) override;

    //For Compatibility
    static Local<Value> newConf(const string& path, const string& defContent = "");
};

//////////////////// APIs ////////////////////

Local<Value> MoneySet(const Arguments& args);
Local<Value> MoneyGet(const Arguments& args);
Local<Value> MoneyAdd(const Arguments& args);
Local<Value> MoneyReduce(const Arguments& args);
Local<Value> MoneyTrans(const Arguments& args);
Local<Value> MoneyGetHintory(const Arguments& args);
Local<Value> MoneyClearHistory(const Arguments& args);

Local<Value> Xuid2Name(const Arguments& args);
Local<Value> Name2Xuid(const Arguments& args);

Local<Value> ToJson(const Arguments& args);
Local<Value> ParseJson(const Arguments& args);
Local<Value> ToMD5(const Arguments& args);
Local<Value> ToSHA1(const Arguments& args);
Local<Value> ToBase64(const Arguments& args);
Local<Value> FromBase64(const Arguments& args);

//For Compatibility
Local<Value> OpenConfig(const Arguments& args);
Local<Value> OpenDB(const Arguments& args);