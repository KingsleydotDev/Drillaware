#include "variables.h"

bool variables::bEnableDLC = true;
bool variables::bAntiLeave = false;
bool variables::bFFATeamFix = false;
bool variables::bDisableEquipment = false;

const char* variables::map_list[27] = {
    "Afghan", "Derail", "Estate", "Favela", "Highrise", "Invasion", "Karachi",
    "Quarry", "Rundown", "Rust", "Scrapyard", "Skidrow", "Sub Base", "Terminal",
    "Underpass", "Wasteland", "-----DLC MAPS-----", "Bailout", "Crash", "Salvage",
    "Overgrown", "Storm", "Carnival", "Fuel", "Strike", "Trailer Park", "Vacant"
};

int variables::map_list_number = 0;
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