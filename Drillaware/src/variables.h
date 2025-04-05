#pragma once

class variables {
public:
    static bool bEnableDLC;
    static bool bAntiLeave;
    static bool bFFATeamFix;
    static bool bDisableEquipment;

    static const char* map_list[27];
    static int map_list_number;

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