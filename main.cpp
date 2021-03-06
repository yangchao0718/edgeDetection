// ---------------------------------- - 【程序说明】----------------------------------------------
//      程序名称:OpenCV边缘检测：新方法实现边缘特征提取并能定位噪音
//      开发所用IDE版本：Visual Studio 2013  
//      <span style="white-space:pre">  </span>开发所用OpenCV版本：    2.4.9  
//      2014年11月19日 Create by 杨超  
//    
//----------------------------------------------------------------------------------------------  
#include "edge.h"  
#include"stdlib.h"
#include "chistogram.h"
#include "outXls.h"
#include "structdef.h"

using namespace std;
using namespace cv;

//-----------------------------------【全局变量声明部分】--------------------------------------  
//      描述：全局变量声明  
//-----------------------------------------------------------------------------------------------  
//原图，原图的灰度版，目标图  
Mat g_srcImage, g_srcGrayImage, g_dstImage;
MyStruct mmi, mmik, man;
//梯度图
Mat g_srcGrad, g_outEdge;
Mat g_medImg;//中值滤波
Mat g_mixGS;
Mat g_PTh, g_PType, g_PBuffer, g_PBigSmall,g_PNdifference,g_PPdifference,g_PNumbOfS;
Mat g_type;
Mat g_thresold;
int g_dg = 8, g_di;//Trackbar位置参数
bool rgb = false;
float g_avgACBC;
//Canny边缘检测相关变量  
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold = 10;//TrackBar位置参数  
Mat gray;
Mat g_listKS;//存放此方法边缘图
Mat g_imgFlg,g_imgFlg2,imgFlg3;//图像标志位
Mat g_nSectionFlg;//区域划分处理标记
Mat g_calImg;//转换了成了浮点数据类型的灰度值
Mat g_calImg_I;
Mat g_angle;//法向量
Mat g_intAngle;//把法向量统一到8个方向上
Mat g_gradBmp;//梯度图像
Mat g_nType;//用来保存边缘点是几类型的（星型）
Mat g_typeEdge;//保存星型边缘图
Mat g_biGrad, g_dstBiGrad;
int g_numPixel;
Mat g_circle;
vector<Point2i> disappearedPoint,triBranchPoint;
vector<Mat> g_edge(3);
vector<Mat> g_srcImageChannels, g_srcImageChannels_canny;//保存原图的通道
vector<Mat> srcBGR(3);
Mat g_mergImg;//合并彩色边缘
Mat g_srcImageBGR, g_srcImageBGR_canny;//混合边缘通道后的图像
Mat g_srcImageB, g_srcImageG, g_srcImageR;
Mat g_gtImg;
Mat g_nTypeFlag;
Mat g_dedgeX, g_dedgeY;//隐边缘点位置
Mat g_inedgeX, g_inedgeY;//显示边缘点位置
vector<Mat> nTypeEdge(3);
Mat_ <Vec3b> img(240, 320, Vec3b(0, 255, 0));//Mat_ 为Mat的模板类型
float temp = 0;
vector<float> tempB_t, tempB, tempS;//_t表示临时存放变量
int tempM[8] = { 0 };//存入循环遍历每层中的最大连续相同的个数
int md_t = 0, md;
int module;//模板大小设置
float dsm, grd;//dsm为中值与原图中心点差值，grd梯度值
float tempGrad[8] = { 0 };
vector<Point2i> arrP;
Mat g_minGrad, g_maxGrad;
Mat g_grad0, g_grad1, g_grad2, g_grad3, g_grad4, g_grad5, g_grad6, g_grad7, g_grad8;
vector<startPT> S37,S5;
int m1 = 0, m2 = 0;
//图像8领域下标变化索引
int a8i[] = { 0, -1, -1, -1, 0, 1, 1, 1, 0 };
int a8j[] = { 1, 1, 0, -1, -1, -1, 0, 1, 1 };
//领域索引，备用
int a5[] = { 2, 1, 0, -1, -2 };
Mat bigThanTh;
int N1, N2;
int stretchTime;
int TH1=20;
int calTime;
bool find7;
int TH = 0, TH2 = 2 * TH1, TH_D;
int n1 = 0, n2 = 0;
int cnt = 0;
int gTH = 50, nGrad = 8;//gTH为梯度阈值，nGrad 为5*5模板中》gTH的个数
int bluePt;
int startPt;
int nTH;
int g_temth[8] = { 0 };
Mat maxminD, opmaxminD;
bool(*pfindStart)(Point2i centerPt, Point2i &A, Point2i &B, Point2i &AC, Point2i &BC, int &direction, bool & min_max);//declare a function point

int main(int agrc, char** agrv)
{
	

	initMain();//first inital
	//load srcImage
	g_srcImage = imread(".\\sourcePicture\\test/1.jpg"); //CONTRADICTIONPicture 2018 poisson.jpg saltpepper.bmp edgeCONTRADICTION.bmp gaussian.bmp
	//load success or not
	if (!g_srcImage.data)
	{
		cout << "读取图片srcImage时出错！\n";
		return false;
	}

	//load GT image
	g_gtImg = imread(".\\sourcePicture\\gtimage\\1.png");

	//use filter
	//blur(g_srcImage, g_srcImage, Size(5, 5));

	//定义一个Mat类型并给其设定ROI区域  
	//g_srcImage = g_srcImage(Rect(0, 0, 50, 15));



	//一、默认把彩色图像转换为灰度图像----------------------------------
	cvtColor(g_srcImage, g_srcGrayImage, CV_BGR2GRAY);
	Edge edge(g_srcGrayImage);//灰色传值初始化

	//二、取消注释下面语句可以实现彩色图像检测--------------------------
	//Edge edge(g_srcImage);//彩色传值初始化

	for (int m = 1; m < NPT; m++)
	{
		
		edge.Init();//深入初始化
		//showHistogram(g_minGrad);
		namedWindow("th");
		imwrite("./output/th.png", g_PTh);
		cout << "the " << m << " th:" << TH1 << endl;
		nTH = m;
		//edge.edgeDection();
		edge.edgeDetection3();

		TH1 = TH1 + 2;
		
	}
	/*outXls("./output/02 g_PType.xls", g_PType, "int");
	outXls("./output/bigThanTh.xls", bigThanTh, "int");*/

	cout << "g_numPixel=" << g_numPixel << endl;
	g_srcGrayImage.convertTo(g_srcGrayImage, CV_32F);


	//创建Canny检测的tracebar
	namedWindow("canny");
	createTrackbar("参数值：", "canny", &g_cannyLowThreshold, 160, edge.on_Canny);

	outXls("./output/gray.xls", g_srcGrayImage, "float");

	outXls("./output/ntype.xls", g_PType, "int");
	outXls("./output/numPN.xls", g_PNdifference, "int");
	outXls("./output/numPP.xls", g_PPdifference, "int");
	outXls("./output/numPS.xls", g_PNumbOfS, "int");
	outXls("./output/imFlag.xls", g_imgFlg, "int");
	outXls("./output/imgFlag2.xls", g_imgFlg2, "int");
	g_maxGrad.convertTo(g_gradBmp, CV_8UC1);
	imshow("梯度图", g_gradBmp);
	imwrite("./output/grad.png", g_gradBmp);

	//showHistogram(g_maxGrad, "maxGrad");
	//showHistogram(g_PTh, "TH");
	//轮询获取按键信息，若按下Q，程序退出  
	while ((char(waitKey(1)) != 'q'))
	{
	}
	return 0;
}

