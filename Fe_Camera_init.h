// Camera初始化程序库
// Fe_Camera_init.h
#ifndef _FE_CAMERA_INIT_H
#define _FE_CAMERA_INIT_H


#include "Fe_Camera_init.h"
#include "stdafx.h"
#include "windows.h"
#include <opencv2/opencv.hpp>

#include <process.h>

#ifdef _WIN64
#pragma comment(lib, "..\\MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "..\\MVCAMSDK.lib")
#endif
#include "..//include//CameraApi.h"

using namespace std;
using namespace cv;

//int Fe_Camera_doinit(CameraHandle &m_hCamera,BYTE* m_pFrameBuffer,char* g_CameraName,HANDLE &m_hDispThread,
					//UINT WINAPI &uiDisplayThread ,UINT &m_threadID);
int Fe_Camera_doinit(CameraHandle &m_hCamera,BYTE** m_pFrameBuffer,char* g_CameraName,HANDLE &m_hDispThread,
					UINT (WINAPI * uiDisplayThread)(LPVOID lpParam) ,UINT &m_threadID);
#endif
					
					
					