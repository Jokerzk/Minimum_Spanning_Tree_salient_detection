#include "common.h"
#include "assert.h"
#include "MSTMap.h"
#include "MBDmap.h"
#include "BoundaryDissimilarityMap.h"
#include "FT.h"
#define PROFILING

using namespace std;
using namespace cv;
bool flag = true;

int getThreshold(cv::Mat img,int width,int height)
{
	int size = width*height;
	cv:Mat img_gray; 
	cvtColor(img, img_gray, CV_BGR2GRAY);

	cv::MatND outputhist;
	int hisSize[1] = {256};
	float range[2] = { 0.0, 255.0 };
	const float *ranges;ranges = &range[0];
	calcHist(&img_gray, 1, 0, Mat(), outputhist, 1, hisSize, &ranges);
	double sum = 0;
	for (int i = 0; i < 256; i++)
	{
		sum = sum + i * outputhist.at<float>(i);
	}
	int threshold = 0;
	float sumvaluew = 0.0, sumvalue = 0.0, maximum = 0.0, wF, p12, diff, between;
	for (int i = 0; i < 256; i++)
	{
		sumvalue = sumvalue + outputhist.at<float>(i);
		sumvaluew = sumvaluew + i * outputhist.at<float>(i);
		wF = size - sumvalue;
		p12 = wF * sumvalue;
		if (p12 == 0){ p12 = 1; }
		diff = sumvaluew * wF - (sum - sumvaluew) * sumvalue;
		between = (float)diff * diff / p12;
		if (between >= maximum){
			threshold = i;
			maximum = between;}	
	}
	return threshold;
}

void CombinationandPostProcessing(cv::Mat img,int width, int height, float* Src1, float* Src2, float* SalientMap,cv::Mat Dest)//进行高斯平滑和自适应反向增强
{
	int threshold = 0;
	//cv::Mat imgoutput = Mat(height, width, CV_32FC1, cv::Scalar(0));
	//cv::normalize(Src1, Src1, 255, 2, CV_MINMAX);
	//cv::normalize(Src2, Src2, 255, 2, CV_MINMAX);//height*width灰度图，相乘
	Normalize(Src1, Src1, width, height, 255);
	Normalize(Src2, Src2, width, height, 255);
	threshold = getThreshold(img,width,height);
	if (!flag)
	{
		for (int i = 0; i < width*height; i++)
		{
			SalientMap[i] = Src1[i];
			if (SalientMap[i] >= threshold)
				Dest.data[i] = 255;
			else
				Dest.data[i] = 0;
		}
	}
	else 
	{
		for (int i = 0; i < width*height; i++)
		{
			SalientMap[i] = 0.5*(Src2[i] + Src1[i]);

			if (SalientMap[i] >= threshold)
				Dest.data[i] = 255;
			else
				Dest.data[i] = 0;
		}
	}
	
	//add Src1 and Src2;
	//Gaussian Blur;
	//Adaptive Contrast Enhance
}

void getSalientMap(cv::Mat img,int width,int height)
{
	float * salientOnFT = (float *)malloc(height * width * sizeof(float));
	SalientBasedOnFT(img, salientOnFT, img.cols, img.rows);
	
	float * salientOnBDMap = (float *)malloc(height * width * sizeof(float));
	getBoundaryDissimilarityMap(img, 6, salientOnBDMap);//利用边界点（背景）聚类的方式得到边界差异性图并利用最下生成树进行滤波；

	float * salientMap = (float *)malloc(height * width  * sizeof(float));
	cv::Mat imgoutput = Mat(height, width, CV_32FC1, float(0));
	CombinationandPostProcessing(img, width, height, salientOnFT, salientOnBDMap, salientMap, imgoutput);//将距离图与差异性图利用加和的方式进行组合，并作高斯模糊和反向增强处理；

}


int main()
{
	string str;
	printf("plz select a picture!\n");
	getline(cin, str);
	cv::Mat imginput = cv::imread(str);
	cv::imshow("Orginal", imginput);
	int width = imginput.cols;
	int height = imginput.rows;
	const int channels = 3;
	int stride = width*channels;
	getSalientMap(imginput,width,height);
	waitKey(0);

	return 0;
}