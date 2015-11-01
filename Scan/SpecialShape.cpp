#include "StdAfx.h"
#include "SpecialShape.h"

using namespace cv;

SpecialShape::SpecialShape(ShapeType _shapeType)
	:shapeType(_shapeType)
{
}


SpecialShape::~SpecialShape(void)
{
}

std::vector<CvPoint> SpecialShape::SiftDetetor(const IplImage* src)
{
	Mat trainedImg = ShapeMap::GetTrainedImg(this->shapeType);

	Mat queryImg(src,0);

	SiftFeatureDetector detector;

	vector<KeyPoint> trainedKeyPoint;
	vector<KeyPoint> queryKetPoint;

	detector.detect(trainedImg,trainedKeyPoint);
	detector.detect(queryImg,queryKetPoint);
 
	SiftDescriptorExtractor extractor;
	Mat trainedDescriptor;
	Mat queryDescriptor;

	//BruteForceMatcher<L2<float>> matcher;
	FlannBasedMatcher matcher;
	vector<DMatch> matches;

	extractor.compute(trainedImg,trainedKeyPoint,trainedDescriptor);
	extractor.compute(queryImg,queryKetPoint,queryDescriptor);

	matcher.match(trainedDescriptor,queryDescriptor,matches);

	vector<CvPoint> resPoints;

	for(int i = 0; i < matches.size(); ++ i)
	{
		resPoints.push_back(queryKetPoint[ matches[i].trainIdx].pt);
	}

	return resPoints;

}

CvPoint SpecialShape::GetCenterPoint(const std::vector<CvPoint>& allFeaturePoints,int leastCnt)
{
	std::vector<CvPoint> res;
	int maxCnt = 0;
	CvPoint ctPoint;
	for(int i = 0 ; i < allFeaturePoints.size();++i)
	{
		int cnt = this->HasNearPoint(allFeaturePoints,i,SpecialShape::MAX_DISTANCE);
		if( cnt >= leastCnt )
		{
			//res.push_back(allFeaturePoints[i]);
			if (cnt > maxCnt)
			{
				maxCnt = cnt;
				ctPoint = allFeaturePoints[i];
			}
		}
	}

	this->ctPoint = ctPoint;

	return this->ctPoint;
}

int SpecialShape::HasNearPoint(const std::vector<CvPoint>& allFeaturePoints,int index,int threshold)
{
	int count = 0;
	for(int i = 0 ; i < allFeaturePoints.size(); ++i)
	{
		if(i == index)
		{
			count++;
			continue;
		}
		if(Line::CalcLineLen(allFeaturePoints[index],allFeaturePoints[i]) <= (double)threshold)
		{
			count++;
		}
	}

	return count;
}