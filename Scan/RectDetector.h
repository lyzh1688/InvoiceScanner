#pragma once
#include "BaseDetector.h"
#include "RectShape.h"

class RectDetector: public BaseDetector<RectShape>
{
public:
	RectDetector(void){};

	~RectDetector(void){};

	virtual int FindShape(const IplImage* img,std::vector<RectShape>& shapes);

	virtual void DrawShape(IplImage* img,const std::vector<RectShape>& shapes);

	double CalcAngle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 );

	//int SplitIntoRows(int columnCnt,const std::vector<RectShape>& cellRects,std::vector<std::map<int,RectShape>>& rows);

};

