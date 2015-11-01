#pragma once

#include <cv.h>

#include "ShapeMap.h"
#include <vector>
#include "Line.h"
#include "BaseShape.h"
class SpecialShape:BaseShape
{
public:
	SpecialShape(){};
	SpecialShape(ShapeType _shapeType);
	~SpecialShape(void);
	std::vector<CvPoint> SiftDetetor(const IplImage * src);
	CvPoint GetCenterPoint(const std::vector<CvPoint>& allFeaturePoints,int leastCnt);
	//CvPoint GetCenterPoint(const std::vector<CvPoint>& filteredFeaturePoints);
public:
	ShapeType shapeType;
	static const int MAX_DISTANCE = 3;
	CvPoint ctPoint;
private:
	int HasNearPoint(const std::vector<CvPoint>& allFeaturePoints,int index,int threshold);
};

