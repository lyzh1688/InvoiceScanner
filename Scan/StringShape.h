#pragma once
#include "RectShape.h"
#include "BaseShape.h"
class StringShape : public BaseShape
{
public:
	StringShape(){};
	StringShape(RectShape _rect);
	~StringShape(void);
	void Draw(IplImage* img);
public:
	RectShape wordRect;
};

