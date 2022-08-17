#include "Include.h"

bool WorldToScreen(Vector4 pos, float matrix[16], Vector2& posscreen) {
	Vector4 clc;
	clc.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clc.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clc.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clc.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clc.w < 0.1f)
		return false;

	Vector3 N;
	N.x = clc.x / clc.w;
	N.y = clc.y / clc.w;
	N.z = clc.z / clc.w;

	posscreen.x = (1920 / 2 * N.x) + (N.x + 1920 / 2);
	posscreen.y = -(1080 / 2 * N.y) + (N.y + 1080 / 2);
	return true;
}

Vector2 PosToScreen(Vector3 pos) {
	float Matrix[16];
	Vector2 posscreen;
	Vector4 poss = { pos.x,pos.y,pos.z,0 };
	if (hooked && ViewAddr != NULL) {
		memcpy(&Matrix, (PBYTE*)ViewAddr, sizeof(Matrix));
		if (WorldToScreen(poss, Matrix, posscreen)) {
			return posscreen;
		}
	}
	return { -1,-1 };
}

bool OnPause() {
	int8_t OnPauseB = *(int8_t*)((DWORD64)GetModuleHandleA("GTA5.exe") + 0x20D6107);
	if (OnPauseB != 0)
		return true;
	return false;
}