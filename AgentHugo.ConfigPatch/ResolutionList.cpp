#include "pch.h"
#include "ResolutionList.h"
#include <d3d9.h>
#include <algorithm>

std::vector<RwVideoMode> resolutions;

bool mode_compare(RwVideoMode first, RwVideoMode in)
{
	return first.height == in.height && first.width == in.width;
}


void CResolutionList::AddResolution(int a2, int* resolution)
{
	((void(__thiscall*)(CResolutionList*, int, int*))0x405670)(this, a2, resolution);
}

void CResolutionList::AddResolution_Hook(int a2, int* resolution)
{
	for (unsigned int i = 0; i < resolutions.size(); i++)
	{
		int x = resolutions[i].width;
		int y = resolutions[i].height;

		int newResolution = MAKELONG(y, x);
		AddResolution(a2, &newResolution);
	}
}

void GetVideoModes()
{
	IDirect3D9* dev = Direct3DCreate9(D3D_SDK_VERSION);

	if (dev)
	{
		for (unsigned int i = 0; i < dev->GetAdapterModeCount(0, D3DFMT_X8R8G8B8); i++)
		{
			D3DDISPLAYMODE mode;

			if (SUCCEEDED(dev->EnumAdapterModes(0, D3DFMT_X8R8G8B8, i, &mode)))
			{
				RwVideoMode rw_mode;
				rw_mode.width = mode.Width;
				rw_mode.height = mode.Height;
				rw_mode.format = mode.Format;
				rw_mode.refRate = mode.RefreshRate;
				rw_mode.flags = 0;
				resolutions.push_back(rw_mode);
			}

		}
		auto pos = std::unique(resolutions.begin(), resolutions.end(), mode_compare);
		resolutions.erase(pos, resolutions.end());

		dev->Release();
		dev = nullptr;
	}
	else
	{
		MessageBox(0, "Failed to create Direct3D device!", "Error", MB_ICONERROR);
	}
}
