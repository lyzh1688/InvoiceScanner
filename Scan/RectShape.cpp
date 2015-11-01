#include "StdAfx.h"
#include "RectShape.h"
#include <algorithm>
RectShape::RectShape()
{
	CvPoint p;
	p.x = 0;
	p.y = 0;
	this->ltPoint = p;
	this->lbPoint = p;
	this->rbPoint = p;
	this->rtPoint = p;
	this->ctPoint = p;
	this->IsVaild = false;
}

RectShape::RectShape(CvPoint _point1, CvPoint _point2, CvPoint _point3, CvPoint _point4)
{
	std::vector<CvPoint> points;
	points.push_back(_point1);
	points.push_back(_point2);
	points.push_back(_point3);
	points.push_back(_point4);
	sort(points.begin(),points.end(),RectShape::CmpPoint);
	this->ltPoint = points[3];
	this->lbPoint = points[2];
	this->rtPoint = points[1];
	this->rbPoint = points[0];

	this->CalcCenterPoint();

	this->IsVaild = true;
}
RectShape::RectShape(CvRect _cvRect)
{
	this->ltPoint.x = _cvRect.x;
	this->ltPoint.y = _cvRect.y;

	this->lbPoint.x = _cvRect.x;
	this->lbPoint.y = _cvRect.y + _cvRect.height;

	this->rtPoint.x = _cvRect.x + _cvRect.width;
	this->rtPoint.y = _cvRect.y;

	this->rbPoint.x = _cvRect.x + _cvRect.width;
	this->rbPoint.y = _cvRect.y + _cvRect.height;

	this->CalcCenterPoint();

	this->IsVaild = true;
}

RectShape::~RectShape(void)
{
}

void RectShape::CalcCenterPoint()
{
	this->ctPoint.x = (this->ltPoint.x + this->lbPoint.x + this->rbPoint.x + this->rtPoint.x) / 4;

	this->ctPoint.y = (this->ltPoint.y + this->lbPoint.y + this->rbPoint.y + this->rtPoint.y) / 4;

}

bool RectShape::RectPosCmp(RectShape r1,RectShape r2)
{
	return r1 < r2;
}

int RectShape::CmpPoint(CvPoint p1,CvPoint p2)
{
	return (p1.x + p1.y) > (p2.x + p2.y) ? 1 : 0;
}

int RectShape::CmpLargerPointX(CvPoint p1,CvPoint p2)
{
	return p1.x > p2.x ? 1 : 0;
}

int RectShape::CmpLessPointX(CvPoint p1,CvPoint p2)
{
	return p1.x < p2.x ? 1 : 0;
}

int RectShape::CmpLargerPointY(CvPoint p1,CvPoint p2)
{
	return p1.y > p2.y ? 1 : 0;
}

int RectShape::CmpLessPointY(CvPoint p1,CvPoint p2)
{
	return p1.y < p2.y ? 1 : 0;
}

void RectShape::Draw( IplImage* img)
{


	CvPoint pt[4], *RectShape = pt;
	int count = 4;

	pt[0] = this->ltPoint;
	pt[1] = this->lbPoint;
	pt[2] = this->rbPoint;
	pt[3] = this->rtPoint;

	// draw the square as a closed polyline
	cvPolyLine( img, &RectShape, &count, 1, 1, CV_RGB(0,255,0), 2, CV_AA, 0 );
}