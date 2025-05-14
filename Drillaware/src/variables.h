#pragma once

class variables {
public:
    static bool bEnableDLC;
    static bool bAntiLeave;
    static bool bFFATeamFix;
    static bool bDisableEquipment;

    static const char* map_list[26];
    static const char* realmaplist[26];
    static int imap_list_number;
    static const char* gamemode_list[12];
    static const char* realgamemode_list[12];
    static int igamemode_list_number;
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
    static char Console[128];
    static char BarracksWins[128];
    static char BarracksLosses[128];
    static char BarracksTies[128];
    static char BarracksWinStreak[128];
    static char BarracksKills[128];
    static char BarracksHeadshots[128];
    static char BarracksAssists[128];
    static char BarracksKillStreak[128];
    static char BarracksDeaths[128];
    static char BarracksTimePlayed[128];






    static int iPrestige;
};