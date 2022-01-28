#include "pch.h"
#include "rw.h"

void RwCameraSetViewWindow(int camera, RwV2d* view)
{
#ifdef ROBORUMBLE
	((void(__cdecl*)(int, RwV2d*))0x631080)(camera, view);
#else
	((void(__cdecl*)(int, RwV2d*))0x5CEF40)(camera, view);
#endif

}
