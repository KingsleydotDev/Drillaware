#pragma once
#include "gui.h"
#include "engine_hooks.h"

namespace functions {
	void handleMouseCursor();
	void handleHotkeys();
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
	int DisableEquipment1(int clientIndex);
	int DisableEquipment2(int clientIndex);
	void doDisableEquipment();
	void doMaxPlayers(int amount);
	void doStartMatch();
	void doBalanceTeams();
	void FastRestart();
	void ChangeMap();
	void ChangeGamemode();
	void doForceHost();
	void writeMemory(DWORD dwAddress, void* bytes, DWORD dwSize);
	void unlockAll();
	void doSaveBarracks();
	void doGiveDeag();
}