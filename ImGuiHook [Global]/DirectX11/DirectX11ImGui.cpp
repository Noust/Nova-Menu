#include "Include.h"
#define _HEAD_              0x796E
#define _LEFTFOOTFRONT_     0xE175
#define _RIGHTFOOTFRONT_	0x60E6
#define _LEFTFOOTBACK_      0x3779
#define _RIGHTFOOTBACK_     0xCC4D
#define _LEFTHAND_	      0x49D9
#define _RIGHTHAND_         0xDEAD
#define _NECK_              0x9995
#define _STOMACH_	       0x60F0
bool ShowMenu = false;
bool ImGui_Initialised = false;
Entity E;
DWORD64 BoneFunc;
char healths[50];
char Distances[50];
char CurrentPointPos[50];
char YC[50];
char ZC[50];
char NPCT[] = "NPC";
char PLAYERT[] = "PLAYER";
Vector2 SnapLineBegin = { 1920 / 2,1080 };

void Colors() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(10, 10);
	style.WindowRounding = 5.0f;
	style.ChildRounding = 5.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;
	style.WindowMinSize = ImVec2(783, 508);
	style.WindowTitleAlign = { 0.5,0.5f };
}
//Address of signature = GTA5.exe + 0x007904F0
//"\x41\x81\xE8\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xB8", "xxx????xx????x"
//"41 81 E8 ? ? ? ? 0F 84 ? ? ? ? B8"

DWORD WINAPI InitiateHooks(HMODULE hMod) {
	while (!hooked) {
		char modulename[] = "GTA5.exe";
		char sig[] = "\xF3\x0F\x00\x00\x00\x0F\x28\x00\x0F\x28\x00\x00\x0F\x28\x00\x00\xF3\x0F";
		char mask[] = "xx???xx?xx??xx??xx";
		char sig1[] = "\x41\x81\xE8\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xB8";
		char mask1[] = "xxx????xx????x";
		char sig2[] = "\x66\x45\x00\x00\x00\x74\x00\x41\x0F\x00\x00\x00\x49\x8B";
		char mask2[] = "xx???x?xx???xx";
		char sig3[] = "\x89\x5F\x00\x48\x8B\x00\x00\x00\x48\x8B\x00\x00\x00\x48\x8B\x00\x00\x00\x48\x83\xC4\x00\x5F\xC3\xCC\x2D";
		char mask3[] = "xx?xx???xx???xx???xxx?xxxx";
		HookAddr = FindPattern(modulename, sig, mask);
		BoneFunc = FindPattern(modulename, sig1, mask1);
		PatchAddr = FindPattern(modulename, sig2, mask2);
		PatchAddr1 = FindPattern(modulename, sig3, mask3);
		int HookLength = 16;
		jmpback = HookAddr + HookLength;
		if (HookAddr != NULL && BoneFunc != NULL && PatchAddr != NULL && PatchAddr1 != NULL) {
			Hook((BYTE*)HookAddr, (BYTE*)GetView, HookLength);
			hooked = true;
		}
	}
	while (!GetAsyncKeyState(VK_NUMPAD1)) {
		Sleep(500);
	}
	FreeLibraryAndExitThread(hMod, 0);
}

Vector2 GetBonePos(int64_t EntityAddr, int32_t mask) {
	Vector4 posbone;
	reinterpret_cast<void* (__fastcall*)(int64_t, Vector4*, int32_t)>(BoneFunc)(EntityAddr, &posbone, mask);
	return PosToScreen({ posbone.x,posbone.y,posbone.z });
}

namespace Process {
	DWORD ID;
	HANDLE Handle;
	HWND Hwnd;
	HMODULE Module;
	WNDPROC WndProc;
	int WindowWidth;
	int WindowHeight;
	LPCSTR Title;
	LPCSTR ClassName;
	LPCSTR Path;
}

namespace DirectX11Interface {
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
	ID3D11RenderTargetView* RenderTargetView;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ShowMenu) {
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
}

HRESULT APIENTRY MJPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (!ImGui_Initialised) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DirectX11Interface::Device))){
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			DirectX11Interface::Device->GetImmediateContext(&DirectX11Interface::DeviceContext);

			DXGI_SWAP_CHAIN_DESC Desc;
			pSwapChain->GetDesc(&Desc);
			WindowHwnd = Desc.OutputWindow;

			ID3D11Texture2D* BackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
			DirectX11Interface::Device->CreateRenderTargetView(BackBuffer, NULL, &DirectX11Interface::RenderTargetView);
			BackBuffer->Release();

			ImGui_ImplWin32_Init(WindowHwnd);
			ImGui_ImplDX11_Init(DirectX11Interface::Device, DirectX11Interface::DeviceContext);
			ImGui_ImplDX11_CreateDeviceObjects();
			ImGui::GetIO().ImeWindowHandle = Process::Hwnd;
			Process::WndProc = (WNDPROC)SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
			ImGui_Initialised = true;
		}
	}
	Colors();
	if (GetAsyncKeyState(VK_INSERT) & 1) ShowMenu = !ShowMenu;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = ShowMenu;
	if (ShowMenu == true) {
		ImGui::Begin("Nova", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::BeginChild("##leftside", ImVec2(200, ImGui::GetContentRegionAvail().y), true);
		if (ImGui::Button("Visuals", ImVec2(ImGui::GetContentRegionAvail().x, 25))) {
			UserSettings.MenuWindow = 0;
		}
		if (ImGui::Button("Player", ImVec2(ImGui::GetContentRegionAvail().x, 25))) {
			UserSettings.MenuWindow = 1;
		}
		if (ImGui::Button("Teleport", ImVec2(ImGui::GetContentRegionAvail().x, 25))) {
			UserSettings.MenuWindow = 2;
		}
		if (ImGui::Button("Disable All", ImVec2(ImGui::GetContentRegionAvail().x, 25))) {
			UserSettings.Godmode = false;
			UserSettings.CarGodMode = false;
			UserSettings.NeverWanted = false;
			UserSettings.InfAmmo = false;
			UserSettings.nocarcollision = false;
			UserSettings.KillAura = false;
			UserSettings.CustomValues = false;
			UserSettings.Name = false;
			UserSettings.Distance = false;
			UserSettings.HP = false;
			UserSettings.Type = false;
			UserSettings.SnapLine = false;
			UserSettings.Esp3d = false;
			UserSettings.BoxEsp = false;
			UserSettings.BoneEsp = false;
		}
		if (ImGui::Button("Best Configuration", ImVec2(ImGui::GetContentRegionAvail().x, 25))) {
			UserSettings.Godmode = true;
			UserSettings.CarGodMode = true;
			UserSettings.NeverWanted = true;
			UserSettings.InfAmmo = true;
			UserSettings.nocarcollision = true;
			UserSettings.CustomValues = true;
			UserSettings.carGravity = 79.0f;
			UserSettings.Name = true;
			UserSettings.Distance = true;
			UserSettings.HP = true;
			UserSettings.Type = true;
			UserSettings.BoxEsp = true;
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##rigthside", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
		if (UserSettings.MenuWindow == 0) {
			ImGui::Checkbox("Bone Esp",&UserSettings.BoneEsp);
			if (UserSettings.BoneEsp) {
				ImGui::ColorEdit4("Bone Player Color", (float*)(&UserSettings.PlayerBoneColor));
				ImGui::ColorEdit4("Bone NPC Color", (float*)(&UserSettings.NPCBoneColor));
				ImGui::SliderInt("Bone thickness", &UserSettings.BoneThickness, 0, 10);
				ImGui::Separator();
			}
			ImGui::Checkbox("Box Esp", &UserSettings.BoxEsp);
			if (UserSettings.BoxEsp) {
				ImGui::ColorEdit4("Box Player Color", (float*)(&UserSettings.PlayerBoxColor));
				ImGui::ColorEdit4("Box NPC Color", (float*)(&UserSettings.NPCBoxColor));
				ImGui::SliderInt("Box thickness", &UserSettings.BoxThickness, 0, 10);
				ImGui::SliderFloat("Box Width", &UserSettings.BoxWidth, 0, 1);
				ImGui::Separator();
			}
			ImGui::Checkbox("3D Box Esp", &UserSettings.Esp3d);
			if (UserSettings.Esp3d) {
				ImGui::ColorEdit4("3D Box Player Color", (float*)(&UserSettings.Player3dColor));
				ImGui::ColorEdit4("3D Box NPC Color", (float*)(&UserSettings.NPC3dColor));
				ImGui::SliderInt("3D Box thickness", &UserSettings.Thickness3d, 0, 10);
				ImGui::Separator();
			}
			ImGui::Checkbox("Snap Line Esp", &UserSettings.SnapLine);
			if (UserSettings.SnapLine) {
				ImGui::ColorEdit4("Snap Player Color", (float*)(&UserSettings.PlayerSnapColor));
				ImGui::ColorEdit4("Snap NPC Color", (float*)(&UserSettings.NPCSnapColor));
				ImGui::SliderInt("Snap thickness", &UserSettings.ThicknessSnap, 0, 10);
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Snap Line begin place");
				if (ImGui::Button("Bottom Center")) {
					SnapLineBegin = { 1920 / 2,1080 };
				}
				if (ImGui::Button("Center")) {
					SnapLineBegin = { 1920 / 2,1080 / 2 };
				}
				if (ImGui::Button("Top Center")) {
					SnapLineBegin = { 1920 / 2,0 };
				}
				if (ImGui::Button("Center Right")) {
					SnapLineBegin = { 1920,1080 / 2 };
				}
				if (ImGui::Button("Center Left")) {
					SnapLineBegin = { 0,1080 / 2 };
				}
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Snap Line end place");
				if (ImGui::Button("Top")) {
					UserSettings.bottom = false;
					UserSettings.top = true;
				}
				if (ImGui::Button("Bottom")) {
					UserSettings.bottom = true;
					UserSettings.top = false;
				}
				ImGui::Separator();
			}
			ImGui::Checkbox("Type Esp", &UserSettings.Type);
			if (UserSettings.Type) {
				ImGui::ColorEdit4("Type Player Color", (float*)(&UserSettings.PlayerTypeColor));
				ImGui::ColorEdit4("Type NPC Color", (float*)(&UserSettings.NPCTypeColor));
				ImGui::Separator();
			}
			ImGui::Checkbox("HP Esp", &UserSettings.HP);
			if (UserSettings.HP) {
				ImGui::ColorEdit4("HP Player Color", (float*)(&UserSettings.PlayerHPColor));
				ImGui::ColorEdit4("HP NPC Color", (float*)(&UserSettings.NPCHPColor));
				ImGui::Separator();
			}
			ImGui::Checkbox("Distance Esp", &UserSettings.Distance);
			if (UserSettings.Distance) {
				ImGui::ColorEdit4("Distance Player Color", (float*)(&UserSettings.PlayerDistanceColor));
				ImGui::ColorEdit4("Distance NPC Color", (float*)(&UserSettings.NPCDistanceColor));
				ImGui::Separator();
			}
			ImGui::Checkbox("Name Esp", &UserSettings.Name);
			if (UserSettings.Name) {
				ImGui::ColorEdit4("Name Player Color", (float*)(&UserSettings.PlayerNameColor));
				ImGui::Separator();
			}
			ImGui::Separator();
			ImGui::SliderFloat("ESP Distance", &UserSettings.ESPDistance, 1, 150);
		}
		if (UserSettings.MenuWindow == 1) {
			ImGui::Checkbox("GodMode", &UserSettings.Godmode);
			ImGui::Checkbox("Car GodMode", &UserSettings.CarGodMode);
			ImGui::Checkbox("Never Wanted", &UserSettings.NeverWanted);
			ImGui::Checkbox("Infinite Ammo", &UserSettings.InfAmmo);
			ImGui::Checkbox("No Car Collision", &UserSettings.nocarcollision);
			ImGui::Checkbox("Kill Aura", &UserSettings.KillAura);
			if (UserSettings.KillAura) {
				ImGui::SliderFloat("Kill Aura Dist", &UserSettings.KillAuraDist, 1, 300);
			}
			if (ImGui::Button("Give Ammo")) {
				if (E.Alive()) {
					local->Weapon->Weaponptr->Weaponptr2->Weaponptr3->Weaponptr4->CurrentAmmo = 9999;
				}
			}
			if (ImGui::Button("Car Max Health")) {
				if (E.CarExist()) {
					local->CarPtr->Health = 1000.0f;
				}
			}
			if (ImGui::Button("Max Health")) {
				if (E.CarExist()) {
					local->Health = local->MaxHealth;
				}
			}
			ImGui::Checkbox("Custom Values", &UserSettings.CustomValues);
			if (UserSettings.CustomValues) {
				if (ImGui::Button("Default")) {
					UserSettings.runSpeed = 1;
					UserSettings.SwimSpeed = 1;
					UserSettings.caracceleration = 1;
					UserSettings.carGravity = 9.800000191f;
				}
				ImGui::SliderFloat("RunSpeed", &UserSettings.runSpeed, 0, 50);
				ImGui::SliderFloat("SwimSpeed", &UserSettings.SwimSpeed, 0, 50);
				ImGui::SliderFloat("Car Acceleration", &UserSettings.caracceleration, 0, 100);
				ImGui::SliderFloat("Car Gravity", &UserSettings.carGravity, 0, 150);
			}
		}
		if (UserSettings.MenuWindow == 2) {
			if (ImGui::Button("Waypoint", ImVec2(100, 25))) {

			}
			ImGui::SameLine();
			if (ImGui::Button("TP to car", ImVec2(100, 25))) {
				if (E.CarExist() && !E.IsInCar()) {
					Vector3 pos = local->CarPtr->CarPos->CarRealPos;
					pos.z += 1.5f;
					E.SetPos(pos);
				}
			}
			ImGui::Separator();
			if (ImGui::Button("Places")) {
				UserSettings.TpWindowMenu = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("Interiors")) {
				UserSettings.TpWindowMenu = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Custom XYZ")) {
				UserSettings.XYZCustom = local->RealP->Realpos;
				UserSettings.TpWindowMenu = 2;
			}
			ImGui::Separator();
			ImGui::BeginChild("##tpmenu", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
			if (UserSettings.TpWindowMenu == 0) {
				if (ImGui::Button("Ammunation")) {
					E.SetPos({ 16.65800095f, -1116.914917f, 29.79118919f });
				}
				if (ImGui::Button("Barber Shop")) {
					E.SetPos({ -1293.230957f, -1117.001953f, 6.639526844f });
				}
				if (ImGui::Button("Bennys MotorWorks")) {
					E.SetPos({ -205.3417816f, -1305.828491f, 31.34135437f });
				}
				if (ImGui::Button("LSC")) {
					E.SetPos({ -367.7335815f, -130.8587646f, 38.66579056f });
				}
				if (ImGui::Button("Ponsonbys Clothing")) {
					E.SetPos({ -153.4320679f, -306.0756836f, 38.71454239f });
				}
				if (ImGui::Button("Tatto Parlor")) {
					E.SetPos({ 319.7876892f, 172.2310944f, 103.7451019f });
				}
				if (ImGui::Button("Airport Entrance")) {
					E.SetPos({ -1034.599976f, -2733.600098f, 13.77630424f });
				}
				if (ImGui::Button("Airport Runaway")) {
					E.SetPos({ -1336.0f, -3044.0f, 13.92542267f });
				}
				if (ImGui::Button("Altruist Cult Camp")) {
					E.SetPos({ -1170.840942f, 4926.645996f, 224.2950134f });
				}
				if (ImGui::Button("Calafia Train Bridge")) {
					E.SetPos({ -517.8690186f, 4425.28418f, 89.79501343f });
				}
				if (ImGui::Button("Cargo Ship")) {
					E.SetPos({ 899.6779785f, -2882.190918f, 19.01298904f });
				}
				if (ImGui::Button("Chumash Historic Family Pier")) {
					E.SetPos({ -3426.683105f, 967.7379761f, 8.346889496f });
				}
				if (ImGui::Button("Del Perro Pier")) {
					E.SetPos({ -1850.126953f, -1231.750977f, 13.01709843f });
				}
				if (ImGui::Button("Devin Westons House")) {
					E.SetPos({ -2639.87207f, 1866.812012f, 160.1347656f });
				}
				if (ImGui::Button("El Burro Heights")) {
					E.SetPos({ 1384.0f, -2057.100098f, 51.99945831f });
				}
				if (ImGui::Button("Elysian Island")) {
					E.SetPos({ 338.2000122f, -2715.899902f, 38.46731186f });
				}
				if (ImGui::Button("Far North San Andrea")) {
					E.SetPos({ 24.77499962f, 7644.102051f, 19.05554962f });
				}
				if (ImGui::Button("Ferris Wheel")) {
					E.SetPos({ -1670.699951f, -1125.0f, 13.03826141f });
				}
				if (ImGui::Button("Fort Zancudo(Opened)")) {
					E.SetPos({ -2047.400024f, 3132.100098f, 32.80938721f });
				}
				if (ImGui::Button("Fort Zancudo Tower")) {
					E.SetPos({ -2358.13208f, 3249.753906f, 101.4508438f });
				}
				if (ImGui::Button("Jetsam")) {
					E.SetPos({ 760.4000244f, -2943.199951f, 5.800281525f });
				}
				if (ImGui::Button("Jolene Cranley-Evans Ghost")) {
					E.SetPos({ 3059.620117f, 5564.246094f, 197.0906067f });
				}
				if (ImGui::Button("Korts Center")) {
					E.SetPos({ -2243.810059f, 264.0480042f, 174.6151276f });
				}
				if (ImGui::Button("Marlowe Vineyards")) {
					E.SetPos({ -1868.970947f, 2095.674072f, 139.115036f });
				}
				if (ImGui::Button("McKenzie Airfield")) {
					E.SetPos({ 2121.699951f, 4796.299805f, 41.10608292f });
				}
				if (ImGui::Button("Merryweather Dock")) {
					E.SetPos({ 486.4169922f, -3339.691895f, 6.069966793f });
				}
				if (ImGui::Button("MineShaft")) {
					E.SetPos({ -595.34198f, 2086.008057f, 131.4120636f });
				}
				if (ImGui::Button("Mount Chiliad")) {
					E.SetPos({ 450.7179871f, 5566.61377f, 806.1831055f });
				}
				if (ImGui::Button("Life Invader Building Roof")) {
					E.SetPos({ -1076.370239f, -247.4655609f, 57.92191315f });
				}
				if (ImGui::Button("NOOSE Headquarters")) {
					E.SetPos({ 2535.24292f, -383.7990112f, 92.99293518f });
				}
				if (ImGui::Button("Pacific Standard Bank")) {
					E.SetPos({ 235.0460052f, 216.4340057f, 106.28685f });
				}
				if (ImGui::Button("Paleto Bay Pier")) {
					E.SetPos({ -275.5220032f, 6635.834961f, 7.425158024f });
				}
				if (ImGui::Button("Play Boy Mansion")) {
					E.SetPos({ -1475.234009f, 167.0879974f, 55.84107971f });
				}
				if (ImGui::Button("Police Station")) {
					E.SetPos({ 436.4909973f, -982.1719971f, 30.69884491f });
				}
				if (ImGui::Button("Quarry")) {
					E.SetPos({ 2954.196045f, 2783.409912f, 41.0033989f });
				}
				if (ImGui::Button("Snady Shores Airfield")) {
					E.SetPos({ 1747.0f, 3273.699951f, 41.13830948f });
				}
				if (ImGui::Button("Satelite Dishes")) {
					E.SetPos({ 2062.123047f, 2942.054932f, 47.43084335f });
				}
				if (ImGui::Button("Sisyphus Theater Stage")) {
					E.SetPos({ 686.2449951f, 577.9500122f, 130.4610291f });
				}
				if (ImGui::Button("Meth Lab")) {
					E.SetPos({ 1391.772949f, 3608.716064f, 38.9419632f });
				}
				if (ImGui::Button("Weed Farm")) {
					E.SetPos({ 2208.7771f, 5578.234863f, 53.73509979f });
				}
				if (ImGui::Button("Wind Farm")) {
					E.SetPos({ 2354.0f, 1830.300049f, 101.1442108f });
				}
				if (ImGui::Button("Crane Top")) {
					E.SetPos({ -119.8789978f, -977.3569946f, 304.2489929f });
				}
				if (ImGui::Button("Fib Building Roof")) {
					E.SetPos({ 150.1260071f, -754.5910034f, 262.8649902f });
				}
				if (ImGui::Button("Galileo Observatory Roof")) {
					E.SetPos({ -438.8039856f, 1076.097046f, 352.4108276f });
				}
				if (ImGui::Button("IAA Building Roof")) {
					E.SetPos({ 134.0850067f, -637.8590088f, 262.8510132f });
				}
				if (ImGui::Button("Maze Bank Roof")) {
					E.SetPos({ -75.01499939f, -818.2150269f, 326.1759949f });
				}
				if (ImGui::Button("Palmer-Taylor Power Station Chimney")) {
					E.SetPos({ 2732.930908f, 1577.540039f, 83.67098236f });
				}
				if (ImGui::Button("Sandy Shores Building Site Crane")) {
					E.SetPos({ 1051.208984f, 2280.451904f, 89.72715759f });
				}
				if (ImGui::Button("Satelite Dish")) {
					E.SetPos({ 2034.988037f, 2953.10498f, 74.60115051f });
				}
				if (ImGui::Button("UFO (Fort Zancudo)")) {
					E.SetPos({ -2052.0f, 3237.0f, 1456.974731f });
				}
				if (ImGui::Button("Windmill Top")) {
					E.SetPos({ 2026.696655f, 1842.723999f, 136.3067322f });
				}
				if (ImGui::Button("Train Crash Location")) {
					E.SetPos({ -532.1309204f, 4526.187012f, 89.79542542f });
				}
				if (ImGui::Button("Plane Crash Location")) {
					E.SetPos({ 2814.518066f, 4758.557129f, 47.02832413f });
				}
				if (ImGui::Button("Strip Club")) {
					E.SetPos({ 131.5956573f, -1304.09082f, 29.21956253f });
				}
				if (ImGui::Button("Gangs")) {
					E.SetPos({ 96.82131195f, -1952.354614f, 20.76452637f });
				}
				if (ImGui::Button("VineWood")) {
					E.SetPos({ 847.9818115f,1251.696045f,356.2430115f });
				}
				if (ImGui::Button("Casino")) {
					E.SetPos({ 921.6513672f,48.96141052f,80.89820099f });
				}
				if (ImGui::Button("Casino Roof")) {
					E.SetPos({ 914.4961548f,57.32708359f,111.6613159f });
				}
				if (ImGui::Button("Casino landing zone")) {
					E.SetPos({ 966.3134155f,44.17022705f,123.1232147f });
				}
				if (ImGui::Button("Suburban Clothing")) {
					E.SetPos({ -1207.556152f,-783.2567139f,17.09370613f });
				}
				if (ImGui::Button("Bunker")) {
					E.SetPos({ -736.9771118f,5953.104492f,18.09233284f });
				}
			}
			if (UserSettings.TpWindowMenu == 1) {
				if (ImGui::Button("Ammunation Gun Range")) {
					E.SetPos({ 22.15299988f, -1072.854004f, 29.79700089f });
				}
				if (ImGui::Button("Bank (Blaine Country)")) {
					E.SetPos({ -109.2990036f, 6464.035156f, 31.626894f });
				}
				if (ImGui::Button("Bank Vault")) {
					E.SetPos({ 255.8509979f, 217.0299988f, 101.6831741f });
				}
				if (ImGui::Button("Car Showroom")) {
					E.SetPos({ -57.66149902f, -1097.594971f, 26.42238426f });
				}
				if (ImGui::Button("FIB Building Burnt")) {
					E.SetPos({ 159.5529938f, -738.8510132f, 246.1520233f });
				}
				if (ImGui::Button("FIB Building Floor 47")) {
					E.SetPos({ 134.572998f, -766.4860229f, 234.1519928f });
				}
				if (ImGui::Button("FIB Building Floor 49")) {
					E.SetPos({ 134.6349945f, -765.8309937f, 242.1519928f });
				}
				if (ImGui::Button("FIB Building Top Floor")) {
					E.SetPos({ 135.7330017f, -749.2160034f, 258.1520081f });
				}
				if (ImGui::Button("Humane Labs Lower Level")) {
					E.SetPos({ 3525.495117f, 3705.301025f, 20.99192047f });
				}
				if (ImGui::Button("Humane Labs Upper Level")) {
					E.SetPos({ 3618.52002f, 3755.76001f, 28.52216148f });
				}
				if (ImGui::Button("IAA Office")) {
					E.SetPos({ 117.2200012f, -620.9379883f, 206.0469971f });
				}
				if (ImGui::Button("Janitor`s Apartment")) {
					E.SetPos({ -110.7210007f, -8.220950127f, 70.51968384f });
				}
				if (ImGui::Button("Lester`s House")) {
					E.SetPos({ 1273.897949f, -1719.303955f, 54.77117538f });
				}
				if (ImGui::Button("Motel Room")) {
					E.SetPos({ 152.2599945f, -1004.469971f, -99.0f });
				}
				if (ImGui::Button("Omega`s Garage")) {
					E.SetPos({ 2330.619629f, 2572.61084f, 46.68009949f });
				}
				if (ImGui::Button("Strip Club, Behind Bar")) {
					E.SetPos({ 126.1211014f, -1278.512939f, 29.26957703f });
				}
				if (ImGui::Button("Torture Room")) {
					E.SetPos({ 136.5140076f, -2203.149902f, 7.309140205f });
				}
				if (ImGui::Button("Michaels House")) {
					E.SetPos({ -813.6030273f, 179.473999f, 72.15540314f });
				}
				if (ImGui::Button("Franklins House(New)")) {
					E.SetPos({ 7.119029999f, 536.6149902f, 176.0279999f });
				}
				if (ImGui::Button("Franklins House(Old)")) {
					E.SetPos({ -14.38029957f, -1438.51001f, 31.1048336f });
				}
				if (ImGui::Button("Trevors House")) {
					E.SetPos({ 1972.609985f, 3817.040039f, 33.42813873f });
				}
				if (ImGui::Button("Trevors House 2")) {
					E.SetPos({ -1151.77002f, -1518.140015f, 10.63270569f });
				}
				if (ImGui::Button("Trevors Office")) {
					E.SetPos({ 97.27069855f, -1290.994019f, 29.26879883f });
				}
			}
			if (UserSettings.TpWindowMenu == 2) {
				sprintf_s(CurrentPointPos, 50, "Current Point Pos: X:%f", UserSettings.PointPos.x);
				sprintf_s(YC, 50, " Y:%f", UserSettings.PointPos.z);
				sprintf_s(ZC, 50, " Z:%f", UserSettings.PointPos.y);
				if (ImGui::Button("Update Current")) {
					UserSettings.XYZCustom = local->RealP->Realpos;
				}
				ImGui::InputFloat("X Cord", &UserSettings.XYZCustom.x);
				ImGui::InputFloat("Y Cord", &UserSettings.XYZCustom.z);
				ImGui::InputFloat("Z Cord", &UserSettings.XYZCustom.y);
				if (ImGui::Button("Apply")) {
					E.SetPos(UserSettings.XYZCustom);
				}
				if (ImGui::Button("Set Point")) {
					UserSettings.PointPos = local->RealP->Realpos;
				}
				if (ImGui::Button("Go To Point")) {
					E.SetPos(UserSettings.PointPos);
				}
				ImGui::Text(CurrentPointPos);
				ImGui::SameLine();
				ImGui::Text(YC);
				ImGui::SameLine();
				ImGui::Text(ZC);
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
		ImGui::End();
	}
	if (hooked) {
		if (UserSettings.BoneEsp || UserSettings.BoxEsp || UserSettings.Esp3d || UserSettings.SnapLine || UserSettings.Type || UserSettings.Distance || UserSettings.HP || UserSettings.Name) {
			for (int i = 0; i < E.GetMaxEntities(); i++) {
				int64_t EntityAddr = E.GetEntity(i);
				ents = (Entitys*)(EntityAddr);
				if (EntityAddr != 0 && local != 0) {
					float maxhealth = ents->MaxHealth;
					float health = ents->Health;
					if (maxhealth > 11 && maxhealth < 999 && health != 0) {
						Vector3 pos = ents->pos;
						if (local->pos.Distance(pos) < UserSettings.ESPDistance){
							pos.z -= 1.0f;
							Vector3 pos1 = ents->pos;
							pos1.z += 0.8f;
							Vector3 pos2 = ents->pos;
							if (UserSettings.top)
								pos2.z += 0.8f;
							if (UserSettings.bottom)
								pos2.z -= 1.0f;
							Vector2 posscreen = PosToScreen(pos);
							Vector2 posscreen1 = PosToScreen(pos1);
							Vector2 posscreen2 = PosToScreen(pos2);
							if (UserSettings.BoneEsp) {
								if (posscreen.x > 0 && posscreen.y > 0 && posscreen.x < 1920 && posscreen.y < 1080) {
									Vector2 neck = GetBonePos(EntityAddr, _NECK_);
									Vector2 head = GetBonePos(EntityAddr, _HEAD_);
									Vector2 Rhand = GetBonePos(EntityAddr, _RIGHTHAND_);
									Vector2 Lhand = GetBonePos(EntityAddr, _LEFTHAND_);
									Vector2 stomach = GetBonePos(EntityAddr, _STOMACH_);
									Vector2 Rfootback = GetBonePos(EntityAddr, _RIGHTFOOTBACK_);
									Vector2 Lfootback = GetBonePos(EntityAddr, _LEFTFOOTBACK_);
									Vector2 Rfootfront = GetBonePos(EntityAddr, _RIGHTFOOTFRONT_);
									Vector2 Lfootfront = GetBonePos(EntityAddr, _LEFTFOOTFRONT_);
									if (maxhealth > 11 && maxhealth < 201) {
										DrawLine(neck, stomach, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
										DrawLine(neck, Rhand, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
										DrawLine(neck, Lhand, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
										DrawLine(stomach, Rfootback, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
										DrawLine(stomach, Lfootback, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
										DrawLine(Lfootback, Lfootfront, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
										DrawLine(Rfootback, Rfootfront, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
										DrawCircle(head, UserSettings.NPCBoneColor, (neck.y - head.y) + 2, UserSettings.BoneThickness);
									}
									if (maxhealth > 201 && maxhealth < 999) {

										DrawLine(neck, stomach, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(neck, Rhand, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(neck, Lhand, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(stomach, Rfootback, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(stomach, Lfootback, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(Lfootback, Lfootfront, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(Rfootback, Rfootfront, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawCircle(head, UserSettings.PlayerBoneColor, (neck.y - head.y) + 2, UserSettings.BoneThickness);
									}
								}
							}
							if (UserSettings.BoxEsp) {
								if (posscreen.x > 0 && posscreen.y > 0 && posscreen.x < 1920 && posscreen.y < 1080) {
									float heigth = posscreen.y - posscreen1.y;
									if (maxhealth > 11 && maxhealth < 201) {
										DrawRect(posscreen, heigth, heigth * UserSettings.BoxWidth, UserSettings.NPCBoxColor, UserSettings.BoxThickness);
									}
									if (maxhealth > 201 && maxhealth < 999) {
										DrawRect(posscreen, heigth, heigth * UserSettings.BoxWidth, UserSettings.PlayerBoxColor, UserSettings.BoxThickness);
									}
								}
							}
							if (UserSettings.Esp3d) {
								if (posscreen.x > 0 && posscreen.y > 0 && posscreen.x < 1920 && posscreen.y < 1080) {
									if (maxhealth > 11 && maxhealth < 201) {
										DrawBox3d(E.GetEntity(i), UserSettings.NPC3dColor, UserSettings.Thickness3d);
									}
									if (maxhealth > 201 && maxhealth < 999) {
										DrawBox3d(E.GetEntity(i), UserSettings.Player3dColor, UserSettings.Thickness3d);
									}
								}
							}
							if (UserSettings.SnapLine) {
								if (posscreen2.x > 0 && posscreen2.y > 0 && posscreen2.x < 1920 && posscreen2.y < 1080) {
									if (maxhealth > 11 && maxhealth < 201) {
										DrawLine(SnapLineBegin, posscreen2, UserSettings.NPCSnapColor, UserSettings.ThicknessSnap);
									}
									if (maxhealth > 201 && maxhealth < 999) {
										DrawLine(SnapLineBegin, posscreen2, UserSettings.PlayerSnapColor, UserSettings.ThicknessSnap);
									}
								}
							}
							if (UserSettings.Type) {
								Vector3 pos3 = ents->pos;
								pos3.z += 1.2f;
								Vector2 posscreen3 = PosToScreen(pos3);
								if (posscreen.x > 0 && posscreen.y > 0 && posscreen.x < 1920 && posscreen.y < 1080) {
									if (maxhealth > 11 && maxhealth < 201) {
										DrawChar(posscreen3, NPCT, UserSettings.NPCTypeColor, 1);
									}
									if (maxhealth > 201 && maxhealth < 999) {
										DrawChar(posscreen3, PLAYERT, UserSettings.PlayerTypeColor, 1);
									}
								}
							}
							if (UserSettings.HP) {
								Vector3 pos3 = ents->pos;
								pos3.z += 1.0f;
								Vector2 posscreen3 = PosToScreen(pos3);
								if (posscreen.x > 0 && posscreen.y > 0 && posscreen.x < 1920 && posscreen.y < 1080) {
									sprintf_s(healths, 50, "HP:%0.f", health);
									if (maxhealth > 11 && maxhealth < 201) {
										DrawChar(posscreen3, healths, UserSettings.NPCHPColor, 2);
									}
									if (maxhealth > 201 && maxhealth < 999) {
										DrawChar(posscreen3, healths, UserSettings.PlayerHPColor, 2);
									}
								}
							}
							if (UserSettings.Distance) {
								Vector3 pos3 = ents->pos;
								pos3.z -= 1.2f;
								Vector2 posscreen3 = PosToScreen(pos3);
								if (posscreen.x > 0 && posscreen.y > 0 && posscreen.x < 1920 && posscreen.y < 1080) {
									sprintf_s(Distances, 50, "[%0.fm]", local->pos.Distance(pos));
									if (maxhealth > 11 && maxhealth < 201) {
										DrawChar(posscreen3, Distances, UserSettings.NPCDistanceColor, 2);
									}
									if (maxhealth > 201 && maxhealth < 999) {
										DrawChar(posscreen3, Distances, UserSettings.PlayerDistanceColor, 1.5f);
									}
								}
							}
							if (UserSettings.Name) {
								Vector3 pos3 = ents->pos;
								pos3.z -= 1.4f;
								Vector2 posscreen3 = PosToScreen(pos3);
								if (posscreen.x > 0 && posscreen.y > 0 && posscreen.x < 1920 && posscreen.y < 1080) {
									if (maxhealth > 201 && maxhealth < 999) {
										DrawChar(posscreen3, ents->PlayerInfoPtr->Names, UserSettings.PlayerNameColor, 6);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	ImGui::EndFrame();
	ImGui::Render();
	DirectX11Interface::DeviceContext->OMSetRenderTargets(1, &DirectX11Interface::RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(HMODULE hMod) {
	bool WindowFocus = false;
	while (WindowFocus == false) {
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (GetCurrentProcessId() == ForegroundWindowProcessID) {

			Process::ID = GetCurrentProcessId();
			Process::Handle = GetCurrentProcess();
			Process::Hwnd = GetForegroundWindow();

			RECT TempRect;
			GetWindowRect(Process::Hwnd, &TempRect);
			Process::WindowWidth = TempRect.right - TempRect.left;
			Process::WindowHeight = TempRect.bottom - TempRect.top;

			char TempTitle[MAX_PATH];
			GetWindowText(Process::Hwnd, TempTitle, sizeof(TempTitle));
			Process::Title = TempTitle;

			char TempClassName[MAX_PATH];
			GetClassName(Process::Hwnd, TempClassName, sizeof(TempClassName));
			Process::ClassName = TempClassName;

			char TempPath[MAX_PATH];
			GetModuleFileNameEx(Process::Handle, NULL, TempPath, sizeof(TempPath));
			Process::Path = TempPath;

			WindowFocus = true;
		}
	}
	bool InitHook = false;
	while (InitHook == false) {
		if (DirectX11::Init() == true) {
		    CreateHook(8, (void**)&oIDXGISwapChainPresent, MJPresent);
			InitHook = true;
		}
	}
	while (!GetAsyncKeyState(VK_NUMPAD1)) {
		Sleep(500);
	}
	if (HookAddr != NULL && PatchAddr != NULL && PatchAddr1 != NULL) {
		Patch((BYTE*)HookAddr, (BYTE*)"\xF3\x0F\x11\x45\x00\x0F\x28\x20\x0F\x28\x68\x10\x0F\x28\x70\x20", 16);
		Patch((BYTE*)PatchAddr, (BYTE*)"\x66\x45\x89\x53\x56", 5);
		Patch((BYTE*)PatchAddr1, (BYTE*)"\x89\x5F\x20", 3);
	}
	ShowMenu = false;
	Sleep(10);
	DisableAll();
	FreeLibraryAndExitThread(hMod, 0);
}

BOOL __stdcall StartFunc(HMODULE hMod, LPTHREAD_START_ROUTINE startaddr) {
	return CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)startaddr, hMod, 0, 0));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		if (ChecktDirectXVersion(DirectXVersion.D3D11) == true) {
			Process::Module = hModule;
			StartFunc(hModule, (LPTHREAD_START_ROUTINE)MainThread);
			StartFunc(hModule, (LPTHREAD_START_ROUTINE)InitiateHooks);
			StartFunc(hModule, (LPTHREAD_START_ROUTINE)SetValues);
		}
		break;
	default:
		break;
	}
	return TRUE;
}