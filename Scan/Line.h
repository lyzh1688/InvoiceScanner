#pragma once

#include "cv.h"
#include "LineTypeEnum.h"
#include "BaseShape.h"
class Line:BaseShape
{
public:
	Line(CvPoint bgnPoint,CvPoint endPoint);
	Line(){};
	~Line(void);


public:
	CvPoint bgnPoint;
	CvPoint endPoint;
	double  lineLen;
	LineType lineType;

public:
	bool operator==(const Line& line)//��������غ���
	{
		if(line.lineType != this->lineType)
		{
			return false;
		}

		if (Line::MatchArea > Line::CalcLineLen(line.bgnPoint,this->bgnPoint) && 
			Line::MatchArea > Line::CalcLineLen(line.endPoint,this->endPoint))
		{
			return true;
		}
		else
		{
			return false;
		}
	};
	bool HasJoint(Line& _line);

	static double CalcLineLen(CvPoint bgnPoint,CvPoint endPoint);

	void Draw(IplImage* img);


	static int CmpLargerPointX(Line p1,Line p2);

	static int CmpLessPointX(Line p1,Line p2);
 
	static int CmpLargerPointY(Line p1,Line p2);

	static int CmpLessPointY(Line p1,Line p2);


private:
	//ƥ�䷶Χ����MatchArea���ص��������Ϊƥ��
	static const int MatchArea = 10; 
	LineType GetLineType();


};

