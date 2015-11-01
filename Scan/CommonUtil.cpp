#include "StdAfx.h"
#include "CommonUtil.h"


CommonUtil::CommonUtil(void)
{
}


CommonUtil::~CommonUtil(void)
{
}

std::string CommonUtil::UTF8ToGBK(const std::string& strUTF8)  
{  
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);  

    unsigned short * wszGBK = new unsigned short[len + 1];  

    memset(wszGBK, 0, len * 2 + 2);  

    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);  
  
    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);  

    char *szGBK = new char[len + 1];  

    memset(szGBK, 0, len + 1);  

    WideCharToMultiByte(CP_ACP,0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);  

    std::string strTemp(szGBK);  

    delete[]szGBK;  

    delete[]wszGBK;  

    return strTemp;  
}  

CvRect CommonUtil::CvtRectShape2CvRect(RectShape& rect)
{
	CvRect cvrect;
	cvrect.x = rect.ltPoint.x;
	cvrect.y = rect.ltPoint.y;
	cvrect.width = rect.rbPoint.x - rect.ltPoint.x;
	cvrect.height = rect.rbPoint.y - rect.ltPoint.y;

	return cvrect;
}

void CommonUtil::Split(const string& src, const string& separator, std::vector<string>& dest)
{
    string str = src;
    string substring;
    string::size_type start = 0, index;

    do
    {
        index = str.find_first_of(separator,start);
        if (index != string::npos)
        {    
            substring = str.substr(start,index-start);
            dest.push_back(substring);
            start = str.find_first_not_of(separator,index);
            if (start == string::npos) return;
        }
    }while(index != string::npos);
    
    //the last token
    substring = str.substr(start);

    dest.push_back(substring);
}