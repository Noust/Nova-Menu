#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <cstdio>
#include <vector>
#include "DirectX11.h"
#include "DirectX11ImGui.h"

#include "../Directories/ImGui/imgui.h"
#include "../Directories/ImGui/imgui_impl_dx11.h"
#include "../Directories/ImGui/imgui_impl_win32.h"
#include "Vector.h"
bool ShowMenu = false;
DWORD64 EntityList;
DWORD64 MaxEntitiesAddr;
DWORD64 OnPauseAddr;
DWORD64 MouseModeAddr;

#include "Hooks.h"
#include "settings.h"
#include "HookFunc.h"
#include "WorldToScreen.h"
#include "Drawing.h"
#include "Entity.h"
#include "MainCheats.h"