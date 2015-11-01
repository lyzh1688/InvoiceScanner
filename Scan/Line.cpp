#include "StdAfx.h"
#include "Line.h"


Line::Line(CvPoint _bgnPoint,CvPoint _endPoint)
{
	if(_bgnPoint.x <= _endPoint.x)
	{
		this->bgnPoint = _bgnPoint;
		this->endPoint = _endPoint;
	}
	else
	{
		this->bgnPoint = _endPoint;
		this->endPoint = _bgnPoint;
	}

	this->lineLen = this->CalcLineLen(this->bgnPoint,this->endPoint);
	this->GetLineType();

	if(VERTICAL == this->lineType)
	{
		if(this->bgnPoint.y > this->endPoint.y)
		{
			CvPoint tmp = this->bgnPoint;
			this->bgnPoint = this->endPoint;
			this->endPoint = tmp;
		}
	}
}



Line::~Line(void)
{
}

void Line::Draw( IplImage* img)
{
	cvLine(img,this->bgnPoint,this->endPoint,CV_RGB(255,255,255),1);
}


double Line::CalcLineLen(CvPoint _bgnPoint,CvPoint _endPoint)
{
	double dx1 = _bgnPoint.x - _endPoint.x;

	double dy1 = _bgnPoint.y - _endPoint.y;

	return sqrt(dx1 * dx1 + dy1 * dy1);
}

//sin(10) = 0.17365
//sin(80) = 0.98481
LINE_TYPE Line::GetLineType()
{

	double sin = 0;

	double dx1 = this->bgnPoint.x - this->endPoint.x;
	double dy1 = this->bgnPoint.y - this->endPoint.y;

	 
	sin = fabs(dy1) / this->lineLen;
	
	if (sin > 0.98481 && sin <= 1)
	{
		this->lineType = VERTICAL;
	}
	else if (sin >= 0 && sin < 0.17365)
	{
		this->lineType = HORIZONTAL;
	}
	else
	{
		this->lineType = OTHER;
	}
	return this->lineType;
}

bool Line::HasJoint(Line& _line)
{
	if(this->lineType != _line.lineType)
	{
		if(Line::CalcLineLen(this->bgnPoint,_line.bgnPoint) <= (double)Line::MatchArea)
		{
			return true;
		}

		if(Line::CalcLineLen(this->endPoint,_line.bgnPoint) <= (double)Line::MatchArea)
		{
			return true;
		}
		if(Line::CalcLineLen(this->bgnPoint,_line.endPoint) <= (double)Line::MatchArea)
		{
			return true;
		}
		if(Line::CalcLineLen(this->endPoint,_line.endPoint) <= (double)Line::MatchArea)
		{
			return true;
		}

	}
	
	return false;
	

}

int Line::CmpLargerPointX(Line p1,Line p2)
{
	return  (p1.bgnPoint.x + p1.endPoint.x) / 2 > (p2.bgnPoint.x + p2.endPoint.x) / 2 ? 1 : 0;
}

int Line::CmpLessPointX(Line p1,Line p2)
{
	return  (p1.bgnPoint.x + p1.endPoint.x) / 2 < (p2.bgnPoint.x + p2.endPoint.x) / 2 ? 1 : 0;
}

int Line::CmpLargerPointY(Line p1,Line p2)
{
	return  (p1.bgnPoint.y + p1.endPoint.y) / 2 > (p2.bgnPoint.y + p2.endPoint.y) / 2 ? 1 : 0;
}

int Line::CmpLessPointY(Line p1,Line p2)
{
	return  (p1.bgnPoint.y + p1.endPoint.y) / 2 < (p2.bgnPoint.y + p2.endPoint.y) / 2 ? 1 : 0;
}