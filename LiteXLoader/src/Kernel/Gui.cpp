#include "Gui.h"
#include "Db.h"
#include "Packet.h"
using namespace std;

int Raw_SendSimpleForm(Player* player,const string &title,const string &content,const vector<string> &buttons)
{
    string model = u8R"({"title":"%s","content":"%s","buttons":%s,"type":"form"})";
    model = model.replace(model.find("%s"),2, title);
    model = model.replace(model.find("%s"),2, content);

    JSON_ROOT buttonText;
    for (auto& text : buttons)
    {
        JSON_VALUE oneButton = JSON_VALUE::object();
        oneButton["text"] = text;
        buttonText.push_back(oneButton);
    }
    string result = buttonText.dump();

    model = model.replace(model.find("%s"),2, result);

    return Raw_SendFormPacket(player,model);    //############## callback崩服 ##############
}

int Raw_SendModalForm(Player* player,const string &title,const string &content,const string &button1,const string &button2)
{
    string model = R"({"title":"%s","content":"%s","button1":"%s","button2":"%s","type":"modal"})";
    model = model.replace(model.find("%s"),2, title);
    model = model.replace(model.find("%s"),2, content);
    model = model.replace(model.find("%s"),2, button1);
    model = model.replace(model.find("%s"),2, button2);

    return Raw_SendFormPacket(player,model);
}

int Raw_SendCustomForm(Player* player,const std::string &data)
{
    return Raw_SendFormPacket(player,data);
}

bool Raw_CancelForm(int formId)
{
    //################# Add code here #################
    return false;
}