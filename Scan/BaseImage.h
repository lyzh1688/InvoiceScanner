#pragma once
#include <cv.h>
#include <vector>
#include <iostream>
#include "ImgProcessor.h"
#include "Tesseract.h"

class BaseImage
{
public:
	BaseImage(char* picName);

	~BaseImage(void);

	virtual std::string Recognize();

	virtual void ScanImage(){};

	virtual std::string RecognizeImage(){ return "";};

	std::vector<std::vector<RectShape>> SplitIntoRow(std::vector<RectShape>& rects);

	RectShape GetRectByIndex(const std::vector<std::vector<RectShape>>& allRects,int i , int j );

	std::string RecognizeRect(RectShape& _rect,char* tranedData,RecogType type = RAW);

	std::string ToJString(const std::vector<std::map<std::string,std::string>>& records);
public:
	IplImage* srcImg;
	IplImage* resizedImg;
	IplImage* contoursImg;
	IplImage* clearImg;
	Tesseract tess;
};

