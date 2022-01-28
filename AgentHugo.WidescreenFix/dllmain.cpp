// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "rw.h"
#include "IniReader.h"

using namespace Memory::VP;

float FOV;
int   ResolutionX, ResolutionY;


void RwCameraSetViewWindow_Hook(int camera, RwV2d* view)
{
	float aspectRatio = (float)ResolutionX / (float)ResolutionY;
	float _4_3 = 4.0f / 3.0f;
	float _3_4 = 3.0f / 4.0f;
	view->y *= _3_4;

	view->y *= _4_3 / aspectRatio;
	view->y /= FOV;
	view->x /= FOV;
	RwCameraSetViewWindow(camera, view);
}

void __declspec(naked) Resolution_Hook()
{
#ifdef ROBORUMBLE
	static int jmpContinue = 0x501AD2;
#else
	static int jmpContinue = 0x4B1DE8;
#endif // ROBORUMBLE

	_asm {
		jmp jmpContinue
	}
}
void Init()
{
	CIniReader ini("");

	ResolutionX = ini.ReadInteger("Settings", "ResolutionX", 0);
	ResolutionY = ini.ReadInteger("Settings", "ResolutionY", 0);

	if (ResolutionX == 0 || ResolutionY == 0)
	{
		HWND hWnd = 0;
		HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &info);
		ResolutionX = info.rcMonitor.right - info.rcMonitor.left;
		ResolutionY = info.rcMonitor.bottom - info.rcMonitor.top;
	}

	FOV = ini.ReadFloat("Settings", "FOV", 1.0f);

	float aspectRatioReversed = (float)ResolutionY / (float)ResolutionX;

#ifdef ROBORUMBLE
	InjectHook(0x501A0F, Resolution_Hook, PATCH_JUMP);
	Patch<int>(0x501AD7 + 6, ResolutionX);
	Patch<int>(0x501AE1 + 6, ResolutionY);

	Patch<float>(0x73266C, 1.0f / (float)ResolutionX * ((float)ResolutionY / 480.0f));

	Patch<float>(0x72F818, aspectRatioReversed);
	Patch<float>(0x735C60, aspectRatioReversed);

	Nop(0x514F87, 6);
	InjectHook(0x514FA3, RwCameraSetViewWindow_Hook);
#else
	InjectHook(0x4B1D25, Resolution_Hook, PATCH_JUMP);
	Patch<int>(0x4B1DED + 6, ResolutionX);
	Patch<int>(0x4B1DF7 + 6, ResolutionY);

	Patch<float>(0x6E2C24, 1.0f / (float)ResolutionX * ((float)ResolutionY / 480.0f));

	Patch<float>(0x6E0E5C, aspectRatioReversed);
	Patch<float>(0x6E59E4, aspectRatioReversed);

	Nop(0x4C59A7, 6);
	InjectHook(0x4C59C3, RwCameraSetViewWindow_Hook);
#endif // ROBORUMBLE


}



extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		Init();
	}
}
