// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "rw.h"

using namespace Memory::VP;

bool IsConfig()
{
	return GetModuleHandle("Config.exe");
}


void Set2DScale()
{
	int x = *(int*)0x72AB88;
	int y = *(int*)0x72AB8C;
	float aspectRatio = (float)x / (float)y;
	float aspectRatioReversed = (float)y / (float)x;

	float _4_3 = 4.0f / 3.0f;
	float scaleRatio = aspectRatio / _4_3;


	float newScale = 1.0f / (float)x * ((float)y / 480.0f);
	Patch<float>(0x6E2C24, newScale);
	Patch<float>(0x6E0E5C, aspectRatioReversed);
	Patch<float>(0x6E59E4, aspectRatioReversed);

}

void CalculateNewViewWindow(int camera, RwV2d* view)
{
	float x = (float)*(int*)0x72AB88;
	float y = (float)*(int*)0x72AB8C;
	float aspectRatio = (float)x / (float)y;

	float _4_3 = 4.0f / 3.0f;
	float scaleRatio = aspectRatio / _4_3;

	view->x *= scaleRatio;
	view->y *= scaleRatio;

	RwCameraSetViewWindow(camera, view);
}


void __declspec(naked) Set2DScale_Hook()
{
	static int jmpContinue = 0x4B1E10;

	_asm pushad
	Set2DScale();

	_asm {
		popad
		mov     al, ds:0x72AB7C
		jmp jmpContinue
	}
}


void Init()
{
	if (IsConfig())
		return;

	InjectHook(0x4B1E0B, Set2DScale_Hook, PATCH_JUMP);
	InjectHook(0x4C59C3, CalculateNewViewWindow);
}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

