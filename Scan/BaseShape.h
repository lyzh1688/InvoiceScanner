#pragma once
#include <cv.h>
class BaseShape
{
public:
	BaseShape(void);

	~BaseShape(void);

	virtual void Draw(IplImage* img){};

};

