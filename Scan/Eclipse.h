#pragma once
#include "cv.h"

class Eclipse
{
public:
	Eclipse(void);
	~Eclipse(void);

	int FindEclipse(IplImage* src);
private:
	int threshold;
};

