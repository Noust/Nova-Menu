#include "Include.h"

DWORD64 Entity::GetEntity(int index) {
	DWORD64 EntityA = *(DWORD64*)((DWORD64)GetModuleHandleA("GTA5.exe") + 0x266DBC8);
	if (EntityA != 0)
		return *(DWORD64*)(EntityA + ((index + 0x18) * (6 << 2)));
	return 0;
}
short Entity::GetMaxEntities() {
	return *(short*)((DWORD64)GetModuleHandleA("GTA5.exe") + (0x266DBC8 + (1 << 3)));
}