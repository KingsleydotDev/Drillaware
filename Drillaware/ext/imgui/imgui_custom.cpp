#include "imgui_custom.h"
using namespace ImGui;
extern struct tab_state
{
    ImVec4 text;
    ImVec4 bg;
    ImVec4 bg2;
};

ImFont* fonts::font2 = nullptr;

namespace colors {
    ImColor color_edit4 = { 110 / 255.f, 117 / 255.f, 255 / 255.f, 255.00f };
    ImVec4 accentcolor = ImColor(205, 92, 92);
    ImVec4 buttonhovercolor = ImColor(63, 63, 63);
}



bool custom::tab(const char* label, bool active)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + ImVec2(120, 35));
    ItemSize(rect, style.FramePadding.y);
    if (!ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held);
    if (pressed)
        MarkItemEdited(id);

    static std::map<ImGuiID, tab_state> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, tab_state() });
        it_anim = anim.find(id);
    }
    it_anim->second.text = ImLerp(it_anim->second.text, active ? ImColor(255, 255, 255) : hovered ? ImColor(71, 71, 73) : ImColor(71, 71, 73), g.IO.DeltaTime * 6.f);
    it_anim->second.bg = ImLerp(it_anim->second.bg, active ? ImColor(20, 20, 20) : hovered ? ImColor(22, 22, 22, 150) : ImColor(0, 0, 0, 0), g.IO.DeltaTime * 6.f);
    it_anim->second.bg2 = ImLerp(it_anim->second.bg2, active ? ImColor(colors::accentcolor) : hovered ? ImColor(22, 22, 22, 150) : ImColor(0, 0, 0, 0), g.IO.DeltaTime * 6.f);

    window->DrawList->AddRectFilled(rect.Min + ImVec2(10, 5), rect.Min + ImVec2(130, 35), ImColor(it_anim->second.bg), 3);
    window->DrawList->AddRectFilled(rect.Min + ImVec2(0, 10), rect.Min + ImVec2(2.5f, 30), ImColor(it_anim->second.bg2), 10, ImDrawCornerFlags_Right);
    window->DrawList->AddText(fonts::font2, 18, rect.Min + ImVec2(20, 10), ImColor(it_anim->second.text), label);
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool custom::button(const char* label, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, 0, 1);

    ImVec2 pos = window->DC.CursorPos;
    if (style.FramePadding.y < window->DC.CurrLineTextBaseOffset)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;

    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, 0); // Flags set to 0

    // Background color logic
    ImU32 col;
    if (held)
        col = ImColor(colors::buttonhovercolor); // Light gray when clicked
    else
        col = ImColor(32, 33, 35);    // Default background

    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, 0, 5);
    window->DrawList->AddRect(bb.Min, bb.Max, ImColor(32, 33, 35), 5); // Border

    // Text color: blue on hover, light gray otherwise
    const ImU32 text_col = hovered ? ImColor(colors::accentcolor) : ImColor(192, 203, 229);
    window->DrawList->AddText(bb.GetCenter() - label_size / 2, text_col, label);

    return pressed;
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

bool custom::combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    const bool value_changed = custom::Combo(label, current_item, custom::Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

bool custom::Combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        items_getter(data, *current_item, &preview_value);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!custom::begincombo(label, preview_value))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        PushID(i);
        const bool item_selected = (i == *current_item);
        const char* item_text;
        if (!items_getter(data, i, &item_text))
            item_text = "*Unknown item*";
        if (Selectable(item_text, item_selected))
        {
            value_changed = true;
            *current_item = i;
        }
        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }

    EndCombo();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

static bool custom::Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}

struct combo_state {
    ImVec4 background, text;

    bool opened_combo = false, hovered = false;
    float combo_size = 0.f, expand_rotation = 1.0f;
    float open_anim = 5.0f;
    float arrow_anim = 5.0f;
};

bool custom::begincombo(const char* label, const char* preview_value)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags();
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const float size = GetWindowWidth();
    const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size - 15, 44));
    const ImRect clickable(window->DC.CursorPos + ImVec2(0, 18), window->DC.CursorPos + ImVec2(size - 15, 44));
    ItemSize(rect, style.FramePadding.y);
    if (!ItemAdd(clickable, id, &rect))
        return false;

    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior(clickable, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    // Animation state
    static std::map<ImGuiID, combo_state> anim;
    auto& state = anim[id];
    state.opened_combo = popup_open;
    state.combo_size = ImLerp(state.combo_size, popup_open ? 1.0f : 0.0f, g.IO.DeltaTime * 6.0f);
    state.arrow_anim = ImLerp(state.arrow_anim, popup_open ? 0.3f : 0.0f, 0.05f * (1.0f - g.IO.DeltaTime));
    state.background = ImLerp(state.background,
        popup_open ? ImColor(20, 20, 20) :
        hovered ? ImColor(25, 25, 25) : ImColor(32, 33, 35), g.IO.DeltaTime * 6.0f);
    state.text = ImLerp(state.text,
        popup_open ? ImColor(255, 255, 255, 255) :
        hovered ? ImColor(180, 180, 180, 180) : ImColor(107, 107, 107, 107), g.IO.DeltaTime * 6.0f);

    // Draw background
    window->DrawList->AddRectFilled(clickable.Min, clickable.Max, GetColorU32(state.background), 3.0f);
    window->DrawList->AddText(rect.Min, GetColorU32(ImGuiCol_Text), label);

    // Preview
    RenderTextClipped(clickable.Min + ImVec2(8, 5), clickable.Max - ImVec2(24, -45), preview_value, NULL, NULL, ImVec2(0.0f, 0.0f));

    // Arrow
    const float arrow_size = GetFrameHeight() + 2;
    const float value_x2 = ImMax(clickable.Min.x, clickable.Max.x - arrow_size);

    window->DrawList->AddLine(ImVec2(value_x2 + 5, clickable.Min.y + 10), ImVec2(value_x2 + 8, clickable.Min.y + 14), ImColor(1.0f, 1.0f, 1.0f, 0.6f + state.arrow_anim));
    window->DrawList->AddLine(ImVec2(value_x2 + 8, clickable.Min.y + 14), ImVec2(value_x2 + 12, clickable.Min.y + 9), ImColor(1.0f, 1.0f, 1.0f, 0.6f + state.arrow_anim));
    window->DrawList->AddLine(ImVec2(value_x2 + 4, clickable.Min.y + 10), ImVec2(value_x2 + 8, clickable.Min.y + 15), ImColor(1.0f, 1.0f, 1.0f, 0.6f + state.arrow_anim));
    window->DrawList->AddLine(ImVec2(value_x2 + 8, clickable.Min.y + 15), ImVec2(value_x2 + 13, clickable.Min.y + 9), ImColor(1.0f, 1.0f, 1.0f, 0.6f + state.arrow_anim));

    if (!popup_open)
        return false;

    g.NextWindowData.Flags = backup_next_window_data_flags;
    if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    // Set popup size (default height = 8 items)
    float w = clickable.GetWidth();
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
    {
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    }
    else
    {
        SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(8) * state.combo_size));
    }

    // Set position
    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size);
    if (ImGuiWindow* popup_window = FindWindowByName(name))
    {
        if (popup_window->WasActive)
        {
            ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
            popup_window->AutoPosLastDirection = ImGuiDir_Down;
            ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(clickable.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, clickable, ImGuiPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos + ImVec2(0, 3));
        }
    }

    // Begin popup
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 10));
    PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
    PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
    PushStyleColor(ImGuiCol_PopupBg, ImVec4(ImColor(32, 33, 35)));

    bool ret = Begin(name, NULL, window_flags | ImGuiWindowFlags_NoScrollbar);

    PopStyleVar(3);
    PopStyleColor();

    if (!ret)
    {
        EndPopup();
        IM_ASSERT(0);
        return false;
    }

    return true;
}
