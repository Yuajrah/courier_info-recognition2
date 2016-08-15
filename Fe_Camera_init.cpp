// Camera初始化程序库
// Fe_Camera_init

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

int Fe_Camera_doinit(CameraHandle &m_hCamera,BYTE** m_pFrameBuffer,char* g_CameraName,HANDLE &m_hDispThread,
					//UINT WINAPI &uiDisplayThread ,UINT &m_threadID)
					UINT (WINAPI *uiDisplayThread)(LPVOID lpParam) ,UINT &m_threadID)
{
	tSdkCameraDevInfo sCameraList[10];
    INT iCameraNums;
    CameraSdkStatus status;
    tSdkCameraCapbility sCameraInfo;

    //枚举设备，获得设备列表
    iCameraNums = 10;//调用CameraEnumerateDevice前，先设置iCameraNums = 10，表示最多只读取10个设备，如果需要枚举更多的设备，请更改sCameraList数组的大小和iCameraNums的值

    if (CameraEnumerateDevice(sCameraList,&iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
    {
        printf("No camera was found!");
        return TRUE;
    }

    //该示例中，我们只假设连接了一个相机。因此，只初始化第一个相机。(-1,-1)表示加载上次退出前保存的参数，如果是第一次使用该相机，则加载默认参数.
    //In this demo ,we just init the first camera.
    if ((status = CameraInit(&sCameraList[0],-1,-1,&m_hCamera)) != CAMERA_STATUS_SUCCESS)
    {
        char msg[128];
        sprintf_s(msg,"Failed to init the camera! Error code is %d",status);
        printf(msg);
        return TRUE;
    }


    //Get properties description for this camera.
    CameraGetCapability(m_hCamera,&sCameraInfo);//"获得该相机的特性描述"

    *m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iWidthMax*3,16);    

    if (sCameraInfo.sIspCapacity.bMonoSensor)
    {
        CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_MONO8);
    } 

    //strcpy_s(g_CameraName,sCameraList[0].acFriendlyName);
	strcpy_s(g_CameraName ,32 , sCameraList[0].acFriendlyName);

    CameraCreateSettingPage(m_hCamera,NULL,
        g_CameraName,NULL,NULL,0);//"通知SDK内部建该相机的属性页面";
/*
#ifdef USE_CALLBACK_GRAB_IMAGE //如果要使用回调函数方式，定义USE_CALLBACK_GRAB_IMAGE这个宏
    //Set the callback for image capture
    CameraSetCallbackFunction(m_hCamera,GrabImageCallback,0,NULL);//"设置图像抓取的回调函数";
#else
	*/
    m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, uiDisplayThread, (PVOID)m_hCamera, 0,  &m_threadID);


    CameraPlay(m_hCamera);

    CameraShowSettingPage(m_hCamera,TRUE);//TRUE显示相机配置界面。FALSE则隐藏。
	return FALSE;
}