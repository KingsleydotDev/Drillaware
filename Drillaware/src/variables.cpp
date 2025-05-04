#include "variables.h"

bool variables::bEnableDLC = true;
bool variables::bAntiLeave = false;
bool variables::bFFATeamFix = false;
bool variables::bDisableEquipment = false;

const char* variables::realmaplist[26] = { "mp_afghan",
"mp_derail",
"mp_estate",
"mp_favela",
"mp_highrise",
"mp_invasion",
"mp_checkpoint",
"mp_quarry",
"mp_rundown",
"mp_rust",
"mp_boneyard",
"mp_nightshift",
"mp_subbase",
"mp_terminal",
"mp_underpass",
"mp_brecourt",
"mp_complex",
"mp_crash",
"mp_compact",
"mp_overgrown",
"mp_storm",
"mp_abandon",
"mp_fuel2",
"mp_strike",
"mp_trailerpark",
"mp_vacant"
};
const char* variables::map_list[26] = {
    "Afghan", "Derail", "Estate", "Favela", "Highrise", "Invasion", "Karachi",
    "Quarry", "Rundown", "Rust", "Scrapyard", "Skidrow", "Sub Base", "Terminal",
    "Underpass", "Wasteland", "Bailout", "Crash", "Salvage",
    "Overgrown", "Storm", "Carnival", "Fuel", "Strike", "Trailer Park", "Vacant"
};
int variables::map_list_number = 0;
const char* variables::gamemode_list[5] = {
    "Domination", "Team Deathmatch", "Search and Destroy", "Free For All", "Highrise"
};
int variables::gamemode_list_number = 0;
int variables::iMaxPlayers = 18;

char variables::friend1[128] = "";
char variables::friend2[128] = "";
char variables::friend3[128] = "";
char variables::friend4[128] = "";

float variables::fLadderCap = 100.0f;
float variables::fProneCap = 85.0f;

int variables::iFPS = 125;
float variables::fFieldOfView = 65.0f;
bool variables::bThirdPerson = false;

int variables::iPrestige = 9;