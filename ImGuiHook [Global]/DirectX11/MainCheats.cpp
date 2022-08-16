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
			}
		}
		Sleep(20);
	}
	FreeLibraryAndExitThread(hMod, 0);
}