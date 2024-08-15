#include "pch.h"
#include "rw.h"

void RwCameraSetViewWindow(int camera, RwV2d* view)
{
	((void(__cdecl*)(int, RwV2d*))0x5CEF40)(camera, view);
}
