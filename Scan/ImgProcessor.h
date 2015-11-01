#pragma once
#include <cv.h>
#include "SpecialShape.h"
#include "StringShape.h"

#define OFFSET_NO_POS 10

class ImgProcessor
{
public:
	ImgProcessor(void);
	~ImgProcessor(void);
	static void ImgProcessor::GetImageRect(IplImage* orgImage, CvRect rectInImage, IplImage* imgRect);
	static void ResizeImg(const IplImage* src,IplImage* &dst);
	static IplImage* ResizeImg(IplImage* &img, double scale);
	static void GetImgContours(int threshold,const IplImage* src,IplImage* &dst);
	static void GetInvoiceNo(const std::vector<StringShape>& shapes,void* p,std::vector<StringShape>& resShapes );
};

