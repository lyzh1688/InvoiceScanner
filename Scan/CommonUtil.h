#pragma once

#include "RectShape.h"
#include <string>
#include <sstream>
#include <locale>
#include <baseapi.h>  
#include <cv.h>


using namespace std;
class CommonUtil
{
public:
	CommonUtil(void);
	~CommonUtil(void);
	static std::string UTF8ToGBK(const std::string& strUTF8);
	static CvRect CvtRectShape2CvRect(RectShape& rect);

	static void Split(const string& src, const string& separator, vector<string>& dest);
};

