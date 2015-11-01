#pragma once

#include <cv.h>
#include "Line.h"
#include "BaseShape.h"
#include <math.h>
#include "define.h"
class RectShape:BaseShape
{
public:
	RectShape(CvPoint _point1,CvPoint _point2,CvPoint _point3,CvPoint _point4);
	RectShape(CvRect _cvRect);
	RectShape();
	
	~RectShape(void);
	
	void Draw(IplImage* img);

	bool operator==(const RectShape& rect)//==运算符重载函数
	{
		if (RectShape::MatchArea > Line::CalcLineLen(rect.ctPoint,this->ctPoint))
		{
			return true;
		}
		else
		{
			return false;
		}
	};


	bool operator<(const RectShape& rect)//<运算符重载函数
	{
		//double sin = abs(this->ctPoint.y - rect.ctPoint.y) / Line::CalcLineLen(this->ctPoint,rect.ctPoint);
		//if( sin < 0.05 )
		//{
		//	if(this->ctPoint.x > rect.ctPoint.x && sin < 0.05 )
		//	{
		//		return false;
		//	}
		//	if(sin < 0.05  && this->ctPoint.x < rect.ctPoint.x)
		//	{
		//		return true;
		//	}
		//}
		//else
		//{
		//	if(sin >= 0.05  && this->ctPoint.y < rect.ctPoint.y)
		//	{
		//		return true;
		//	}
		//	if(sin >= 0.05  && this->ctPoint.y >= rect.ctPoint.y)
		//	{
		//		return false;
		//	}
		//}

		bool issameline = false;

		if(abs(this->ctPoint.y - rect.ctPoint.y) < LINE_DISTANCE_X_OFFSET)
		{
			issameline = true;
			if(this->ctPoint.x > rect.ctPoint.x && issameline == true )
		    {
		    	return false;
		    }
		    if(issameline == true  && this->ctPoint.x < rect.ctPoint.x)
		    {
		    	return true;
		    }

		}
		else
		{
			if(issameline == false  && this->ctPoint.y < rect.ctPoint.y)
		    {
		    	return true;
		    }
		    if(issameline == false  && this->ctPoint.y >= rect.ctPoint.y)
		    {
		    	return false;
		    }
		}

		return false;
	};


	inline bool isNear(const RectShape& rect)
	{
		if (RectShape::MatchArea > Line::CalcLineLen(rect.ltPoint,this->ltPoint))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	//plus number means move to right or bottom
	//minus number means move to left or top
	inline RectShape MoveOffset(double x,double y,RectEdge edge)
	{
		switch(edge)
		{
		case LEFT:
			this->lbPoint.x += x;
			this->ltPoint.x += x;
			break;
		case RIGHT:
			this->rtPoint.x += x;
			this->rbPoint.x += x;
			break;
		case TOP:
			this->ltPoint.y += y;
			this->rtPoint.y += y;
			break;
		case BOTTOM:
			this->lbPoint.y += y;
			this->rbPoint.y += y;
			break;
		default:
			break;
		}

		return *this;
	};
public:
	CvPoint ltPoint;
	CvPoint lbPoint;
	CvPoint rbPoint;
	CvPoint rtPoint;
	CvPoint ctPoint;
	bool IsVaild;
private:
	//匹配范围，在MatchArea像素点距离内则为匹配
	static const int MatchArea = 5; 
	
public:

	static bool RectPosCmp(RectShape r1,RectShape r2);

	static int CmpPoint(CvPoint p1,CvPoint p2);

	static int CmpLessPointX(CvPoint p1,CvPoint p2);

	static int CmpLargerPointX(CvPoint p1,CvPoint p2);

	static int CmpLargerPointY(CvPoint p1,CvPoint p2);
	
	static int CmpLessPointY(CvPoint p1,CvPoint p2);

	void CalcCenterPoint();
};

