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

	bool CornerBoxEsp = false;
	int CornerBoxThickness = 0;
	ImColor PlayerCornerBoxColor = { 0,0,255 };
	ImColor NPCCornerBoxColor = { 0, 255, 0 };

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

	bool ShowMissiles = false;
	ImColor MissilesColor = { 255,255,255 };

	bool FilledESP = false;
	ImColor NPCFilledColor = { 0,0,0,80 };
	ImColor PlayerFilledColor = { 0,0,0,80 };
	float FilledWidth = 0.5f;

	bool Godmode = false;
	bool CarGodMode = false;
	bool NeverWanted = false;
	bool InfAmmo = false;
	bool nocarcollision = false;
	bool KillAura = false;
	bool InfiniteMissiles = false;
	float KillAuraDist = 6.0f;

	int TpWindowMenu = 0;

	Vector3 XYZCustom;
	Vector3 PointPos;

	bool Aimbot = false;
	bool onlyPlayers = false;
	bool onlyNPC = false;
	float miniumHealth = 11;
	float MaxHealth = 900;
	int32_t AimbotTarget = 0x796E;
	int AimbotFov = 1920;
	bool ShowFov = false;
	int FovThickness = 0;
	ImColor FovColor = { 255,255,255 };
	bool ShowTarget = false;
	int TargetThickness = 0;
	ImColor TargetColor = { 255,0,0 };
	bool FilledCircle = false;
	ImColor FilledCircleColor = { 0,0,0,80 };
	int AimbotSleep = 16;

	bool NoRadgoll = false;

	bool CustomValues = false;
	float SwimSpeed = 1, runSpeed = 1, caracceleration = 1, carGravity = 9.800000191f;

	float ESPDistance = 150;
}UserSettings;