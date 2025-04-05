#pragma once
#include "../ext/imgui/imgui.h"
#define WINDOW_WIDTH  620
#define WINDOW_HEIGHT 470
bool loader_active = true;

namespace fonts {
    ImFont* font = nullptr;
    ImFont* font2 = nullptr;
    ImFont* Main_Font = nullptr;
    ImFont* Regylar = nullptr;
    ImFont* icons = nullptr;
    ImFont* ico_list = nullptr;
    ImFont* ico = nullptr;
}
namespace settings {
    int tab;
}

namespace colors {
    extern ImVec4 buttonhovercolor;
    extern ImColor color_edit4;
}
