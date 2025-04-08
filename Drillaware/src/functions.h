#pragma once
#include <Drillaware/Drillaware/ext/imgui/imgui.h>
#include "gui.h"
#include "engine_hooks.h"

namespace functions {
	void handleMouseCursor();
	void doTweaks();
	void sendPrestige(int prestige);
	char* getPlayerName(int client);
	void doLevel70();
	void doDLCMaps();
	int getHostId();
	int getPlayerAdmin1();
	int getPlayerAdmin2();
	int getPlayerAdmin3();
	int getPlayerAdmin4();
	void doFFATeamFix();
	void doAntiLeave();
}