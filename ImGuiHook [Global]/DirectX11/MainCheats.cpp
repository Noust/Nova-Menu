#include "Include.h"
bool initiate = false;
bool initiate1 = false;
bool initiate2 = false;
bool initiate3 = false;
bool initiate4 = false;
bool initiate5 = false;
bool initiate6 = false;
bool initiate7 = false;

DWORD WINAPI SetValues(HMODULE hMod) {
	Entity E;
	while (!GetAsyncKeyState(VK_DELETE)) {
		if (hooked) {
			local = (Entitys*)(E.GetLocalPlayer());
			if (local != 0) {
				if (UserSettings.Godmode) {
					if (!initiate2) {
						if (E.Alive()) {
							if (local->GodMode != true) {
								local->GodMode = true;
								initiate2 = true;
								initiate3 = false;
							}
						}
					}
				}
				else {
					if (!initiate3) {
						if (E.Alive()) {
							if (local->GodMode != false) {
								local->GodMode = false;
								initiate2 = false;
								initiate3 = true;
							}
						}
					}
				}
				if (UserSettings.CarGodMode) {
					if (!initiate4) {
						if (E.CarExist()) {
							if (local->CarPtr->GodMode != true) {
								local->CarPtr->GodMode = true;
								initiate4 = true;
								initiate5 = false;
							}
						}
					}
				}
				else {
					if (!initiate5) {
						if (E.CarExist()) {
							if (local->CarPtr->GodMode != false) {
								local->CarPtr->GodMode = false;
								initiate4 = false;
								initiate5 = true;
							}
						}
					}
				}
				if (UserSettings.nocarcollision) {
					if (!initiate6) {
						if (E.CarExist()) {
							if (local->CarPtr->CarCustom->DEFORM_MULTIPLIER != 0) {
								local->CarPtr->CarCustom->DEFORM_MULTIPLIER = 0;
								initiate6 = true;
								initiate7 = false;
							}
						}
					}
				}
				else {
					if (!initiate7) {
						if (E.CarExist()) {
							if (local->CarPtr->CarCustom->DEFORM_MULTIPLIER != 1) {
								local->CarPtr->CarCustom->DEFORM_MULTIPLIER = 1;
								initiate6 = false;
								initiate7 = true;
							}
						}
					}
				}
				if (UserSettings.NeverWanted) {
					if (E.Alive()) {
						if (local->PlayerInfoPtr->WantedLevel != 0)
							local->PlayerInfoPtr->WantedLevel = 0;
					}
				}
				if (UserSettings.InfAmmo) {
					if (!initiate) {
						Patch((BYTE*)PatchAddr, (BYTE*)"\x90\x90\x90\x90\x90", 5);
						Patch((BYTE*)PatchAddr1, (BYTE*)"\x90\x90\x90", 3);
						initiate = true;
						initiate1 = false;
					}
				}
				else {
					if (!initiate1) {
						Patch((BYTE*)PatchAddr, (BYTE*)"\x66\x45\x89\x53\x56", 5);
						Patch((BYTE*)PatchAddr1, (BYTE*)"\x89\x5F\x20", 3);
						initiate1 = true;
						initiate = false;
					}
				}
				if (UserSettings.KillAura) {
					for (int i = 0; i < E.GetMaxEntities(); i++) {
						DWORD64 EntityAddr = E.GetEntity(i);
						if (EntityAddr != 0) {
							entsK = (Entitys*)(EntityAddr);
							if (local->pos.Distance(entsK->pos) < UserSettings.KillAuraDist) {
								entsK->Health = 0;
							}
						}
					}
				}
				if (E.Alive()) {
					local->PlayerInfoPtr->SwimSpeed = UserSettings.SwimSpeed;
					local->PlayerInfoPtr->RunSpeed = UserSettings.runSpeed;
					if (E.CarExist()) {
						local->CarPtr->CarCustom->ACCELERATION = UserSettings.caracceleration;
					}
				}
			}
		}
		Sleep(20);
	}
	FreeLibraryAndExitThread(hMod, 0);
}