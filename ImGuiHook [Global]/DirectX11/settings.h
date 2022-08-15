#pragma once
#include "Include.h"
class settings {
public:
	int MenuWindow;
	
	bool BoneEsp = false;
	int BoneThickness = 1;
	ImColor PlayerBoneColor = ( 0,0,255 );
	ImColor NPCBoneColor = (0, 255, 0);
	//float BoxWidth = 0.5f;

	float ESPDistance = 200;
}UserSettings;
