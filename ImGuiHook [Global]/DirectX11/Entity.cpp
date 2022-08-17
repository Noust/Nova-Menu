#include "Include.h"

DWORD64 Entity::GetEntity(int index) {
	DWORD64 EntityA = *(DWORD64*)((DWORD64)GetModuleHandleA("GTA5.exe") + 0x266DBC8);
	if (EntityA != 0)
		return *(DWORD64*)((EntityA + 0x18) + (index * (6 << 2)));
	return 0;
}
short Entity::GetMaxEntities() {
	short maxents = *(short*)((DWORD64)GetModuleHandleA("GTA5.exe") + (0x266DBC8 + (1 << 3)));
	return maxents - 1;
}
DWORD64 Entity::GetLocalPlayer() {
	DWORD64 EntityA = *(DWORD64*)((DWORD64)GetModuleHandleA("GTA5.exe") + 0x266DBC8);
	if (EntityA != 0)
		return *(DWORD64*)(EntityA);
	return 0;
}
bool Entity::Alive() {
	if (local->Health != 0)
		return true;
	else
		return false;
}
bool Entity::CarExist() {
	if (local->CarPtr != 0)
		return true;
	return false;
}
bool Entity::IsInCar() {
	if (local->IsInCar == 96 || local->IsInCar == 64) {
		if (CarExist())
			return true;
	}
	return false;
}
void Entity::SetPos(Vector3 pos) {
	if (IsInCar()) {
		local->CarPtr->CarPos->CarRealPos = pos;
	}
	else {
		local->RealP->Realpos = pos;
		local->pos = pos;
	}
}