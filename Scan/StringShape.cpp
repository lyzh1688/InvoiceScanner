#include "StdAfx.h"
#include "StringShape.h"


StringShape::StringShape(RectShape _rect)
	:wordRect(_rect)
{
}


StringShape::~StringShape(void)
{
}

void StringShape::Draw(IplImage* img)
{
	this->wordRect.Draw(img);
}