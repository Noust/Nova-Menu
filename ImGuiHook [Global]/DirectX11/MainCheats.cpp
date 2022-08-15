#include "Include.h"


DWORD WINAPI SetValues(HMODULE hMod) {
	Entity E;
	while (!GetAsyncKeyState(VK_DELETE)) {
		if (hooked) {
			local = (Entitys*)(E.GetLocalPlayer());
		}
		Sleep(20);
	}
	FreeLibraryAndExitThread(hMod, 0);
}