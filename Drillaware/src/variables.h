#pragma once

class variables {
public:
    static bool bEnableDLC;
    static bool bAntiLeave;
    static bool bFFATeamFix;
    static bool bDisableEquipment;

    static const char* map_list[26];
    static const char* realmaplist[26];
    static int map_list_number;
    static const char* gamemode_list[5];
    static int gamemode_list_number;
    static int iMaxPlayers;

    static char friend1[128];
    static char friend2[128];
    static char friend3[128];
    static char friend4[128];

    static float fLadderCap;
    static float fProneCap;

    static int iFPS;
    static float fFieldOfView;
    static bool bThirdPerson;

    static int iPrestige;
};