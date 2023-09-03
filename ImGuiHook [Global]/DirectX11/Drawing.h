#pragma once
#include "Include.h"

void DrawLine(Vector2 pos1, Vector2 pos2, ImColor color, int thickness);
void DrawRect(Vector2 pos, float height, float width, ImColor color, int thickness);
void DrawChar(Vector2 pos, const char* text, ImColor color, float divide);
void DrawBox3d(DWORD64 EntityAddr, ImColor color, int thickness);
void DrawCircle(Vector2 pos, ImColor color, float radius, int thickness);
void DrawFilledRect(Vector2 pos, float heigth, float Width, ImColor color);
void DrawCornerEsp(float W, float H, Vector2 pos, ImColor color, int thickness);