#include "Global.h"
#include "Device.h"
#include "Player.h"
#include <Engine/LocalShareData.h>
using namespace std;

class NetworkHandler;

string Raw_GetIP(Player* player)
{
    return liteloader::getIP(*offPlayer::getNetworkIdentifier(player));
}

int Raw_GetAvgPing(Player* player)
{
    //return liteloader::getAvgPing(player);
    auto netid = offPlayer::getNetworkIdentifier(player);
    auto nwpeer = SymCall("?getPeerForUser@NetworkHandler@@QEAAPEAVNetworkPeer@@AEBVNetworkIdentifier@@@Z"
        , NetworkPeer*, NetworkHandler*, NetworkIdentifier*)(LocateService<Minecraft>()->getNetworkHandler(), netid);
    if (!nwpeer)
        return -1;

    auto nwstatus = nwpeer->getNetworkStatus();

    string res = std::to_string(nwstatus.avgping);      //strange, but better than crash
    return stoi(res);
}

float Raw_GetAvgPacketloss(Player* player)
{
    return 0.0f;   // liteloader::getAvgPacketloss(player);
}

std::string Raw_GetOs(Player* player)
{
    try
    {
        if (Raw_IsSimulatedPlayer(player))
            return "None";
        return Raw_GetOsNameByType(localShareData->deviceInfoRecord.at((uintptr_t)player).DeviceOS);
    }
    catch (...)
    {
        return "Unknown";
    }
}

std::string Raw_GetClientId(Player* player)
{
    try
    {
        if (Raw_IsSimulatedPlayer(player))
            return "";
        return localShareData->deviceInfoRecord.at((uintptr_t)player).DeviceId;
    }
    catch (...)
    {
        return "";
    }
}

std::string Raw_GetOsNameByType(int type)
{
    string os = "";
    switch (type)
    {
    case 1:
        os = "Android";
        break;
    case 2:
        os = "iOS";
        break;
    case 3:
        os = "OSX";
        break;
    case 4:
        os = "Amazon";
        break;
    case 5:
        os = "GearVR";
        break;
    case 6:
        os = "Hololens";
        break;
    case 7:
        os = "Windows10";
        break;
    case 8:
        os = "Win32";
        break;
    case 10:
        os = "TVOS";
        break;
    case 11:
        os = "PlayStation";
        break;
    case 12:
        os = "Nintendo";
        break;
    case 13:
        os = "Xbox";
        break;
    case 14:
        os = "WindowsPhone";
        break;
    default:
        os = "Unknown";
        break;
    }
    return os;
}