#include "gui.h"
#include "hooks.h"
#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../ext/imgui/imgui_impl_dx9.h"

#include "utils.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include "variables.h"
#include "Font.h"
#include "functions.h"
#include "../ext/imgui/imgui_custom.h"
#include "../ext/imgui/etc_element.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

// window process
LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	//Populate Window Class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	// register
	if (!RegisterClassEx(&windowClass))
	{
		return false;
	}

	return true;
}

void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	// Setup Temp Window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,
		100,
		0,
		0,
		windowClass.hInstance,
		0
	);
	if (!window) {
		return false;
	}
	return true;
}
void gui::DestroyWindow() noexcept
{
	if (window) {
		DestroyWindow(window);
	}
}

bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
	{
		return false;
	}

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));

	if (!create) 
	{
		return false;
	}

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
	{
		return false;
	}

	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device) < 0) return false;

	return true;

}
void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}
	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

// Setup Device
void gui::Setup()
{
	if (!SetupWindowClass("hackClass001"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("Hack Window"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to setup DirectX.");

	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{};
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
	);

	//Imgui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg] = ImColor(12, 12, 12);
    style.Colors[ImGuiCol_ChildBg] = ImColor(9, 9, 9);
    style.Colors[ImGuiCol_Border] = ImColor(12, 12, 12, 0);
    style.Colors[ImGuiCol_FrameBg] = ImColor(18, 18, 18);
    style.Colors[ImGuiCol_TextSelectedBg] = ImColor(205, 92, 92);

    style.ChildRounding = 5.f;
    style.ItemSpacing = ImVec2(0, 0);
    ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
    style.WindowRounding = 5.f;

    fonts::font = io.Fonts->AddFontFromMemoryTTF(&Font, sizeof Font, 24.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    fonts::Regylar = io.Fonts->AddFontFromMemoryTTF(&Font1, sizeof Font1, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(0, 0, 0);
    DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect);
    auto x = float(screen_rect.right - WINDOW_WIDTH) / 2.f;
    auto y = float(screen_rect.bottom - WINDOW_HEIGHT) / 2.f;

	setup = true;

}

void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// restore wnd proc
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}

void FastRestart()
{
    MapRestart(0, 0);
    return;
}
void ChangeMap()
{
    SV_SpawnServer((char*)variables::realmaplist[variables::map_list_number],0,0);
    return;
}
void gui::Render() noexcept
{
	ImGui::Begin("DRILLAWARE", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
	{
        ImDrawList* draw = ImGui::GetWindowDrawList();
        const auto& p = ImGui::GetWindowPos();

        draw->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 620, p.y + 50), ImColor(9, 9, 9), 5, ImDrawCornerFlags_Top);
        draw->AddRectFilled(ImVec2(p.x, p.y + 50), ImVec2(p.x + 620, p.y + 51), ImColor(18, 18, 18), 5);
        draw->AddRectFilled(ImVec2(p.x, p.y + 51), ImVec2(p.x + 140, p.y + 470), ImColor(9, 9, 9), 5, ImDrawCornerFlags_BotLeft);
        draw->AddText(fonts::font, 24, ImVec2(p.x + 14, p.y + 12), ImColor(255, 255, 255), ".");
        draw->AddText(fonts::font, 24, ImVec2(p.x + 18, p.y + 12), ImColor(205, 92, 92), "DRILLAWARE");

        ImGui::SetCursorPos(ImVec2(1, 55));
        ImGui::BeginGroup(); {

            ImGui::TextLine("Main");
            if (custom::tab("Host", 0 == settings::tab))
                settings::tab = 0;
            if (custom::tab("Friends", 1 == settings::tab))
                settings::tab = 1;
            if (custom::tab("Players", 2 == settings::tab))
                settings::tab = 2;
            if (custom::tab("Trickshotting", 3 == settings::tab))
                settings::tab = 3;
            if (custom::tab("Tweaks", 4 == settings::tab))
                settings::tab = 4;
            ImGui::TextLine("Misc");
            if (custom::tab("Profile", 5 == settings::tab))
                settings::tab = 5;
            if (custom::tab("About", 6 == settings::tab))
                settings::tab = 6;

        }ImGui::EndGroup();

        if (settings::tab == 0) {
            ImGui::SetCursorPos(ImVec2(150, 25));
            e_elements::begin_child("Host", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                if (ImGui::Checkbox("Enable DLC", &variables::bEnableDLC))
                    functions::doDLCMaps();
                ImGui::Checkbox("Anti Leave", &variables::bAntiLeave);
                ImGui::Checkbox("FFA Team Fix", &variables::bFFATeamFix);
                ImGui::Checkbox("Disable Equipment", &variables::bDisableEquipment);
            }ImGui::EndGroup();
            e_elements::end_child();
            ImGui::SetCursorPos(ImVec2(384, 25));
            e_elements::begin_child("Lobby Settings", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);
                custom::combo("Select Map", &variables::map_list_number, variables::map_list, IM_ARRAYSIZE(variables::map_list), 5);
                if (custom::button("Change Map", ImVec2(95, 25)))
                {
                    // king josh make it do the map change
                    //removed what i had to make it eaiser for you
                    Cbuf_AddCall(0, ChangeMap);
                }
                ImGui::SameLine(); 
                if (custom::button("Fast Restart", ImVec2(95, 25)))
                {
                    Cbuf_AddCall(0, FastRestart);
                }
                if (custom::button("Lock Lobby", ImVec2(200, 25)))
                    Cbuf_AddText(0, "xblive_privatematch 1");
                if (custom::button("Match Settings", ImVec2(200, 25)))
                    OpenMenu(0, "popup_gamesetup");
                if (custom::button("Start Match", ImVec2(200, 25))) 
                {
              
                    functions::doMaxPlayers(variables::iMaxPlayers);
                    functions::doStartMatch();
                    functions::doBalanceTeams();
                    
                   
                   
                }
                ImGui::SliderInt("Max Players", &variables::iMaxPlayers, 2, 18);
                ImGui::PopFont();

            }ImGui::EndGroup();
            e_elements::end_child();
        }
        if (settings::tab == 1) {
            ImGui::SetCursorPos(ImVec2(150, 25));
            e_elements::begin_child("Friends", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);
                ImGui::InputText("Friend 1", variables::friend1, IM_ARRAYSIZE(variables::friend1));
                ImGui::InputText("Friend 2", variables::friend2, IM_ARRAYSIZE(variables::friend2));
                ImGui::InputText("Friend 3", variables::friend3, IM_ARRAYSIZE(variables::friend3));
                ImGui::InputText("Friend 4", variables::friend4, IM_ARRAYSIZE(variables::friend4));
                ImGui::PopFont();
            }ImGui::EndGroup();
            e_elements::end_child();
        }
        if (settings::tab == 2) {
            ImGui::SetCursorPos(ImVec2(150, 25));
            e_elements::begin_child("Kick Players", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);
                if (custom::button(functions::getPlayerName(0), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(0, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(1), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(1, 0, (char*)"t EXE_DISCONNECTED");
                }

                if (custom::button(functions::getPlayerName(2), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(2, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(3), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(3, 0, (char*)"t EXE_DISCONNECTED");
                }

                if (custom::button(functions::getPlayerName(4), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(4, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(5), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(5, 0, (char*)"t EXE_DISCONNECTED");
                }

                if (custom::button(functions::getPlayerName(6), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(6, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(7), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(7, 0, (char*)"t EXE_DISCONNECTED");
                }

                if (custom::button(functions::getPlayerName(8), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(8, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(9), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(9, 0, (char*)"t EXE_DISCONNECTED");
                }

                if (custom::button(functions::getPlayerName(10), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(10, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(11), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(11, 0, (char*)"t EXE_DISCONNECTED");
                }

                if (custom::button(functions::getPlayerName(12), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(12, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(13), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(13, 0, (char*)"t EXE_DISCONNECTED");
                }

                if (custom::button(functions::getPlayerName(14), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(14, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(15), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(15, 0, (char*)"t EXE_DISCONNECTED");
                }

                if (custom::button(functions::getPlayerName(16), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(16, 0, (char*)"t EXE_DISCONNECTED");
                } ImGui::SameLine();

                if (custom::button(functions::getPlayerName(17), ImVec2(95, 25)))
                {
                    SV_GameSendServerCommand(17, 0, (char*)"t EXE_DISCONNECTED");
                }

                ImGui::PopFont();

            }ImGui::EndGroup();
            e_elements::end_child();
        }
        if (settings::tab == 3) {
            ImGui::SetCursorPos(ImVec2(150, 25));
            e_elements::begin_child("Trickshotting", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);
                ImGui::SliderFloat("Ladder Cap", &variables::fLadderCap, 100.0f, 360.0f, "% .1f");
                ImGui::SliderFloat("Prone Cap", &variables::fProneCap, 85.0f, 360.0f, "% .1f");

                ImGui::PopFont();
            }ImGui::EndGroup();
            e_elements::end_child();
            ImGui::SetCursorPos(ImVec2(384, 25));
            e_elements::begin_child("Trickshotting 2", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);
                ImGui::PopFont();

            }ImGui::EndGroup();
            e_elements::end_child();
        }
        if (settings::tab == 4) {
            ImGui::SetCursorPos(ImVec2(150, 25));
            e_elements::begin_child("Tweaks", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::SliderInt("Frames Per Second", &variables::iFPS, 125, 333);
                ImGui::SliderFloat("Field Of View", &variables::fFieldOfView, 65.0f, 120.0f, "%.1f");
                ImGui::Checkbox("Third Person", &variables::bThirdPerson);
            }ImGui::EndGroup();
            e_elements::end_child();
            ImGui::SetCursorPos(ImVec2(384, 25));
            e_elements::begin_child("Tweaks 2", ImVec2(224, 435));

            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);
                ImGui::PopFont();

            }ImGui::EndGroup();
            e_elements::end_child();
        }
        if (settings::tab == 5) {
            ImGui::SetCursorPos(ImVec2(150, 25));
            e_elements::begin_child("Account", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);
                ImGui::SliderInt("Prestige", &variables::iPrestige, 0, 11);
                if (custom::button("Send Prestige", ImVec2(200, 25)))
                    functions::sendPrestige(variables::iPrestige);
                if (custom::button("LVL 70", ImVec2(200, 25)))
                    functions::doLevel70();
                ImGui::PopFont();
            }ImGui::EndGroup();
            e_elements::end_child();
        }
        if (settings::tab == 6) {
            ImGui::SetCursorPos(ImVec2(150, 25));
            e_elements::begin_child("About", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);
                ImGui::Text("Thank you to Josh/@GRIIMtB");
                ImGui::PopFont();
            }ImGui::EndGroup();
            e_elements::end_child();
            ImGui::SetCursorPos(ImVec2(384, 25));
            e_elements::begin_child("Misc", ImVec2(224, 435));
            ImGui::SetCursorPos(ImVec2(10, 40));
            ImGui::BeginGroup(); {
                ImGui::PushFont(fonts::Regylar);

                ImGui::PopFont();

            }ImGui::EndGroup();
            e_elements::end_child();
        }
	}
	ImGui::End();
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	// toggle menu
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		gui::open = !gui::open;
	}

    // calling functions
    functions::handleMouseCursor();
    functions::doTweaks();
    //engine::hookNotify();
    functions::doAntiLeave();
    //functions::doFFATeamFix(); // commented out due to notify testing
    functions::doDisableEquipment();

	// Pass Messages to Imgui
	if (gui::open && ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		wideParam,
		longParam
	)) return 1L;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}