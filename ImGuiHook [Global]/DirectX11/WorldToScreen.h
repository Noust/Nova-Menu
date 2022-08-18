#pragma once
#include "Include.h"

bool WorldToScreen(Vector4 pos, float matrix[16], Vector2& posscreen);
Vector2 PosToScreen(Vector3 pos);
bool OnPause();
void SetMouseMode(int8_t MouseMode);