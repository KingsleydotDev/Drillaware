// engine_hooks.cpp
#include "engine_hooks.h"
#include <Drillaware/Drillaware/ext/detours/detours.h>

// Define function pointers with their appropriate addresses
ConvertToString_t ConvertToString = (ConvertToString_t)0x05799B0;
GetSelf_t GetSelf = (GetSelf_t)0x57B060; // Define GetSelf with its correct address

SV_GameSendServerCommand_t SV_GameSendServerCommand = (SV_GameSendServerCommand_t)0x588340;

#define MAX_CLIENTS  18
Notify_fp oNotify = nullptr;

namespace engine
{
    void mNotify(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue* top)
    {
        // Custom logic before calling the original function
        if (variables::bFFATeamFix == true)
        {
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                *(int*)(0x1B1139C - 0x80 + (0x366C * i)) = 0x00;  // Done
            }
        }
        // Call the original function that was replaced
        return oNotify(notifyListOwnerId, stringValue, top);
    }

    void hookNotify()
    {
        //oNotify = reinterpret_cast<Notify_fp>(DetourFunction(reinterpret_cast<PBYTE>(0x57FA70), reinterpret_cast<PBYTE>(mNotify)));

    }
}
