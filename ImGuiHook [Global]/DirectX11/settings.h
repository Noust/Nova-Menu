#pragma once
#include "Include.h"
class settings {
public:
	int MenuWindow;
	
	bool BoneEsp = false;
	int BoneThickness = 0;
	ImColor PlayerBoneColor = { 0,0,255 };
	ImColor NPCBoneColor = { 0, 255, 0 };

	bool BoxEsp = false;
	int BoxThickness = 0;
	ImColor PlayerBoxColor = { 0,0,255 };
	ImColor NPCBoxColor = { 0, 255, 0 };
	float BoxWidth = 0.5f;

	bool Esp3d = false;
	int Thickness3d = 0;
	ImColor Player3dColor = { 0,0,255 };
	ImColor NPC3dColor = { 0, 255, 0 };

	bool SnapLine = false;
	int ThicknessSnap = 0;
	ImColor PlayerSnapColor = { 0,0,255 };
	ImColor NPCSnapColor = { 0, 255, 0 };
	bool top = false;
	bool bottom = true;

	bool Type = false;
	ImColor PlayerTypeColor = { 0,0,255 };
	ImColor NPCTypeColor = { 0, 255, 0 };

	bool HP = false;
	ImColor PlayerHPColor = { 0,0,255 };
	ImColor NPCHPColor = { 0, 255, 0 };

	bool Distance = false;
	ImColor PlayerDistanceColor = { 0,0,255 };
	ImColor NPCDistanceColor = { 0, 255, 0 };

	bool Name = false;
	ImColor PlayerNameColor = { 0,0,255 };

	bool Godmode = false;
	bool CarGodMode = false;
	bool NeverWanted = false;
	bool InfAmmo = false;

	float ESPDistance = 200;
}UserSettings;
