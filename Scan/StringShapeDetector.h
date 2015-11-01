#pragma once
#include "BaseDetector.h"
#include "StringShape.h"

#define DILATESIZE 8

typedef void (FUNC)(const std::vector<StringShape>& shapes,void* p,std::vector<StringShape>& res); 

class StringShapeDetector:public BaseDetector<StringShape>
{
public:
	StringShapeDetector(void)
	{
		this->DilateSize = DILATESIZE;
	};
	~StringShapeDetector(void){};

	virtual int FindShape(const IplImage* img,std::vector<StringShape>& shapes);

	virtual void DrawShape(IplImage* img,const std::vector<StringShape>& shapes);

	//int FindShape(const IplImage* img,std::vector<StringShape>& shapes,FUNC func,void* p);

private:
	int DilateSize;
};

