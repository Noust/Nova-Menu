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
Vector2 SnapLineBegin = { 1920 / 2,1080 };

void Colors() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(10, 10);
	style.WindowRounding = 5.0f;
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
		HookAddr = FindPattern(modulename, sig, mask);
		BoneFunc = FindPattern(modulename, sig1, mask1);
		int HookLength = 16;
		jmpback = HookAddr + HookLength;
		if (HookAddr != NULL && BoneFunc != NULL) {
			Hook((BYTE*)HookAddr, (BYTE*)GetView, HookLength);
			hooked = true;
		}
	}
	while (!GetAsyncKeyState(VK_DELETE)) {
		Sleep(500);
	}
	FreeLibraryAndExitThread(hMod, 0);
}

Vector2 GetBonePos(DWORD64 EntityAddr, DWORD mask) {
	Vector4 posbone;
	reinterpret_cast<void* (__fastcall*)(DWORD64, Vector4*, DWORD)>(BoneFunc)(EntityAddr, &posbone, mask);
	return bonePosToScreen(posbone);
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
			ImGui::Checkbox("Snap Box Esp", &UserSettings.SnapLine);
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
				if (ImGui::Button("Center")) {
					UserSettings.bottom = false;
					UserSettings.top = false;
				}
				if (ImGui::Button("Bottom")) {
					UserSettings.bottom = true;
					UserSettings.top = false;
				}
				ImGui::Separator();
			}
			ImGui::SliderFloat("ESP Distance", &UserSettings.ESPDistance, 1, 200);
		}
		if (UserSettings.MenuWindow == 1) {

		}
		if (UserSettings.MenuWindow == 2) {

		}
		ImGui::EndChild();
		ImGui::End();
	}
	if (hooked) {
		if (UserSettings.BoneEsp || UserSettings.BoxEsp || UserSettings.Esp3d || UserSettings.SnapLine) {
			for (int i = 0; i < E.GetMaxEntities(); i++) {
				ents = (Entitys*)(E.GetEntity(i));
				if (ents != nullptr) {
					float maxhealth = ents->MaxHealth;
					if (maxhealth > 11 && maxhealth < 999) {
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
									DWORD64 EntityAddr = E.GetEntity(i);
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
										ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head.x, head.y), (neck.y - head.y) + 2, UserSettings.NPCBoneColor, 0, UserSettings.BoneThickness);
									}
									if (maxhealth > 201 && maxhealth < 999) {

										DrawLine(neck, stomach, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(neck, Rhand, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(neck, Lhand, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(stomach, Rfootback, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(stomach, Lfootback, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(Lfootback, Lfootfront, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										DrawLine(Rfootback, Rfootfront, UserSettings.PlayerBoneColor, UserSettings.BoneThickness);
										ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head.x, head.y), (neck.y - head.y) + 2, UserSettings.PlayerBoneColor, 0, UserSettings.BoneThickness);
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
								if (posscreen.x > 0 && posscreen.y > 0 && posscreen.x < 1920 && posscreen.y < 1080) {
									if (maxhealth > 11 && maxhealth < 201) {
										DrawLine(SnapLineBegin, posscreen2, UserSettings.NPCSnapColor, UserSettings.ThicknessSnap);
									}
									if (maxhealth > 201 && maxhealth < 999) {
										DrawLine(SnapLineBegin, posscreen2, UserSettings.PlayerSnapColor, UserSettings.ThicknessSnap);
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

void APIENTRY MJDrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) {

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
			CreateHook(12, (void**)&oID3D11DrawIndexed, MJDrawIndexed);
			InitHook = true;
		}
	}
	while (!GetAsyncKeyState(VK_DELETE)) {
		Sleep(500);
	}
	if (HookAddr != NULL) {
		Patch((BYTE*)HookAddr, (BYTE*)"\xF3\x0F\x11\x45\x00\x0F\x28\x20\x0F\x28\x68\x10\x0F\x28\x70\x20", 16);
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
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}