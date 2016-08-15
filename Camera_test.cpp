// OpenCV.cpp : 定义控制台应用程序的入口点。
//
//BIG5 TRANS ALLOWED
#include "stdafx.h"
#include "windows.h"
#include "Fe_img_pro_tools.h"
#include "Fe_Camera_init.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <process.h>

#ifdef _WIN64                 
#pragma comment(lib, "..\\MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "..\\MVCAMSDK.lib")
#endif
#include "..//include//CameraApi.h"                                   


//#define _PROCESS_KEY_
            
using namespace std;
using namespace cv;

UINT            m_threadID;		//图像抓取线程的ID
HANDLE          m_hDispThread;	//图像抓取线程的句柄
BOOL            m_bExit = FALSE;		//用来通知图像抓取线程结束
CameraHandle    m_hCamera;		//相机句柄，多个相机同时使用时，可以用数组代替	
BYTE*           m_pFrameBuffer; //用于将原始图像数据转换为RGB的缓冲区
tSdkFrameHead   m_sFrInfo;		//用于保存当前图像帧的帧头信息

int	            m_iDispFrameNum;	//用于记录当前已经显示的图像帧的数量
float           m_fDispFps;			//显示帧率
float           m_fCapFps;			//捕获帧率
tSdkFrameStatistic  m_sFrameCount;
tSdkFrameStatistic  m_sFrameLast;
int					m_iTimeLast;
char		    g_CameraName[64];


/*
USE_CALLBACK_GRAB_IMAGE
如果需要使用回调函数的方式获得图像数据，则反注释宏定义USE_CALLBACK_GRAB_IMAGE.
我们的SDK同时支持回调函数和主动调用接口抓取图像的方式。两种方式都采用了"零拷贝"机制，以最大的程度的降低系统负荷，提高程序执行效率。
但是主动抓取方式比回调函数的方式更加灵活，可以设置超时等待时间等，我们建议您使用 uiDisplayThread 中的方式
*/
//#define USE_CALLBACK_GRAB_IMAGE 

#ifdef USE_CALLBACK_GRAB_IMAGE
/*图像抓取回调函数*/
IplImage *g_iplImage = NULL;

void _stdcall GrabImageCallback(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead,PVOID pContext)
{

	CameraSdkStatus status;
	

	//将获得的原始数据转换成RGB格式的数据，同时经过ISP模块，对图像进行降噪，边沿提升，颜色校正等处理。
	//我公司大部分型号的相机，原始数据都是Bayer格式的
	status = CameraImageProcess(hCamera, pFrameBuffer, m_pFrameBuffer,pFrameHead);

	//分辨率改变了，则刷新背景
	if (m_sFrInfo.iWidth != pFrameHead->iWidth || m_sFrInfo.iHeight != pFrameHead->iHeight)
	{
		m_sFrInfo.iWidth = pFrameHead->iWidth;
		m_sFrInfo.iHeight = pFrameHead->iHeight;
	}

	if(status == CAMERA_STATUS_SUCCESS )
	{
		//调用SDK封装好的显示接口来显示图像,您也可以将m_pFrameBuffer中的RGB数据通过其他方式显示，比如directX,OpengGL,等方式。
		CameraImageOverlay(hCamera, m_pFrameBuffer,pFrameHead);
		if (g_iplImage)
		{
			cvReleaseImageHeader(&g_iplImage);
		}
		g_iplImage = cvCreateImageHeader(cvSize(pFrameHead->iWidth,pFrameHead->iHeight),IPL_DEPTH_8U,sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8?1:3);
		cvSetData(g_iplImage,m_pFrameBuffer,pFrameHead->iWidth*(sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8?1:3));
		cvShowImage(g_CameraName,g_iplImage);		
		m_iDispFrameNum++;
	    waitKey(30);
	}    

	memcpy(&m_sFrInfo,pFrameHead,sizeof(tSdkFrameHead));

}

#else 
/*图像抓取线程，主动调用SDK接口函数获取图像*/
UINT WINAPI uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CameraHandle    hCamera = (CameraHandle)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;
	IplImage *iplImage = NULL;

/*******************************************************************************************
****************************************程序移植*****************************************/
    IplImage *pFrame = NULL;
	//二值图像
	IplImage *bwImg = NULL;
	//剪切图像
	IplImage *backg1 = NULL;
	//旋转图像
	IplImage *backg2 = NULL;
	IplImage *backg3 = NULL;
	IplImage *backg4 = NULL;
	IplImage *back_num = NULL;
	IplImage *back_num2 = NULL;
	IplImage *back_num4 = NULL;
	IplImage *back_num5 = NULL;
	IplImage *back_num6 = NULL;
	IplImage *back_num7 = NULL;
	
	CvBox2D box[10];
	CvBox2D target_box;
  
	CvRect  box2[10];
	CvRect  Cut_box,Bar_box;
	CvRect  numRects[20];
	
	int box_n;

    //backg2中心点
	CvPoint center =cvPoint(300,300);
	CvPoint center2 =cvPoint(300,30);
	CvPoint points[100];
	
	vector<vector<int>> spe_bar;
	spe_bar.resize(1,vector<int>(30));
	
	vector<vector<double>> spe_ori;
	spe_ori.resize(9,vector<double>(30));
	int file_flag = Fe_spe_read(spe_ori,"Spe_Data.txt");
	
	if (file_flag == -1)
	{

		m_bExit = TRUE;
		system("pause");
		_endthreadex(0);
		return TRUE;
		//m_bExit = TRUE;
	}
	
	bwImg = cvCreateImage( cvSize(1600,1200),IPL_DEPTH_8U ,1 );
	backg3 = cvCreateImage( cvSize(1600,1200),IPL_DEPTH_8U,1);
	backg4 = cvCreateImage( cvSize(1600,1200),IPL_DEPTH_8U,1);
	
	backg1 = cvCreateImage( cvSize(600,600),IPL_DEPTH_8U,3);
	backg2 = cvCreateImage( cvSize(600,600),IPL_DEPTH_8U,3);
	
	back_num = cvCreateImage( cvSize(600,60),IPL_DEPTH_8U,3);
	back_num4 = cvCreateImage( cvSize(600,60),IPL_DEPTH_8U,3);
	back_num5 = cvCreateImage( cvSize(600,60),IPL_DEPTH_8U,3);
	
	back_num2 = cvCreateImage( cvSize(600,60),IPL_DEPTH_8U,1);
	back_num6 = cvCreateImage( cvSize(600,60),IPL_DEPTH_8U,1);
	back_num7 = cvCreateImage( cvSize(600,60),IPL_DEPTH_8U,1);
	
	
	//Target_pox dest_location = {7.5,1.6,1,1.02};
	Target_pox dest_location = {2.25,1.6,1.1,1.02};
	
/**************************************************************************************
**************************************************************************************/

	while (!m_bExit)
	{   

		int c = waitKey(10);

		if(CameraGetImageBuffer(hCamera,&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{	
			//将获得的原始数据转换成RGB格式的数据，同时经过ISP模块，对图像进行降噪，边沿提升，颜色校正等处理。
			//我公司大部分型号的相机，原始数据都是Bayer格式的
			status = CameraImageProcess(hCamera, pbyBuffer, m_pFrameBuffer,&sFrameInfo);//连续模式

			//分辨率改变了，则刷新背景
			if (m_sFrInfo.iWidth != sFrameInfo.iWidth || m_sFrInfo.iHeight != sFrameInfo.iHeight)
			{
				m_sFrInfo.iWidth = sFrameInfo.iWidth;
				m_sFrInfo.iHeight = sFrameInfo.iHeight;
				//图像大小改变，通知重绘
			}

			if(status == CAMERA_STATUS_SUCCESS)
			{
				//调用SDK封装好的显示接口来显示图像,您也可以将m_pFrameBuffer中的RGB数据通过其他方式显示，比如directX,OpengGL,等方式。
				CameraImageOverlay(hCamera, m_pFrameBuffer, &sFrameInfo);
				if (iplImage)
				{
					cvReleaseImageHeader(&iplImage);
				}
				iplImage = cvCreateImageHeader(cvSize(sFrameInfo.iWidth,sFrameInfo.iHeight),IPL_DEPTH_8U,sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8?1:3);
				cvSetData(iplImage,m_pFrameBuffer,sFrameInfo.iWidth*(sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8?1:3));
				//cvShowImage(g_CameraName,iplImage);
				
/********************************************************************************************************
******************************************程序移植************************************************/
	pFrame = iplImage;
	Fe_im2bw(pFrame,bwImg,120,3);
	cvDilate(bwImg,backg3,0,1);
	cvErode(backg3,backg4,0,1);			
	#ifndef _PROCESS_KEY_
	box_n = Fe_EdgeDetect(backg4,box,CV_RETR_CCOMP,sizeof(box)/sizeof(box[0]));
	
	//画图
	cout<<box_n<<endl;
		for(int i=0;i<box_n;i++)
		{
			Fe_draw_box(pFrame,box[i],CV_RGB(0,255,0));
		}
		

	//if (box_n>=1&&(c=='r'||c=='R'))
	if(box_n>=1)
	{
		Fe_target(target_box,box[0],dest_location);
		cout<<"\ntarget:\n"
			<<"height:"<<target_box.size.height
			<<"width:"<<target_box.size.width<<endl;
		
		//Fe_draw_box(pFrame,box[0],CV_RGB(0,255,0));
		//Fe_draw_box(pFrame,box[1],CV_RGB(0,255,0));
		//cout<<"box_n="<<box_n<<endl;
		// <<"\nbox_0:\n height="<<box[0].size.height
		// <<"\n width="<<box[0].size.width
		// <<"\n angle="<<box[0].angle
		// <<"\n x="<<box[0].center.x
		// <<"\n y="<<box[0].center.y
		// <<"\n"<<endl;
		
		// cout<<"box_1:\n height="<<box[1].size.height
		// <<"\n width="<<box[1].size.width
		// <<"\n angle="<<box[1].angle
		// <<"\n x="<<box[1].center.x
		// <<"\n y="<<box[1].center.y
		// <<"\n"<<endl;
		
		
		Fe_box2out_rect(target_box,box2[1]);
		Fe_rect_cut(pFrame,box2[1],backg1,center);

		
		//条件旋转
		if(box[0].size.width>=box[0].size.height)
			rotateImage1(backg1,backg2,box[0].angle);
		else
			rotateImage1(backg1,backg2,box[0].angle-270);
		
		
		//Fe_box2rect(Cut_box,target_box,center);
		Fe_box2two_rect(Cut_box,Bar_box,target_box,center);
		Fe_draw_rect(backg2,Cut_box,CV_RGB(0,255,0));
	
		Fe_rect_cut(backg2, Cut_box, back_num, center2 );
		Fe_im2bw3D(back_num,back_num2,0, 780 ,1,3,3);
		//Fe_rect_cut(backg2, Bar_box, back_num, center2 );
		
		
		
		//Fe_im2bw(back_num,back_num2,120,1);
		int n_pints = Fe_pfind(back_num2,30,points,100);
		cout<<"pints =:"<<n_pints<<endl;
		for (int i=0;i<n_pints;i++)
			cout<<points[i].x<<"\t"<<points[i].y<<endl;
			
		box_n = Fe_EdgeDetect(back_num2,box,CV_RETR_CCOMP,sizeof(box)/sizeof(box[0]));
		
		cvCvtColor(back_num2,back_num4,CV_GRAY2BGR);
		
		//Fe_FloodFill(back_num4,points[2]);
		//for (int i=0;i<1;i++)
		for (int i=0;i<n_pints;i++)
		{
			Fe_FloodFill(back_num4,points[i]);
			//Fe_FloodFill(back_num4,cvPoint(20,20));
		}
		
		cvCvtColor(back_num4,back_num6,CV_BGR2GRAY);
		cvAbsDiff(back_num6,back_num2,back_num7);
		int nums = Fe_edgecut(back_num7,numRects);
		cout<<"\nnums= "<<nums<<"\n"<<endl;
		
		
		if(nums >0)
		{
			//cout<<"\nnums= "<<nums<<"\n"<<endl;
			
			cout<<"box_1:\n height="<<numRects[1].height
			<<"\n width="<<numRects[1].width
			<<"\n x="<<numRects[1].x
			<<"\n y="<<numRects[1].y
			<<"\n"<<endl;
			
			//识别
			
			cout<<"\n\nthe numbers are :\n"<<endl;
			 
			for(int i=0 ;i<nums;i++)
			{
				Fe_draw_rect(back_num4,numRects[i],CV_RGB(0,255,0));
				//cout<<"the rate is :"<<(double)numRects[i].width/numRects[i].height<<"\n"<<endl;
				if ((double)numRects[i].width/numRects[i].height > 0.5)
				Fe_spe_get(back_num7,numRects[i],spe_ori);
				else 
				cout<<"1\t";
			 
			}
			cout<<"\n"<<endl;
			
		}
		
		//Fe_im2bw3D(back_num,back_num2,0, 720 ,1,3,3);
		//Fe_barcode_Recognition(back_num2,spe_bar);
		//Fe_barcode_Recognition(back_num2);
		//Fe_im2bw(back_num,back_num2,90, 2);
		//Fe_draw_box(pFrame,target_box,CV_RGB(0,255,0));
	}
	//imshow(g_CameraName,back_num3);
	    cvShowImage(g_CameraName,back_num7);    ///bwImg pFrame backg2 back_num back_num7
	#else
		cvShowImage(g_CameraName,bwImg);     ///backg4  bwImg
	#endif			
/************************************************************************************************
***********************************************************************************************/				
		
				
				
				
				m_iDispFrameNum++;
			}    

			//在成功调用CameraGetImageBuffer后，必须调用CameraReleaseImageBuffer来释放获得的buffer。
			//否则再次调用CameraGetImageBuffer时，程序将被挂起，知道其他线程中调用CameraReleaseImageBuffer来释放了buffer
			CameraReleaseImageBuffer(hCamera,pbyBuffer);

			memcpy(&m_sFrInfo,&sFrameInfo,sizeof(tSdkFrameHead));
		}

		

		if (c == 'q' || c == 'Q' || (c & 255) == 27)
		{
			m_bExit = TRUE;
			break;
		}
		if(c=='c'||c =='C')
		{
			cout<<"stop"<<endl;
			//sleep();
			cvWaitKey();
		}
	}

	if (iplImage)
	{
		cvReleaseImageHeader(&iplImage);
	}
	
/*********************************************************************************************
**************************************Release************************************************/
 if(bwImg)
 {
 cout<<"release bwImg"<<endl;
 cvReleaseImage ( &bwImg);
 }
 
 if(backg1)
 {
 cout<<"release backg1"<<endl;
 cvReleaseImage ( &backg1);
 }
 
 if(backg2)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &backg2);
 }
 if(backg3)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &backg3);
 }
 if(backg4)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &backg4);
 }
 if(back_num)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &back_num);
 }
 if(back_num2)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &back_num2);
 }
 if(back_num4)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &back_num4);
 }
 
 if(back_num5)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &back_num5);
 }
 if(back_num6)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &back_num6);
 }
 if(back_num7)
 {
 cout<<"release backg2"<<endl;
 cvReleaseImage ( &back_num7);
 }
 

/**************************************************************************************
**************************************************************************************/







	_endthreadex(0);
	return 0;
}
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	
	fstream file( "data.txt", ios::out );

/*
	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums;
	CameraSdkStatus status;
	tSdkCameraCapbility sCameraInfo;

	//枚举设备，获得设备列表
	iCameraNums = 10;//调用CameraEnumerateDevice前，先设置iCameraNums = 10，表示最多只读取10个设备，如果需要枚举更多的设备，请更改sCameraList数组的大小和iCameraNums的值

	if (CameraEnumerateDevice(sCameraList,&iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
	{
		printf("No camera was found!");
		return FALSE;
	}

	//该示例中，我们只假设连接了一个相机。因此，只初始化第一个相机。(-1,-1)表示加载上次退出前保存的参数，如果是第一次使用该相机，则加载默认参数.
	//In this demo ,we just init the first camera.
	if ((status = CameraInit(&sCameraList[0],-1,-1,&m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		char msg[128];
		sprintf_s(msg,"Failed to init the camera! Error code is %d",status);
		printf(msg);
		return FALSE;
	}


	//Get properties description for this camera.
	CameraGetCapability(m_hCamera,&sCameraInfo);//"获得该相机的特性描述"

	m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iWidthMax*3,16);	

	if (sCameraInfo.sIspCapacity.bMonoSensor)
	{
		CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_MONO8);
	} 
	
	strcpy_s(g_CameraName,sCameraList[0].acFriendlyName);

	CameraCreateSettingPage(m_hCamera,NULL,
		g_CameraName,NULL,NULL,0);//"通知SDK内部建该相机的属性页面";

#ifdef USE_CALLBACK_GRAB_IMAGE //如果要使用回调函数方式，定义USE_CALLBACK_GRAB_IMAGE这个宏
	//Set the callback for image capture
	CameraSetCallbackFunction(m_hCamera,GrabImageCallback,0,NULL);//"设置图像抓取的回调函数";
#else
	m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread, (PVOID)m_hCamera, 0,  &m_threadID);
#endif

	CameraPlay(m_hCamera);
	
	CameraShowSettingPage(m_hCamera,TRUE);//TRUE显示相机配置界面。FALSE则隐藏。
*/
	int m_bExit2 = Fe_Camera_doinit(m_hCamera,&m_pFrameBuffer,g_CameraName,m_hDispThread,
								uiDisplayThread,m_threadID);
	cout<<m_bExit<<endl;
	while(m_bExit != TRUE)
	{
		waitKey(10);
	}
	
	CameraUnInit(m_hCamera);

	CameraAlignFree(m_pFrameBuffer);

	destroyWindow(g_CameraName);

#ifdef USE_CALLBACK_GRAB_IMAGE
	if (g_iplImage)
	{
		cvReleaseImageHeader(&g_iplImage);
	}
#endif
	return 0;
}

