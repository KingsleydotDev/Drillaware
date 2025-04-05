#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace fonts {
    extern ImFont* font2;
}

namespace custom {
    bool tab(const char* name, bool boolean);
    bool button(const char* label, const ImVec2& size_arg);
    bool combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items);
    bool Combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items);
    bool Items_ArrayGetter(void* data, int idx, const char** out_text);
    bool begincombo(const char* label, const char* preview_value);
}

