//Myimg_pro_tools.h
//useful digital image processing functions I made and collected for creating classes in C++ and opencv
#ifndef _FE_IMG_PRO_TOOLS_H
#define _FE_IMG_PRO_TOOLS_H


#include "stdafx.h"
#include "Fe_img_pro_tools.h"
#include <iostream>
#include <algorithm> 
#include "math.h"
#include <opencv2/opencv.hpp>

using namespace std;

extern const double spe_ave[9][30];

typedef struct Target_pox
{
	double kh ;
	double kw ;
	double ka ;
	double kb ;
}Target_pox;


//输出Box定点坐标
void Fe_cvBoxPoints(CvBox2D box,CvPoint2D32f pt[4]);

//图像数据多个通道转换为int类型的一维数组
void Fe_img2array3C(int *out_array,IplImage *OriImg,int c);

//图像局部数据转换为二维数组
void Fe_img2bwarray2D(IplImage* inImg,CvRect aRect,vector<vector<int>> &myvect,int limit);

//图像二值化
void Fe_im2bw(IplImage* OriImg,IplImage* OutImg,unsigned int th_value,unsigned int c);

//图像二值化形式2
void Fe_im2bw3D(IplImage* OriImg,IplImage* OutImg,unsigned int lo_value,unsigned int up_value ,
			unsigned int kR,unsigned int kG,unsigned int kB);

//边缘识别
int Fe_EdgeDetect(IplImage* InputImg,CvBox2D *ptr_Box,int mode,int maxn_box);

//图片旋转
void rotateImage1(IplImage *img,IplImage* img_rotate,int degree);

//斜矩形绘图
void Fe_draw_box(IplImage *img,CvBox2D track_box,CvScalar color);

//正矩形绘图
void Fe_draw_rect(IplImage *img,CvRect track_box,CvScalar color);

//区域截取
void Fe_rect_cut(IplImage* orgImage, CvRect rectInImage,IplImage* outImage,CvPoint center );

//视频滤波
void Fe_v_filter(IplImage* OutImg,CvCapture *in_Capture,int n);

//box的旋转等价rect
void Fe_box2rect(CvRect &aRect,CvBox2D &aBox,CvPoint center);

//Box中切两个Rect
void Fe_box2two_rect(CvRect &aRect,CvRect &bRect,CvBox2D &aBox,CvPoint center);


//box外接rect
void Fe_box2out_rect(CvBox2D &aBox,CvRect &aRect);

//数字识别
int Fe_num_rec(double *fvector,int length,int n_row,vector<vector<double>> &spe_vector);

//特征向量生成
void Fe_spe_get(IplImage* inImg,CvRect aRect,vector<vector<double>> &spe_vector);

//目标截取
//void Fe_target(CvBox2D &abox,CvBox2D Bbox);
void Fe_target(CvBox2D &abox,CvBox2D Bbox,Target_pox target);

//查找所在某行下降沿
int Fe_pfind(IplImage* src,int the_row,CvPoint *points,int n_max);

//区域填充
void Fe_FloodFill(IplImage *src,CvPoint points);

//数字切割
int Fe_edgecut(IplImage *src,CvRect *Rects);

//样本特征读取
int Fe_spe_read(vector<vector<double>> &spe_ave,const char* filename);

//条形码识别
//void Fe_barcode_Recognition(IplImage* inImg,vector<vector<double>> &spe_vector);
void Fe_barcode_Recognition(IplImage* inImg);

#endif