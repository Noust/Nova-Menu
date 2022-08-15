#include "Include.h"

void DrawLine(Vector2 pos1, Vector2 pos2, ImColor color, int thickness) {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), ImColor(0, 0, 0), thickness + 2);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), color, thickness);
}
void DrawRect(Vector2 pos, float height, float width, ImColor color, int thickness) {
	DrawLine({ pos.x - width / 2,pos.y }, { pos.x + width / 2,pos.y }, color, thickness);
	DrawLine({ pos.x - width / 2,pos.y }, { pos.x - width / 2,pos.y - height }, color, thickness);
	DrawLine({ pos.x - width / 2,pos.y - height }, { pos.x + width / 2,pos.y - height }, color, thickness);
	DrawLine({ pos.x + width / 2,pos.y - height }, { pos.x + width / 2,pos.y }, color, thickness);
}
void DrawChar(Vector2 pos, char* text, ImColor color) {
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize(text).x / 2, pos.y - ImGui::CalcTextSize(text).y / 2), color, text);
}