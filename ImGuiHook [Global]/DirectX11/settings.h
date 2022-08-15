#pragma once
#include "Include.h"
class settings {
public:
	int MenuWindow;
	
	bool BoxEsp = false;
	int BoxThickness = 1;
	ImColor PlayerBoxColor = ( 0,0,255 );
	ImColor NPCBoxColor = (0, 255, 0);
	float BoxWidth = 0.5f;

	float ESPDistance = 200;
}UserSettings;
