#include "StdAfx.h"
#include "SpecialShapeDetector.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace cv;

 

SpecialShape SpecialShapeDetector::FindShape(const IplImage* img,ShapeType shapeType)
{
	SpecialShape specialShape(shapeType);
	CvPoint NoCenterPoints = specialShape.GetCenterPoint(specialShape.SiftDetetor(img),1);
	return specialShape;
}

void SpecialShapeDetector::DrawShape(IplImage* img,const SpecialShape& shapes)
{
	cvCircle( img, shapes.ctPoint ,10 , CV_RGB(0,255,0),1, 8, 3 );
	std::cout << "SpecialShape x: " << shapes.ctPoint.x << " y : " << shapes.ctPoint.y << std::endl;
}
