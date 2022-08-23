#include "Include.h"
bool initiate = false;
bool initiate1 = false;

DWORD WINAPI SetValues(HMODULE hMod) {
	Entity E;
	while (!GetAsyncKeyState(VK_NUMPAD1)) {
		if (hooked) {
			local = (Entitys*)(E.GetLocalPlayer());
			if (local != 0 && !OnPause()) {
				if (ShowMenu && UserSettings.MenuWindow != 2) {
					if (UserSettings.NoRadgoll) {
						if (local->radgoll != 0)
							local->radgoll = 0;
					}
					else {
						if (local->radgoll != 32)
							local->radgoll = 32;
					}
					if (UserSettings.Godmode) {
						if (local->GodMode != true)
							local->GodMode = true;
					}
					else {
						if (local->GodMode != false)
							local->GodMode = false;
					}
					if (UserSettings.CarGodMode) {
						if (E.IsInCar()) {
							if (local->CarPtr->GodMode != true)
								local->CarPtr->GodMode = true;
						}
					}
					else {
						if (E.IsInCar()) {
							if (local->CarPtr->GodMode != false)
								local->CarPtr->GodMode = false;
						}
					}
					if (UserSettings.nocarcollision) {
						if (E.IsInCar()) {
							if (local->CarPtr->CarCustom != 0) {
								if (local->CarPtr->CarCustom->DEFORM_MULTIPLIER != 0)
									local->CarPtr->CarCustom->DEFORM_MULTIPLIER = 0;
							}
						}
					}
					else {
						if (E.IsInCar()) {
							if (local->CarPtr->CarCustom != 0) {
								if (local->CarPtr->CarCustom->DEFORM_MULTIPLIER != 1)
									local->CarPtr->CarCustom->DEFORM_MULTIPLIER = 1;
							}
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
					if (UserSettings.CustomValues) {
						if (E.Alive()) {
							local->PlayerInfoPtr->SwimSpeed = UserSettings.SwimSpeed;
							local->PlayerInfoPtr->RunSpeed = UserSettings.runSpeed;
							if (E.IsInCar() && local->CarPtr->CarCustom != 0) {
								local->CarPtr->CarCustom->ACCELERATION = UserSettings.caracceleration;
								local->CarPtr->GravityCar = UserSettings.carGravity;
							}
						}
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
				if (UserSettings.NeverWanted && !ShowMenu && local->PlayerInfoPtr != 0) {
					if (local->PlayerInfoPtr->WantedLevel != 0)
						local->PlayerInfoPtr->WantedLevel = 0;
				}
				if (UserSettings.InfiniteMissiles && !ShowMenu && E.IsOpressor()) {
					if (local->CarPtr->OpressorMisiles != 45)
						local->CarPtr->OpressorMisiles = 45;
				}
			}
		}
		Sleep(20);
	}
	FreeLibraryAndExitThread(hMod, 0);
}