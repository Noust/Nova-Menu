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

Vector2 GetposBone(DWORD64 EntityAddr, DWORD mask) {
	Vector4 TempVec;
	reinterpret_cast<void* (__fastcall*)(DWORD64, Vector4*, DWORD)>(BoneFunc)(EntityAddr, &TempVec, mask);
	return PosToScreen(TempVec);
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
		ImGui::EndChild;
		ImGui::SameLine();
		ImGui::BeginChild("##rigthside", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
		ImGui::EndChild;
		ImGui::End;
	}
	if (hooked) {
		if (UserSettings.BoneEsp) {
			for (int i = 0; i < E.GetMaxEntities(); i++) {
				DWORD64 EntityAddr = E.GetEntity(i);
				if (EntityAddr != 0) {
					Vector2 head = GetposBone(EntityAddr, _HEAD_);
					Vector2 neck = GetposBone(EntityAddr, _NECK_);
					Vector2 stomach = GetposBone(EntityAddr, _STOMACH_);
					Vector2 Rhand = GetposBone(EntityAddr, _RIGHTHAND_);
					Vector2 Lhand = GetposBone(EntityAddr, _LEFTHAND_);
					Vector2 Rfootback = GetposBone(EntityAddr, _RIGHTFOOTBACK_);
					Vector2 Lfootback = GetposBone(EntityAddr, _LEFTFOOTBACK_);
					Vector2 Rfootfront = GetposBone(EntityAddr, _RIGHTFOOTFRONT_);
					Vector2 Lfootfront = GetposBone(EntityAddr, _LEFTFOOTFRONT_);
					DrawLine(neck, stomach, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
					DrawLine(neck, Rhand, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
					DrawLine(neck, Lhand, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
					DrawLine(stomach, Lfootback, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
					DrawLine(stomach, Rfootback, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
					DrawLine(Rfootback, Rfootfront, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
					DrawLine(Lfootback, Lfootfront, UserSettings.NPCBoneColor, UserSettings.BoneThickness);
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