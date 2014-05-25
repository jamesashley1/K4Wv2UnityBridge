#pragma once
#pragma once
#include <iostream>
#include <Kinect.h>

#define DllExport extern "C" __declspec(dllexport)

DllExport bool Kinect_Init();

DllExport bool Kinect_Deinit();

DllExport int Poll_Color();

DllExport byte* Get_Color();

