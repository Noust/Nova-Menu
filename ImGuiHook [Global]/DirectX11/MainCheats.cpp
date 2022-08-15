#include "Include.h"


DWORD WINAPI SetValues(HMODULE hMod) {
	while (!GetAsyncKeyState(VK_DELETE)) {
		if (hooked) {

		}
		Sleep(20);
	}
	FreeLibraryAndExitThread(hMod, 0);
}