// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "MemoryMgr.h"
#include "ResolutionList.h"

using namespace Memory::VP;

bool IsConfig()
{
    return GetModuleHandle("Config.exe");
}


void Init()
{
    if (!IsConfig())
        return;

    GetVideoModes();

    // config actually filters out duplicates and sorts everything nicely
    InjectHook(0x4068EC, &CResolutionList::AddResolution_Hook, PATCH_CALL);
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

