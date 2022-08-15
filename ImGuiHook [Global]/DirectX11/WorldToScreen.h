#pragma once
#include "Include.h"

bool WorldToScreen(Vector4 pos, float matrix[16], Vector2& posscreen);
Vector2 PosToScreen(Vector3 pos);
Vector2 bonePosToScreen(Vector4 pos);