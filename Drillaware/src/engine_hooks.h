// engine_hooks.h
#pragma once
#include "variables.h"

#define IW4_FUNCTION(FUNC_NAME, RETURN_TYPE, ARGUMENTS, ADDRESS) RETURN_TYPE(*FUNC_NAME) ARGUMENTS = reinterpret_cast<RETURN_TYPE(*) ARGUMENTS>(ADDRESS)
#define G_LOBBYDATA		0x10F91E8
#define PARTYSESSION_P	0x10F5A18
#define MAX_CLIENTS 18

// Function pointer types
typedef void(__cdecl* SV_GameSendServerCommand_t)(int clientNum, int reliable, char* command);
extern SV_GameSendServerCommand_t SV_GameSendServerCommand;

typedef void(*Notify_fp)(unsigned int notifyListOwnerId, unsigned int stringValue, void* VariableValue_top);
extern Notify_fp oNotify;

typedef const char* (__cdecl* ConvertToString_t)(unsigned int stringValue);
extern ConvertToString_t ConvertToString; 

typedef unsigned int(__cdecl* GetSelf_t)(unsigned int threadId);
extern GetSelf_t GetSelf;

typedef void(__cdecl* Cbuf_AddText_t)(int localClientNum, const char* text);
extern Cbuf_AddText_t Cbuf_AddText;

typedef void(__cdecl* OpenMenu_t)(int localClientNum, const char* menuName);
extern OpenMenu_t OpenMenu;

typedef void(__cdecl* StartMatch_t)(void* party, int localControllerIndex);
extern StartMatch_t Host_StartMatch;

typedef void(__cdecl* SV_SpawnServer_t)(char* name, int, int);
extern SV_SpawnServer_t SV_SpawnServer;
typedef void(__cdecl* BalanceTeams_t)(void* PartyData_s_party);
extern BalanceTeams_t BalanceTeams;

typedef void(__cdecl* MapRestart_t)(int fastRestart, int unknown);
extern MapRestart_t MapRestart;

typedef void(__cdecl* Cbuf_AddCall_t)(int a1, void* a2);
extern Cbuf_AddCall_t Cbuf_AddCall;

enum scriptType_e
{
    SCRIPT_NONE = 0,
    SCRIPT_OBJECT = 1,
    SCRIPT_STRING = 2,
    SCRIPT_VECTOR = 4,
    SCRIPT_FLOAT = 5,
    SCRIPT_INTEGER = 6
};

struct VariableValue
{
    union
    {
        void* entity;
        float number;
        unsigned short string;
        float* vector;
        int integer;
    };

    scriptType_e type;
};

namespace engine
{
    void hookNotify();
    void mNotify(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue* top);
 
}

