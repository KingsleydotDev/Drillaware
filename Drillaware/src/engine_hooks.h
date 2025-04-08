// engine_hooks.h
#pragma once
#include "variables.h"

// Function pointer types
typedef void(__cdecl* SV_GameSendServerCommand_t)(int clientNum, int reliable, char* command);
extern SV_GameSendServerCommand_t SV_GameSendServerCommand;

typedef void(*Notify_fp)(unsigned int notifyListOwnerId, unsigned int stringValue, void* VariableValue_top);
extern Notify_fp oNotify;

typedef const char* (__cdecl* ConvertToString_t)(unsigned int stringValue);
// Declare ConvertToString as extern
extern ConvertToString_t ConvertToString; 

typedef unsigned int(__cdecl* GetSelf_t)(unsigned int threadId);
extern GetSelf_t GetSelf; // Declare it as extern

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
