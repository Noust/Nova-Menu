#include "Include.h"


DWORD WINAPI SetValues(HMODULE hMod) {
	Entity E;
	while (!GetAsyncKeyState(VK_DELETE)) {
		if (hooked) {
			local = (Entitys*)(E.GetLocalPlayer());
			if (local != 0) {
				if (UserSettings.Godmode) {
					if (E.Alive())
						local->GodMode = true;
				}
				else
					local->GodMode = false;
				if (UserSettings.CarGodMode) {
					if (E.CarExist())
						local->CarPtr->GodMode = true;
				}
				else {
					if (E.CarExist())
						local->CarPtr->GodMode = false;
				}
				if (UserSettings.NeverWanted) {
					if (E.Alive())
						local->PlayerInfoPtr->WantedLevel = 0;
				}
				if (UserSettings.InfAmmo) {
					if (E.Alive()) {
						Patch((BYTE*)PatchAddr, (BYTE*)"\x90\x90\x90\x90\x90", 5);
						Patch((BYTE*)PatchAddr1, (BYTE*)"\x90\x90\x90", 3);
					}
					else {
						Patch((BYTE*)PatchAddr, (BYTE*)"\x66\x45\x89\x53\x56", 5);
						Patch((BYTE*)PatchAddr1, (BYTE*)"\x89\x5F\x20", 3);
					}
				}
				else {
					Patch((BYTE*)PatchAddr, (BYTE*)"\x66\x45\x89\x53\x56", 5);
					Patch((BYTE*)PatchAddr1, (BYTE*)"\x89\x5F\x20", 3);
				}
			}
		}
		Sleep(20);
	}
	FreeLibraryAndExitThread(hMod, 0);
}