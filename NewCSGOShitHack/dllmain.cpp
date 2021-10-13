﻿#include "includes.h"

#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);

	return;
}

HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!Config.ImGui_Init)
	{
		InitImGui(pDevice);
		Hack.SetCustomImGuiStyle();

		Config.ImGui_Init = true;
	}

	if (GetAsyncKeyState(Config.MenuHotKey) & 1)
		Config.MenuActive = !Config.MenuActive;

	if (Config.MenuActive)
		Hack.MenuThread();

	if (Config.WallHackESP)
		Hack.DXESPThread(pDevice);

	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE;

	window = handle;
	return FALSE;
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI KieroInit(HMODULE hModule)
{
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)&oEndScene, hkEndScene);

			do {
				window = GetProcessWindow();
			} while (window == NULL);

			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			Config.ImGui_Attached = true;
		}
	} while (!Config.ImGui_Attached);

	return TRUE;
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    MEM.UpdateOffsets();

	FILE* pFile = nullptr;
	
	if (Config.console)
		pFile = Console.Init();

    while (!GetAsyncKeyState(Config.EndHotKey))
    {
		Hack.MainThread();

		Sleep(Config.Delay);
    }

	kiero::shutdown();
	
	if (Config.console && pFile != nullptr)
		Console.Release(pFile);

    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)KieroInit, hModule, NULL, NULL));
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}

    return TRUE;
}

