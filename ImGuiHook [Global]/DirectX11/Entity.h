#pragma once
#include "Include.h"

class Entity {
public:
	DWORD64 GetEntity(int index);
	short GetMaxEntities();
	DWORD64 GetLocalPlayer();
	bool Alive();
	bool CarExist();
	bool IsInCar();
	void SetPos(Vector3 pos);
	bool IsOpressor();
	bool HaveMissiles();
};