#pragma once
#include <cv.h>
#include <vector>

template<class T>
class BaseDetector
{
public:
	BaseDetector(void);
	~BaseDetector(void);
public:

	virtual int FindShape(const IplImage* img,std::vector<T>& shapes)
	{
		return 0;
	}; 
	virtual void DrawShape(IplImage* src,const std::vector<T>& shapes)
	{

	};
};

template<class T>
BaseDetector<T>::BaseDetector(void)
{
}

template<typename T>
BaseDetector<T>::~BaseDetector(void)
{
}

