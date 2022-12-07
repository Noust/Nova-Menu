#include "Include.h"

DWORD64 Entity::GetEntity(int index) {
	DWORD64 EntityA = *(DWORD64*)(EntityList);
	return *(DWORD64*)((EntityA + 0x18) + (index * (6 << 2)));
}
short Entity::GetMaxEntities() {
	short maxents = *(short*)(MaxEntitiesAddr);
	return maxents - 2;
}
DWORD64 Entity::GetLocalPlayer() {
	DWORD64 EntityA = *(DWORD64*)(EntityList);
	return *(DWORD64*)(EntityA);
}
bool Entity::Alive() {
	if (local->Health != 0)
		return true;
	return false;
}
bool Entity::CarExist() {
	if (local->CarPtr != 0)
		return true;
	return false;
}
bool Entity::IsInCar() {
	if (local->IsInCar == 96 && CarExist())
		return true;
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
bool Entity::IsOpressor() {/*
	if (IsInCar()) {
		if (local->CarPtr->OpressorMisiles != -1)
			return true;
	}*/
	return false;
}