#pragma once
#include "Include.h"
void Patch(BYTE* addr, BYTE* bytes, unsigned int size);
bool Hook(BYTE* pTarget, BYTE* pHook, UINT Length);
MODULEINFO GetModuleInfo(char* szModule);
DWORD64 FindPattern(char* module, char* pattern, char* mask);
void GetAddresses();