#pragma once
#include "rw.h"
#include <vector>

class CResolutionList {
public:

	void AddResolution(int a2, int* resolution);
	void AddResolution_Hook(int a2, int* resolution);
};

void GetVideoModes();