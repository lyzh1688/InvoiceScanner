#include "StdAfx.h"
#include "StringShapeDetector.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>


int StringShapeDetector::FindShape(const IplImage* src,std::vector<StringShape>& shapes)
{
	//cv::Mat srcImage = cv::imread("invoice1.png");
	if(!src)
	{
		return -1;
	}
	cv::Mat srcImage(src,0);
 
	cv::Mat	grayImage = srcImage;
	 
	//resize(srcImage,resizeImg,Size(srcImage.cols * 2 ,srcImage.rows * 2),0,0,CV_INTER_LINEAR);
	cv::Mat dstImage = srcImage;
	int cPointR,cPointG,cPointB,cPoint;//currentPoint;
  


	//输入的SRC已经是单通道图片，不需要cvtColor
	//cv::cvtColor(resizeImg,grayImage, CV_BGR2GRAY);

	//中值滤波
	cv::Mat medianImage;
	cv::medianBlur(grayImage,medianImage,3); //最后一个参数需要为奇数
	cv::Mat sobelImage;

	//参数为：源图像，结果图像，图像深度，x方向阶数，y方向阶数，核的大小，尺度因子，增加的值  
	//这个效果比CANNY好
	cv::Sobel(medianImage,sobelImage,CV_8U,1,0,3,0.4,128);  
 
	cv::Mat normalizeImage;
	cv::normalize(sobelImage,normalizeImage,255,0,CV_MINMAX);
	cv::Mat binaryImage;
	cv::threshold(normalizeImage,binaryImage, 100, 255, cv::THRESH_BINARY_INV );  
	cv::Mat closeImage;
 
	cv::Mat openImage(closeImage.rows,closeImage.cols,CV_8UC1);
	
	//according to experiment,4 times of dilate is better
	cv::dilate(binaryImage,openImage,cv::Mat(3,3,CV_8U),cv::Point(-1,-1),this->DilateSize);
	//imshow("binaryImage",openImage);

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(openImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
	
	for( int i = 0; i < contours.size(); i++ )
	{  
		//使用边界框的方式  
		CvRect aRect =  boundingRect(contours[i]);
		int tmparea = aRect.height * aRect.height;  
		RectShape myRect(aRect);
		if (((double)aRect.width/(double)aRect.height>2)&& ((double)aRect.width/(double)aRect.height<20)&& tmparea>=6&&tmparea<=25000)  
		{  
			StringShape StringShape(myRect);
			shapes.push_back(StringShape);
			//rectangle(resizeImg,cvPoint(aRect.x,aRect.y),cvPoint(aRect.x+aRect.width ,aRect.y+aRect.height),color,2);  
		}  
	} 

	return shapes.size();
}
/*
int StringShapeDetector::FindShape(const IplImage* img,std::vector<StringShape>& shapes,FUNC func,void* p)
{
	std::vector<StringShape> tmpshapes;
	int cnt = this->FindShape(img,tmpshapes);

	if(cnt > 0)
	{
		func(tmpshapes,p,shapes); 
	}
	else
	{
		return 0;
	}

	return shapes.size();
}
*/


void StringShapeDetector::DrawShape(IplImage* img,const std::vector<StringShape>& shapes)
{
	cv::Scalar color = cv::Scalar(0,255,255 );
	cv::Mat srcImage(img,0);
	for(int i = 0 ; i < shapes.size(); ++i)
	{
		std::cout << std::endl;
		std::cout << "ltPoint x : " <<  shapes[i].wordRect.ltPoint.x << "y : " << shapes[i].wordRect.ltPoint.y << std::endl;
		std::cout << "lbPoint x : " <<  shapes[i].wordRect.lbPoint.x << "y : " << shapes[i].wordRect.lbPoint.y << std::endl;
		std::cout << std::endl;
		cv::rectangle(srcImage,shapes[i].wordRect.ltPoint,shapes[i].wordRect.rbPoint,color,2);  
	}
}
