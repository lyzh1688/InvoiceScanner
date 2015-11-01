#pragma once
#include "BaseDetector.h"
#include "Line.h"

typedef enum LINE_FILTER_ALGO
{
	WITHOUT_NO_JOINT
} LineFilterAlgo;

class LineDetector: public BaseDetector<Line>
{
public:
	LineDetector(void){};
	~LineDetector(void){};

	virtual int FindShape(const IplImage* img,std::vector<Line>& shapes);
	virtual void DrawShape(IplImage* src,const std::vector<Line>& shapes);

	int GetLongLine(double minLen,const std::vector<Line>& lines,std::vector<Line>& longLines);
	std::vector<Line> GetLinesByType(const std::vector<Line>& lines,LineType type);
	int FindShape(const IplImage* src,std::vector<Line>& shapes,LineFilterAlgo);
	bool IsExist(Line& line,const std::vector<Line>& list);
	std::vector<Line> FilterShortLine(const std::vector<Line>& lines,int minLen);
	int Merge(std::vector<Line>& srcList,std::vector<Line>& dstList,LineType lineType);
private:
	std::vector<Line> HorizontalLines;
	std::vector<Line> VerticalLines;

};
