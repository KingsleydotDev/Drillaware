#include "functions.h"
#include "variables.h"
#include "../ext/imgui/imgui.h"

namespace functions
{
    void handleMouseCursor()
    {
        if (gui::open)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.MouseDrawCursor = true;
            *reinterpret_cast<int**>(0x6427D3D) = nullptr;
        }
        else
        {
            ImGuiIO& io = ImGui::GetIO();
            io.MouseDrawCursor = false;
            *reinterpret_cast<int*>(0x6427D3D) = 1; // Release mouse
        }
    }
    void handleHotkeys()
    {
        if (GetAsyncKeyState(VK_F4) & 1) {
            functions::ChangeGamemode();
            OpenMenu(0, "popup_gamesetup");
            Cbuf_AddText(0, "xblive_privatematch 1");
        }
        if (GetAsyncKeyState(VK_F5) & 1) {
            functions::ChangeGamemode();
            functions::doMaxPlayers(variables::iMaxPlayers);
            functions::doStartMatch();
            functions::doBalanceTeams();
        }
    }
    void doTweaks()
    {
        DWORD dwPointer = *(DWORD*)0xAAC1F8; // FOV
        *(float*)(dwPointer + 0xC) = variables::fFieldOfView;

        *(int*)0x638152C = variables::iFPS; // FPS

        if (variables::bThirdPerson)
        {
            DWORD dwPointer = *(DWORD*)0x86A264;
            *(int*)(dwPointer + 0xC) = 1; //enabled
        }
        if (!variables::bThirdPerson)
        {
            DWORD dwPointer = *(DWORD*)0x86A264;
            *(int*)(dwPointer + 0xC) = 0; //disabled
        }

    }
    void sendPrestige(int prestige)
    {
        *(DWORD*)0x1B8B770 = prestige;
    }
    char* getPlayerName(int client)
    {
        return (char*)(0x99786C + (client * 0x52C)); // get player name for kick buttons
    }
    void doLevel70()
    {
        *(DWORD*)0x1B8B768 = 2450000;
    }

    void doDLCMaps()
    {
        if (variables::bEnableDLC)
        {
            *(int*)0x637A7C0 = 8; // DLC 2
        }
        else
        {
            *(int*)0x637A7C0 = 0; // NO DLC
        }
    }
    int getHostId()
    {
        auto hostid = reinterpret_cast<int*>(0xAB5DDC);
        return *hostid;
    }
    int getPlayerAdmin1()
    {
        for (int i = 0; i < 18; i++)
        {
            if (!strcmp(variables::friend1, getPlayerName(i)))
            {
                return i;
            }
        }
        return -1;
    }

    int getPlayerAdmin2()
    {
        for (int i = 0; i < 18; i++)
        {
            if (!strcmp(variables::friend2, getPlayerName(i)))
            {
                return i;
            }
        }
        return -1;
    }
    int getPlayerAdmin3()
    {
        for (int i = 0; i < 18; i++)
        {
            if (!strcmp(variables::friend3, getPlayerName(i)))
            {
                return i;
            }
        }
        return -1;
    }
    int getPlayerAdmin4()
    {
        for (int i = 0; i < 18; i++)
        {
            if (!strcmp(variables::friend4, getPlayerName(i)))
            {
                return i;
            }
        }
        return -1;
    }
    void doFFATeamFix()
    {
        if (variables::bFFATeamFix == true)
        {
            for (int i = 0; i < 18; i++)
            {
                *(int*)(0x1B1139C - 0x80 + (0x366C * i)) = 0x00; 
            }
        }
    }
    void doAntiLeave()
    {
        for (int i = 0; i < 18; i++)
        {
            if ((i == getHostId()) && (i == getPlayerAdmin1()) && (i == getPlayerAdmin2()) && (i == getPlayerAdmin3()) && (i == getPlayerAdmin4()))
            {
                if (variables::bAntiLeave == true)
                {
                    SV_GameSendServerCommand(i, 0, (char*)"s g_scriptmainmenu \"class\"");
                }
            }
            if (!(i == getHostId()) && !(i == getPlayerAdmin1()) && !(i == getPlayerAdmin2()) && !(i == getPlayerAdmin3()) && !(i == getPlayerAdmin4()))
            {
                if (variables::bAntiLeave == true)
                {
                    SV_GameSendServerCommand(i, 0, (char*)"s g_scriptmainmenu \"antileave\"");
                }
            }
        }
    }
    int DisableEquipment1(int clientIndex)
    {
        return 0x1B0E4A0 + (0x366C * (int)clientIndex);
    }
    int DisableEquipment2(int clientIndex)
    {
        return 0x1B0E4B8 - 0x80 + (0x366C * (int)clientIndex);
    }
    void doDisableEquipment()
    {
        for (int i = 0; i < 18; i++)
        {
            if (!(i == getHostId()) && !(i == getPlayerAdmin1()) && !(i == getPlayerAdmin2()) && !(i == getPlayerAdmin3()) && !(i == getPlayerAdmin4()))
            {
                if (variables::bDisableEquipment == true)
                {
                    byte disable[1] = { 0x00 };
                    *(byte*)DisableEquipment1(i) = *disable;
                    *(byte*)DisableEquipment2(i) = *disable;
                }
            }
        }
    }
    void doMaxPlayers(int amount) 
    {
        Cbuf_AddText(0, "sv_maxclients " + amount);
        Cbuf_AddText(0, "party_maxplayers " + amount);
        Cbuf_AddText(0, "ui_maxclients " + amount);
    }
    void doStartMatch()
    {
        Cbuf_AddText(0, ";xblive_privatematch 1;wait 2;xpartygo;wait 2;xblive_privatematch 0;");
    }
    void doBalanceTeams()
    {
        BalanceTeams(reinterpret_cast<void*>(G_LOBBYDATA));
        BalanceTeams(reinterpret_cast<void*>(PARTYSESSION_P));
    }
    void FastRestart()
    {
        MapRestart(0, 0);
        return;
    }
    void ChangeMap()
    {
        SV_SpawnServer((char*)variables::realmaplist[variables::imap_list_number], 0, 0);
        return;
    }
    void ChangeGamemode()
    {
        switch (variables::igamemode_list_number)
        {
        case 0:
            Cbuf_AddText(0, "g_gametype dom; ui_gametype dom; party_gametype dom");
            break;
        case 1:
            Cbuf_AddText(0, "g_gametype war; ui_gametype war; party_gametype war");
            break;
        case 2:
            Cbuf_AddText(0, "g_gametype sd; ui_gametype sd; party_gametype sd");
            break;
        case 3:
            Cbuf_AddText(0, "g_gametype ffa; ui_gametype ffa; party_gametype ffa");
            break;
        case 4:
            Cbuf_AddText(0, "g_gametype koth; ui_gametype koth; party_gametype koth");
            break;
        case 5:
            Cbuf_AddText(0, "g_gametype dem; ui_gametype dem; party_gametype dem");
            break;
        case 6:
            Cbuf_AddText(0, "g_gametype sab; ui_gametype sab; party_gametype sab");
            break;
        case 7:
            Cbuf_AddText(0, "g_gametype ctf; ui_gametype ctf; party_gametype ctf");
            break;
        case 8:
            Cbuf_AddText(0, "g_gametype gtnw; ui_gametype gtnw; party_gametype gtnw");
            break;
        case 9:
            Cbuf_AddText(0, "g_gametype oneflag; ui_gametype oneflag; party_gametype oneflag");
            break;
        case 10:
            Cbuf_AddText(0, "g_gametype vip; ui_gametype vip; party_gametype vip");
            break;
        case 11:
            Cbuf_AddText(0, "g_gametype arena; ui_gametype arena; party_gametype arena");
            break;
        default:
            Cbuf_AddText(0, "g_gametype dom; ui_gametype dom; party_gametype dom"); // default to domination if failed
            break;
        }
    }


}
