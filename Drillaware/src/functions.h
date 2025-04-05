#pragma once
#include <Drillaware/Drillaware/ext/imgui/imgui.h>
#include "gui.h"

namespace functions {
	void handleMouseCursor();
	void doTweaks();
	void sendPrestige(int prestige);
	char* getPlayerName(int client);
	void doLevel70();
}