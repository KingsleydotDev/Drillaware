#include "functions.h"
#include "variables.h"

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
}