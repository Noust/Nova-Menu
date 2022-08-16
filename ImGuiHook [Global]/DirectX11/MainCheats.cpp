#include "Include.h"


DWORD WINAPI SetValues(HMODULE hMod) {
	Entity E;
	while (!GetAsyncKeyState(VK_DELETE)) {
		if (hooked) {
			local = (Entitys*)(E.GetLocalPlayer());
			if (local != 0) {
				if (UserSettings.Godmode && E.Alive())
					local->GodMode = true;
				else
					local->GodMode = false;
			}
		}
		Sleep(20);
	}
	FreeLibraryAndExitThread(hMod, 0);
}