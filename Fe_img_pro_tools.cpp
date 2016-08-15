//Myimg_pro_tools.cpp
//useful digital image processing functions I made and collected for creating classes in C++ and opencv


#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <fstream>
#include "iomanip"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <algorithm>  
#include "Fe_img_pro_tools.h"

extern const double spe_ave[9][30];


using namespace std;
using namespace cv;
// const double spe_ave[9][30] = {{	0	,	0.6875	,	0.90625	,	0.6875	,	0.408163265	,	0.6875	,	0.5625	,	0	,	0.640625	,	0.734693878	,	0.890625	,	0.265625	,	0	,	0.25	,	0.959183673	,	0.875	,	0	,	0	,	0.25	,	0.979591837	,	0.859375	,	0.28125	,	0	,	0.59375	,	0.897959184	,	0.59375	,	0.828125	,	0.328125	,	0.921875	,	0.612244898	}	,

// {	0.453125	,	0.78125	,	0.87755102	,	0.734375	,	0.428571429	,	0.8125	,	0.34375	,	0	,	0.515625	,	0.836734694	,	0.328125	,	0.34375	,	0	,	0.5	,	0.897959184	,	0	,	0	,	0.612244898	,	0.796875	,	0.081632653	,	0.359375	,	0.921875	,	0.510204082	,	0.0625	,	0	,	0.859375	,	0.578125	,	0.285714286	,	0.375	,	0.285714286	}	,
// {	0.453125	,	0.75	,	0.87755102	,	0.75	,	0.040816327	,	0.8125	,	0.5	,	0	,	0.421875	,	0.836734694	,	0	,	0.109375	,	0.040816327	,	0.59375	,	0.673469388	,	0	,	0.453125	,	0.428571429	,	0.78125	,	0.714285714	,	0.515625	,	0	,	0	,	0.1875	,	1	,	0.734375	,	0.71875	,	0.183673469	,	0.75	,	0.734693878	}	,
// {	0	,	0	,	0	,	0.796875	,	0.546875	,	0	,	0	,	0.6875	,	0.859375	,	0.375	,	0	,	0.671875	,	0.5625	,	0.75	,	0.375	,	0.546875	,	0.59375	,	0	,	0.75	,	0.375	,	0.765625	,	0.75	,	0.75	,	0.953125	,	0.890625	,	0	,	0	,	0	,	0.75	,	0.46875	}	,
// {	0.375	,	0.916667	,	0.75	,	0.75	,	0.5	,	0.625	,	0.583333	,	0	,	0	,	0	,	1	,	0.916667	,	0.7916665	,	0.916667	,	0.416667	,	0.25	,	0	,	0	,	0.166667	,	1	,	0.5	,	0	,	0	,	0	,	1	,	0.5	,	0.75	,	0.75	,	0.666667	,	0.416667}	,
// {	0.166667	,	0.666667	,	0.833333	,	0.916667	,	0.4375	,	0.583333	,	0.583333	,	0	,	0	,	0.4375	,	1	,	0.833333	,	0.75	,	0.875	,	0.4375	,	1	,	0.416667	,	0	,	0	,	0.8125	,	0.75	,	0.333333	,	0	,	0	,	0.8125	,	0.166667	,	0.833333	,	0.75	,	0.7916665	,	0.46875}	,
// {	0.703125	,	0.75	,	0.75	,	0.75	,	0.75	,	0.171875	,	0	,	0	,	0.59375	,	0.625	,	0	,	0	,	0.53125	,	0.75	,	0	,	0	,	0.4375	,	0.890625	,	0	,	0	,	0	,	0.6875	,	0.703125	,	0	,	0	,	0	,	0.859375	,	0.53125	,	0	,	0	}	,
// {	0.2777775	,	0.9583335	,	0.763889	,	0.8055555	,	0.09722215	,	0.7916665	,	0.375	,	0	,	0.5416665	,	0.666667	,	0.625	,	0.7916665	,	0.5416665	,	0.875	,	0.4583335	,	0.7916665	,	0.541667	,	0.16666665	,	0.5833335	,	0.75	,	1	,	0.0833333	,	0	,	0.0833333	,	1	,	0.541667	,	0.833333	,	0.625	,	0.8333335	,	0.5	}	,
// {	0.4375	,	0.6875	,	0.84375	,	0.6875	,	0.408163265	,	0.84375	,	0.515625	,	0	,	0.59375	,	0.816326531	,	0.890625	,	0.078125	,	0	,	0.25	,	1	,	0.703125	,	0.890625	,	0.5	,	0.8125	,	1	,	0.21875	,	0.453125	,	0.203125	,	0.546875	,	0.897959184	,	0.671875	,	0.828125	,	0.3125	,	0.890625	,	0.428571429	}	
// };


/***********************************************************************
函数名称：Fe_cvBoxPoints
函数功能：计算Box的定点坐标
函数输入：box
	      
函数输出：pt[ 4 ](定点坐标)
返回值：  无
**********************************************************************/
void Fe_cvBoxPoints(CvBox2D box,CvPoint2D32f pt[ 4 ] )  
   {  
        double   angle  =  box.angle * CV_PI/180 ; 
        float   a   =   ( float )cos(angle) * 0.5 ;
        float   b   =   ( float )sin(angle) * 0.5 ;
		pt[ 0 ].x   =   box.center.x   -   a * box.size.height - b * box.size.width;
        pt[ 0 ].y   =   box.center.y   +   b * box.size.height - a * box.size.width;  
        pt[ 1 ].x   =   box.center.x   +   a * box.size.height - b * box.size.width;  
        pt[ 1 ].y   =   box.center.y   -   b * box.size.height - a * box.size.width;  
        pt[ 2 ].x   =   2 * box.center.x   -   pt[ 0 ].x;  
        pt[ 2 ].y   =   2 * box.center.y   -   pt[ 0 ].y;  
        pt[ 3 ].x   =   2 * box.center.x   -   pt[ 1 ].x;  
        pt[ 3 ].y   =   2 * box.center.y   -   pt[ 1 ].y;  
  } 
/***********************************************************************
函数名称：Fe_img2array3C
函数功能：图像数据3个通道转换为int类型的一维数组
函数输入：orgImag(图片地址)	  
		  flag（通道选择1/3)
		  c(通道RGB(0,1,2)3为三通道全选)
函数输出：out_array（输出一维数组地址）
返回值：  无
***********************************************************************/
void Fe_img2array3C(int *out_array,IplImage *OriImg,int c = 0)
{
	int n = OriImg->width;
	int m = OriImg->height;
	for (int i = 0;i < m;i ++)
	{
		//定义一个指针指向图像的像素
		unsigned char *OutPtr = (unsigned char*)OriImg->imageData + OriImg->widthStep * i;

		//循环遍历原始图像的每一列像素除以n
		for (int j = 0;j < n;j++)
		{
			if(c==3)
			{
				out_array[i*n*3+j*3]=(int)OutPtr[3 * j ];
				out_array[i*n*3+j*3+1]=(int)OutPtr[3 * j +1];
				out_array[i*n*3+j*3+2]=(int)OutPtr[3 * j +2];
			}
			else
				out_array[i*n+j]=(int)OutPtr[3 * j + c];
		}
	}
}

/***********************************************************************
函数名称：Fe_img2bwarray2D
函数功能：图像局部数据3个通道转换为int类型的二维数组
函数输入：inImg(图片地址)	  
		  aRect（待转换区域）
		  
		  limit（二值化阈值）
函数输出：vector(图像数组)
返回值：  无
***********************************************************************/
void Fe_img2bwarray2D(IplImage* inImg,CvRect aRect,vector<vector<int>> &myvect,int limit)
{
	int startp_x = aRect.x;
	int startp_y = aRect.y;
	int R_height = aRect.height;
	int R_width = aRect.width;
	int sum;
	for (int i = 0;i <R_height;i ++)
	{
		//定义一个指针指向图像的像素
		unsigned char *OutPtr = (unsigned char*)inImg->imageData + inImg->widthStep * (i+startp_y);

		//循环遍历原始图像的每一列像素除以n
		for (int j = 0;j < R_width;j++)
		{
			sum=(int)OutPtr[j+startp_x];
			//sum+=(int)OutPtr[3 * (j+startp_x)+1];
			//sum+=(int)OutPtr[3 * (j+startp_x) +2];
			
			myvect[i][j] = (sum>limit)? 1:0;
			
		}
	}
}

/***********************************************************************
函数名称：Fe_im2bw
函数功能：用于对彩色图像的某一分量c图做固定阈值th_value的二值化处理
函数输入：OriImg（原始彩色图像指针）
		  th_value(阈值0~254)
		  c(彩色分量R/G/B(0~2))
		  c=3时，模式3（RGB求和）
函数输出：OutImg（灰度图像起始指针）
返回值：  无
***********************************************************************/
void Fe_im2bw(IplImage* OriImg,IplImage* OutImg,unsigned int th_value,unsigned int c)
{
	if(c<3)
	{
		for (int i = 0;i < OriImg->height;i ++)
		{
			//定义一个指针指向原始图像的像素，访问像素的数据
			unsigned char *OriPtr = (unsigned char*) OriImg->imageData + OriImg->widthStep * i;

			//定义一个指针指向灰度图像的像素，保存二值化后的数据
			unsigned char *OutPtr = (unsigned char*) OutImg->imageData + OutImg->widthStep * i;

			//循环遍历原始图像的每一列像素
			for (int j = 0;j < OriImg->width;j ++)
			{
				//如果像素点的G分量值不小于165，则说明为白点，否则为黑点
				//OutPtr[3*j] =OutPtr[3*j+1]=OutPtr[3*j+2]= ( OriPtr[3 * j + c] >= th_value ) ? 255 : 0;
				//OutPtr[j] = ( OriPtr[3 * j + c] < th_value ) ? 0: 255;
				OutPtr[j] = ( OriPtr[3 * j + c] < th_value ) ? 0: 255;
			}
		}
	}
	else
	{
		for (int i = 0;i < OriImg->height;i ++)
		{
			//定义一个指针指向原始图像的像素，访问像素的数据
			unsigned char *OriPtr = (unsigned char*) OriImg->imageData + OriImg->widthStep * i;

			//定义一个指针指向灰度图像的像素，保存二值化后的数据
			unsigned char *OutPtr = (unsigned char*) OutImg->imageData + OutImg->widthStep * i;

			//循环遍历原始图像的每一列像素
			for (int j = 0;j < OriImg->width;j ++)
			{
				//如果像素点的G分量值不小于165，则说明为白点，否则为黑点
				//OutPtr[3*j] =OutPtr[3*j+1]=OutPtr[3*j+2]= ( OriPtr[3 * j + c] >= th_value ) ? 255 : 0;
				//OutPtr[j] = ( (OriPtr[3 * j ]+ OriPtr[3 * j +1]+OriPtr[3 * j +2])>= th_value ) ? 0 : 255;
				OutPtr[j] = ( ((OriPtr[3 * j ]+ OriPtr[3 * j +1]+OriPtr[3 * j +2]*1.2)>= th_value )
							 && ((OriPtr[3 * j ]+ OriPtr[3 * j +1]+OriPtr[3 * j +2]*1.2)<= 430 )
							) ? 0 : 255;
			}
		}
	}
}
/***********************************************************************
函数名称：Fe_im2bw3D
函数功能：用于对彩色图像的三分量c图做固定阈值th_value的二值化处理
函数输入：OriImg（原始彩色图像指针）
		  lo_value(阈值0~254)
		  up_value(阈值0~254)
		  kR,kG,kB(RGB分量的增益)
函数输出：OutImg（灰度图像起始指针）
返回值：  无
***********************************************************************/
void Fe_im2bw3D(IplImage* OriImg,IplImage* OutImg,unsigned int lo_value,unsigned int up_value ,
			unsigned int kR,unsigned int kG,unsigned int kB)
{
	for (int i = 0;i < OriImg->height;i ++)
		{
			//定义一个指针指向原始图像的像素，访问像素的数据
			unsigned char *OriPtr = (unsigned char*) OriImg->imageData + OriImg->widthStep * i;

			//定义一个指针指向灰度图像的像素，保存二值化后的数据
			unsigned char *OutPtr = (unsigned char*) OutImg->imageData + OutImg->widthStep * i;

			//循环遍历原始图像的每一列像素
			for (int j = 0;j < OriImg->width;j ++)
			{
				//如果像素点的G分量值不小于165，则说明为白点，否则为黑点
				//OutPtr[3*j] =OutPtr[3*j+1]=OutPtr[3*j+2]= ( OriPtr[3 * j + c] >= th_value ) ? 255 : 0;
				//OutPtr[j] = ( (OriPtr[3 * j ]+ OriPtr[3 * j +1]+OriPtr[3 * j +2])>= th_value ) ? 0 : 255;
				OutPtr[j] = ( ((OriPtr[3 * j ]*kR+ OriPtr[3 * j +1]*kG+OriPtr[3 * j +2]*kB)>= lo_value )
							 && ((OriPtr[3 * j ]*kR+ OriPtr[3 * j +1]*kG+OriPtr[3 * j +2]*kB)<= up_value )
							) ? 0 : 255;
			}
		}
}



/***********************************************************************
函数名称：Fe_EdgeDetect
函数功能：在原始图像中捕捉积木的矩形边框序列
函数输入：InputImg（二值图像起始地址）	
		  mode（边缘模式）CV_RETR_CCOMP
		  maxn_box（数组的长度）
函数输出：p_Box(边框box数组指针)
返回值：  BoxCount
***********************************************************************/
int Fe_EdgeDetect(IplImage* InputImg,CvBox2D *ptr_Box,int mode,int maxn_box)
{
	//用于保存检测出的有效地积木边框的个数
	int BoxRectCount=0;
	//用于保存每个搜索到的轮廓的序列
	CvSeq *Seq = NULL;	
	//用于保存所有边缘序列的内存块
	CvMemStorage *EdgeStorage = cvCreateMemStorage();
	int EdgeCount;
	//调用cvFindContours函数，在二值化处理后的图像中捕捉边缘序列，序列的个数保存于EdgeCount
	EdgeCount = cvFindContours(InputImg , EdgeStorage , &Seq , sizeof(CvContour) , mode);

	//循环遍历每一个捕捉到的边缘序列
	for ( ; Seq != NULL ; Seq = Seq->h_next )
	{
		//取某一序列的最小外接矩形
		CvBox2D Box = cvMinAreaRect2( Seq , NULL);

		//计算该序列的周长
		double BoxLength = cvArcLength( Seq );

		//计算该序列的面积
		double BoxArea = fabs( cvContourArea(Seq) );

		//取最小外接矩形的角度
		double BoxAngle = Box.angle;

		//计算序列的周长平方与面积比，用以滤去细长的不合理边缘序列
		double Length2AreaRate = BoxLength * BoxLength  / BoxArea;
		
		//计算矩形的周长
		double BoxPerimeter = (Box.size.height+Box.size.width)*2;
		//长宽比
		double rate_HW = Box.size.height/Box.size.width;

		//根据以下条件，确保边缘序列属于正方形的积木边缘
		
		if ( BoxPerimeter >= 1500 && BoxPerimeter <=2600 
			&&((rate_HW >=2.3 && rate_HW<=2.9)||(rate_HW >=0.344 && rate_HW<=0.435))
			//&& Length2AreaRate <= 520 
			//&& Length2AreaRate >= 15 
			) 
		/*
		if ( BoxPerimeter >= 605 && BoxPerimeter <=750 )
		*/
		{
			//保存符合积木边缘条件的边缘序列信息
			//BoxRect = cvBoundingRect( Seq , 0 );

			//使用绿色的矩形边框在原图中标识出积木边缘的位置
 			//cvRectangle( OriImg , cvPoint( BoxRect.x , BoxRect.y ) , cvPoint( BoxRect.x + BoxRect.width , BoxRect.y + BoxRect.height ) , CV_RGB( 0 , 255 , 0 ) , 2 );

			//将每一个有效的积木边框序列起始点坐标保存
			//this->BoxRectTable[ this->BoxRectCount ] = BoxRect;
			ptr_Box[BoxRectCount] = Box;

			//积木个数计数累加
			++ BoxRectCount;
		}
		if (BoxRectCount > maxn_box)
			break;
	}
	cvReleaseMemStorage(&EdgeStorage);
	return BoxRectCount;
}

/***********************************************************************
函数名称：rotateImage1
函数功能：顺时针旋转图像degree度
函数输入：img（图像起始地址）	
		  degree（旋转度数）
函数输出：
返回值：  img(旋转后的图像指针)
***********************************************************************/
void rotateImage1(IplImage *img,IplImage* img_rotate,int degree)
{
	double angle = degree  * CV_PI / 180.; // 弧度  
	double a = sin(angle), b = cos(angle); 
	int img_width = img->width;  
	int img_height = img->height;  
	int width_rotate= int(img_height * fabs(a) + img_width * fabs(b));  
	int height_rotate=int(img_width * fabs(a) + img_height * fabs(b));  
	//旋转数组map
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
	float map[6];
	CvMat map_matrix = cvMat(2, 3, CV_32F, map);  
	// 旋转中心
	CvPoint2D32f center = cvPoint2D32f(img_width / 2, img_height / 2);  
	cv2DRotationMatrix(center, degree, 1.0, &map_matrix);  
	//图片拓展
	//map[2] += (width_rotate - img_width) / 2;  
	//map[5] += (height_rotate - img_height) / 2;  
 
	//对图像做仿射变换
	//CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。
	//如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.
	//CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，
	//CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS
	cvWarpAffine( img,img_rotate, &map_matrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, CV_RGB(255,255,255));  //cvScalarAll(0)
}

/***********************************************************************
函数名称：Fe_draw_box
函数功能：绘制倾斜矩形
函数输入：track_box(矩形框)
	      color(颜色CV_RGB(0,0,0))
函数输出：界面绘出矩形
返回值：  无
***********************************************************************/
void Fe_draw_box(IplImage *img,CvBox2D track_box,CvScalar color)
{
	CvPoint2D32f point4[4];
    cvBoxPoints( track_box, point4);//得到track_box四个角的点的坐标
    cout<<point4[0].x<<"\n"<<point4[0].y<<"\n"<<endl;
    CvPoint pt[4];
    for(int i=0;i<4;i++)
		pt[i]=cvPoint((int)point4[i].x,(int)point4[i].y);//把CvPoint2D32f类型的点转为CvPoint类型
	CvPoint* ppt=pt;
	int count=4;
    cvPolyLine(img, &ppt,&count,1,1,color,4);//画矩形
}

/***********************************************************************
函数名称：Fe_draw_rect
函数功能：绘制rect
函数输入：track_box(矩形框)
	      color(颜色CV_RGB(0,0,0))
函数输出：界面绘出矩形
返回值：  无
***********************************************************************/
void Fe_draw_rect(IplImage *img,CvRect track_box,CvScalar color)
{
	CvPoint point4[4];
    //cvBoxPoints( track_box, point4);//得到track_box四个角的点的坐标
	point4[0].x= (int)track_box.x;
	point4[0].y= (int)track_box.y;
	point4[1].x= (int)(track_box.x+track_box.width);
	point4[1].y= (int)track_box.y;
	point4[2].x= (int)(track_box.x+track_box.width);
	point4[2].y= (int)(track_box.y+track_box.height);
	point4[3].x= (int)track_box.x;
	point4[3].y= (int)(track_box.y+track_box.height);
    //CvPoint pt[4];
    CvPoint* ppt=point4;
	int count=4;
    cvPolyLine(img, &ppt,&count,1,1,color,4);//画矩形
}

/***********************************************************************
函数名称：Fe_rect_cut
函数功能：矩形剪切
函数输入：orgImage(图片地址)
	      rectInImage（剪切矩形）
函数输出：imgRect（输出地址）
返回值：  无
***********************************************************************/
void Fe_rect_cut(IplImage* orgImage, CvRect rectInImage,IplImage* outImage,CvPoint center )
{
	cvSet(outImage,CV_RGB(255,255,255));
	CvRect rect_dest;
	//建立目标输出位置
	rect_dest.x = (int)(center.x-rectInImage.width/2);
	rect_dest.y = (int)(center.y-rectInImage.height/2);
	rect_dest.width = rectInImage.width;
	rect_dest.height = rectInImage.height;
	//剪切矩形
	cvSetImageROI(orgImage,rectInImage);
	cvSetImageROI(outImage,rect_dest);
	cvCopy(orgImage, outImage);  
	cvResetImageROI(orgImage);
	cvResetImageROI(outImage);

}
/***********************************************************************
函数名称：Fe_v_filter
函数功能：视频滤波
函数输入：in_Capture(视频源地址)
函数输出：OutImg（输出地址）
返回值：  无
***********************************************************************/
void Fe_v_filter(IplImage* OutImg,CvCapture *in_Capture,int n)
{
	IplImage *OriImg = NULL;
	int vwidth = OutImg->width;
	int vheight = OutImg->height;
	int *imgd_store = new int[OutImg->height*OutImg->width*3];
	Fe_img2array3C(imgd_store,OutImg,3);

	for (int k=1;k<n;k++)
	{
		OriImg = cvQueryFrame( in_Capture );
		for (int i = 0;i < OriImg->height;i ++)
		{
			//定义一个指针指向原始图像的像素，访问像素的数据
			unsigned char *OriPtr = (unsigned char*) OriImg->imageData + OriImg->widthStep * i;

			//循环遍历原始图像的每一列像素
			for (int j = 0;j < OriImg->width;j ++)
			{
				imgd_store[i*vwidth*3+j*3]=imgd_store[i*vwidth*3+j*3]+(int)OriPtr[3 * j ];
				imgd_store[i*vwidth*3+j*3+1]=imgd_store[i*vwidth*3+j*3+1]+(int)OriPtr[3 * j + 1 ];
				imgd_store[i*vwidth*3+j*3+2]=imgd_store[i*vwidth*3+j*3+2]+(int)OriPtr[3 * j + 2 ];
	
			}
		}	
	}
	
	for (int i = 0;i < OutImg->height;i ++)
	{
		//定义一个指针指向图像的像素
		unsigned char *OutPtr = (unsigned char*)OutImg->imageData + OutImg->widthStep * i;
		
		//循环遍历原始图像的每一列像素除以n
		for (int j = 0;j < OutImg->width;j++)
		{
			//如果像素点的G分量值不小于165，则说明为白点，否则为黑点
			OutPtr[3 * j ]=(unsigned char)(imgd_store[i*vwidth*3+j*3]/n);
			OutPtr[3 * j + 1 ]=(unsigned char)(imgd_store[i*vwidth*3+j*3+1]/n);
			OutPtr[3 * j + 2 ]=(unsigned char)(imgd_store[i*vwidth*3+j*3+2]/n);
		}
	}
	delete[] imgd_store;
}

/***********************************************************************
函数名称：Fe_box2rect
函数功能：box的旋转等价rect
函数输入：box(box类型)
	      
函数输出：rect（rect类型）
返回值：  无
***********************************************************************/
void Fe_box2rect(CvRect &aRect,CvBox2D &aBox,CvPoint center)
{
	double R_height,R_width;
	if( aBox.size.height < aBox.size.width)
	{
		R_height = aBox.size.height;
		R_width = aBox.size.width;
	}
	else
	{
		R_height = aBox.size.width;
		R_width = aBox.size.height;
	}
    aRect.x = (int)(center.x - R_width/2 + 0.5); 
	aRect.y = (int)(center.y - R_height/2 + 0.5);
	aRect.width = (int)(R_width+0.5);
	aRect.height = (int)(R_height+0.5);
}

/***********************************************************************
函数名称：Fe_box2two_rect
函数功能：box的旋转等价rect
函数输入：box(box类型)
	      
函数输出：rect（rect类型）
返回值：  无
***********************************************************************/
void Fe_box2two_rect(CvRect &aRect,CvRect &bRect,CvBox2D &aBox,CvPoint center)
{
	double R_height,R_width;
	if( aBox.size.height < aBox.size.width)
	{
		R_height = aBox.size.height;
		R_width = aBox.size.width;
	}
	else
	{
		R_height = aBox.size.width;
		R_width = aBox.size.height;
	}
	aRect.x = (int)(center.x - R_width/2 + 0.5); 
	bRect.x = (int)(center.x - R_width/2 + 0.5); 
	aRect.y = (int)(center.y + R_height/2 -45 + 0.5 );
	bRect.y = (int)(center.y - 40 +0.5);
	aRect.width = (int)R_width;
	aRect.height = 46;
	bRect.width = (int)R_width;
	bRect.height = 40;
	
}




/***********************************************************************
函数名称：Fe_box2out_rect
函数功能：box转外接rect类型
函数输入：box(box类型)
	      
函数输出：rect（rect类型）
返回值：  无
***********************************************************************/
void Fe_box2out_rect(CvBox2D &aBox,CvRect &aRect)
{
	
	double angle = aBox.angle*CV_PI/180;
    float a = (float)fabs(cos(angle)*0.5f); 
    float b = (float)fabs(sin(angle)*0.5f); 
    aRect.x = (int)(aBox.center.x - b*aBox.size.height - a*aBox.size.width-6); 
	aRect.y = (int)(aBox.center.y - a*aBox.size.height - b*aBox.size.width-6);
	aRect.width = (int)((b*aBox.size.height + a*aBox.size.width)*2+12);
	aRect.height = (int)((a*aBox.size.height + b*aBox.size.width)*2+12);
	
}

/***********************************************************************
函数名称：Fe_num_rec
函数功能：数字识别
函数输入：fvector（识别图像）
	      length(特征向量的长度)
		  n_row(样本类个数)
函数输出：无
返回值：  number(最小值)
**********************************************************************/
int Fe_num_rec(double *fvector,int length,int n_row,vector<vector<double>> &spe_vector)
{
	vector<double> out_D;
	double min_v;
	int post;
	out_D.resize(n_row);
	for (int i=0 ;i<n_row;i++)
	{
		double sum=0;
		for(int j=0;j<length;j++)
		{
			sum+=pow(*(fvector+j)-spe_vector[i][j],2);
		}
		out_D[i]=sum;
	}
	min_v=out_D[0];
	post=0;
	for (int i=1;i<9;i++)
	{
		if(min_v>out_D[i])
		{
			min_v = out_D[i];
			post = i;
		}	
		
	}
	/*
	//显示匹配比例
	cout<<"rate_like\n";
	for (int i=0;i<10;i++)
	{
		cout<<out_D[i]<<setw(8)<<" ";
	}
	cout<<endl;
	*/
	if(post==0)
		return post;
    else
		return post+1;

}

/***********************************************************************
函数名称：Fe_spe_get
函数功能：特征提取
函数输入：inImg（识别图像）
	      box（识别区域）
函数输出：
返回值：  无
**********************************************************************/
void Fe_spe_get(IplImage* inImg,CvRect aRect, vector<vector<double>> &spe_vector)
{	
	//double v_height,v_width;
	int number;
	//box长宽校正
	/*
	if (box.size.width>=box.size.height)
	{
		v_height = box.size.width;
		v_width = box.size.height;
	}
	else
	{
		v_height = box.size.height;
		v_width = box.size.width;
	}
	*/
	//CvRect aRect;
	/*
	//aRect初始化
	aRect.x = (int)(center.x-v_width/2+1);
	aRect.y = (int)(center.y-v_height/2+1);
	aRect.width = (int)(v_width-2);
	aRect.height = (int)(v_height-2);
	*/
	//avect初始化
	vector <vector<int>> avect;
	avect.resize(aRect.height,vector<int>(aRect.width));
	//avect生成
	Fe_img2bwarray2D(inImg,aRect,avect,130);
	//特征向量生成
	double fvector[30]; 
	double sum = 0;
	
	int cal_width = 0;
	int cal_height = 0;
	int rest_width = aRect.width;
	int rest_height = aRect.height;
	
	
	for (int i = 0;i < 6 ; i++)
	{
		rest_height -= cal_height;
		cal_height  = (int)(rest_height/(6-i)+0.5);
		rest_width = aRect.width;
		cal_width = 0;
		
		for (int j=0 ;j <5 ; j++)
		{
			rest_width -= cal_width;
			cal_width = (int)(rest_width/(5-j)+0.5);
			sum = 0;
			/*
			if (i!=5&&j!=4)
			{
			*/
				for (int kr = 0;kr < cal_height;kr++)
				{
					for (int kc = 0;kc < cal_width ; kc++)
					{
						sum += avect[kr+aRect.height-rest_height][kc+aRect.width-rest_width];
					}
				}
				fvector[i*5+j]= sum/(cal_width*cal_height);
			//}
			/*
			else if (i!=5&&j==4)
			{
				for (int kr = 0;kr < cal_height;kr++)
				{
					for (int kc = cal_width*2;kc < aRect.width ; kc++)
					{
						sum += avect[kr+i*cal_height][kc];
					}
				}
				fvector[i*5+j]= sum/((aRect.width-cal_width*2)*cal_height);
			}
			else if (i==5&&j!=4)
			{
				for (int kr = cal_height*5;kr < aRect.height;kr++)
				{
					for (int kc = 0;kc < cal_width ; kc++)
					{
						sum += avect[kr][kc+j*cal_width];
					}
				}
				fvector[i*5+j]= sum/((aRect.height-cal_height*5)*cal_width);
			}
			else 
			{
				for (int kr = cal_height*5;kr < aRect.height;kr++)
				{
					for (int kc = cal_width*2;kc < aRect.width ; kc++)
					{
						sum += avect[kr][kc];
					}
				}
				fvector[i*5+j]= sum/((aRect.height-cal_height*5)*(aRect.width-cal_width*2));
			}
			*/
		}
	}
	
	//cout<<"out_spe \n";
	
	/*
    for (int i=0;i<30;i++)
	{
		cout<<fvector[i]<<setw(6)<<" ";
	}
	cout<<"\nnext\n"<<endl;
	*/
	
	number = Fe_num_rec(fvector,30,9,spe_vector);
	cout<<number<<"\t";
	//数据保存
	ofstream outfile("data.txt",ios::app);
	if(!outfile){
        cout << "Open the file failure...\n";
        exit(0);
    }
	//outfile <<number<<"datas:  "<<endl;
    for(int i=0;i<30;i++)//向文件写数据
	outfile <<fvector[i]<<"\t";
	outfile <<"\n"<<endl;
    outfile.close();
	/*
	cout<<"to compare \n";
	for (int i=0;i<18;i++)
	{
		cout<<spe_ave[number][i]<<setw(6)<<" ";
	}
	cout<<"\nnext\n"<<endl;
	*/
}


	
	


/***********************************************************************
函数名称：Fe_target
函数功能：查找数字编码的相对区域
函数输入：Bbox（原始框）

函数输出：abox(数字所在框)
返回值：  无
**********************************************************************/
void Fe_target(CvBox2D &abox,CvBox2D Bbox,Target_pox a_target)
{
	abox.angle = Bbox.angle;
	double R_height,R_width,a,b,a_angle,ka,kb,kh,kw;
	
	// //大小调节
	// kh = 7.5;
	// //kh = 6.5 ;
 	// kw = 1.6;
	// //位置调节
	// ka=1;
	// kb=1.02;
	kh = a_target.kh;
	//kh = 6.5 ;
 	kw = a_target.kw;
	//位置调节
	ka= a_target.ka;
	kb= a_target.kb;
	
	if( Bbox.size.height < Bbox.size.width)
	{
		R_height = Bbox.size.height;
		R_width = Bbox.size.width;
		a_angle = Bbox.angle*CV_PI/180;
		abox.size.height = R_height/kh;
		abox.size.width = R_width/kw;
	}
	else
	{
		R_height = Bbox.size.width;
		R_width = Bbox.size.height;
		a_angle = (Bbox.angle + 90)*CV_PI/180;
		abox.size.height =  R_width/kw;
		abox.size.width = R_height/kh;
	}
	
	a = (R_width*0.5*(1-1/kw))*ka;//+580;
	b = (R_height*0.5*(1+1/kh))*kb;//-150;

	abox.center.x = Bbox.center.x - a*cos(a_angle)+b*sin(a_angle);
	abox.center.y = Bbox.center.y - b*cos(a_angle)-a*sin(a_angle);
	
}

/***********************************************************************
函数名称：Fe_pfind
函数功能：查找某一行不为白色的点位置，连续点只找第一个；
函数输入：inImg（识别图像）
	      box（识别区域）
函数输出：界面绘出矩形
返回值：  j(点的个数)
**********************************************************************/
int Fe_pfind(IplImage* src,int the_row,CvPoint *points,int n_max)
{
	int n,test_p,j;
	unsigned char *OriPtr;
	n = src->width;
	j=0;
	//for (int k=0;k<2;k++)
	for (int k=0;k<1;k++)
	{
		//OriPtr = (unsigned char*) src->imageData + src->widthStep * (the_row-4+8*k);
		OriPtr = (unsigned char*) src->imageData + src->widthStep * (the_row);
		for (int i =50;i<n-50;i++)
		{
			test_p = OriPtr[i+1]-OriPtr[i];
			if(test_p<0)
			{
				points[j].x = i+1;

				//points[j].y = (the_row-4+8*k);
				points[j].y = (the_row);
				j++;
			}
			if (j>n_max)
				break;
		}
		if (j>n_max)
				break;
	}
	return j;	
}

/***********************************************************************
函数名称：Fe_FloodFill
函数功能：填充区域；
函数输入：src(原图)
	  	  points（种子）
函数输出：
返回值：  
**********************************************************************/
void Fe_FloodFill(IplImage *src,CvPoint points)
{
	CvScalar scalarTemp;
	cvFloodFill(
        src,
        points,
        CV_RGB(255,255,255),
		CV_RGB(0,0,0),
		CV_RGB(0,0,0),
        //CV_RGB(10,10,10),
        //CV_RGB(10,10,10),
		NULL,4
        );
}

/***********************************************************************
函数名称：Fe_edgecut
函数功能：数字切割
函数输入：src(原图)
	  	  points（种子）
函数输出：
返回值：  
**********************************************************************/
int Fe_edgecut(IplImage *src,CvRect *Rects)
{
	vector<int> x_sum,y_sum;
	x_sum.resize(src->width);
	y_sum.resize(src->height);
	int a,b;
	int up_points[20];
	int down_points[20];
	unsigned char *OriPtr ;
	int sum =0;
	//x轴投影


	
	for (int i=0;i<src->width;i++)
	{
		sum = 0;
		OriPtr =  (unsigned char*) src->imageData +i;
		for(int j=0;j<src->height;j++)
		{
			if((OriPtr[src->widthStep * j])>100)
				sum +=1;
		}
		x_sum[i]=(sum >2)? 1:0;
	}
	int j = 0;
	//投影作差分
	for (int i=0;i<src->width-1;i++)
	{
		a=x_sum[i+1]-x_sum[i];
		if (a > 0)
			up_points[j]=i+1;
		else if (a<0)
		{
			down_points[j]=i+1;
			j++;
		}
	}
	int k = 0;
	//确定数字所在Rect的x和width
	for (int i=0;i<j;i++)
	{
		b = down_points[i]-up_points[i];
		if (b>3)
		{
			Rects[k].x = up_points[i];
			Rects[k].width = b;
			k++;
			
		}
		if (k>20)
		{
			cout<<"out of num"<<endl;
			return 0;
		}
			
	
	}
	
	//y轴投影
	for (int i=0;i<k;i++)
	{	
		OriPtr =  (unsigned char*) src->imageData +Rects[i].x;
		for(int j=0;j<src->height;j++)
		{
			sum = 0;
			//OriPtr += src->widthStep * j;
			for (int m = 0;m<Rects[i].width;m++)
			{
				if(OriPtr[src->widthStep * j+m]>100)
					sum += 1;
			}
			y_sum[j]=(sum > 2)? 1:0;
		}
		int n = 0;
		//y轴差分
		for(int j=0;j<src->height-1;j++)
		{
			a = y_sum[j+1]-y_sum[j];
			if (a > 0)
				up_points[n]=j+1;
			else if (a<0)
			{
				down_points[n]=j+1;
				n++;
			}
				
		}
		if (n>2)
		{
			cout<<"out of num2"<<endl;
			return 0;
		}
		//y,height确定
		for (int j=0;j<n;j++)
		{
			b = down_points[j]-up_points[j];
			if (b>3)
			{
				Rects[i].y = up_points[j];
				Rects[i].height = b;
			}
		
		}
	}
	return k;	
}

/***********************************************************************
函数名称：Fe_spe_read
函数功能：样本特征均值读取
函数输入：str文件名
		  
函数输出：spe_ave输出数组
返回值：  
**********************************************************************/

int Fe_spe_read(vector<vector<double>> &spe_ave,const char* filename)
{
	string str;
	
	ifstream in(filename);
	
    if(!in)
    {
        cerr << "some errors happened\n";
        return -1;
    }
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<30;j++)
		{
			
			if(in.eof())
			{
				cout<<"datas are not enough"<<endl;
				return -1;
			}
			in>>spe_ave[i][j] ;
			// cout<<spe_ave[i][j] <<' ';	
		}
		cout<<endl;
	}
	in.close();
	return 1;
}
/***********************************************************************
函数名称：Fe_barcode_Recognition
函数功能：特征提取
函数输入：inImg（识别图像）
	      box（识别区域）
函数输出：
返回值：  无
**********************************************************************/
//void Fe_barcode_Recognition(IplImage* inImg,vector<vector<int>> &spe_vector)
void Fe_barcode_Recognition(IplImage* inImg)
{
	vector<int> barpoint,barwidth;
	barpoint.resize(600);
	barwidth.resize(97);
	int j=0;
	int n=0;
	unsigned char *OriPtr ;//= (unsigned char*) OriImg->imageData + OriImg->widthStep * 20;
	while(j<98&&n<20)
	{
		
		OriPtr = (unsigned char*) inImg->imageData + inImg->widthStep * (20+n);
		n++;
		for (int i = 0;i < inImg->width-1;i++)
		{
			if((OriPtr[i+1]-OriPtr[i])!=0)
			{
				cout<<"\nedge"<<i<<"\t";
				barpoint[j] = i;
				j++;
			}
			if(j>98)
			{
				//cout<<"\nj="<<j<<"\n"<<endl;
				//cout<<"\nn="<<n<<"\n"<<endl;
				j=0;
				break;
			}	
		}
		cout<<"\nj="<<j<<"\n"<<endl;
		
	}
	
	if(j==98)
	{
		cout<<"barwidths are :\n";
		for(int i =0;i<97;i++)
		{
			barwidth[i]=(int)((barpoint[i+1]-barpoint[i])/3.2)+1;
			cout<<barwidth[i]<<"\t";
		}
		cout<<endl;
	}

}
