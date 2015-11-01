#include "StdAfx.h"
#include "Tesseract.h"
#include "CommonUtil.h"



using namespace std;
//
//string UTF8ToGBK(const std::string& strUTF8)  
//{  
//    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);  
//    unsigned short * wszGBK = new unsigned short[len + 1];  
//    memset(wszGBK, 0, len * 2 + 2);  
//    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, (
//		LPWSTR)wszGBK, len);  
//  
//    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);  
//    char *szGBK = new char[len + 1];  
//    memset(szGBK, 0, len + 1);  
//    WideCharToMultiByte(CP_ACP,0, (
//		LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);  
//    //strUTF8 = szGBK;  
//    std::string strTemp(szGBK);  
//    delete[]szGBK;  
//    delete[]wszGBK;  
//    return strTemp;  
//}  

Tesseract::Tesseract(void)
{
	this->api = new tesseract::TessBaseAPI();
	setlocale(LC_ALL,"Chinese-simplified");
 
}


Tesseract::~Tesseract(void)
{
	delete this->api;
}

bool Tesseract::Init(const char* trainedData)
{ 
	//if sucess return null
	if(this->api->Init(NULL,trainedData,tesseract::OEM_DEFAULT))
	{
		return false;
	}

	this->api->SetVariable("tessedit_char_whitelist", "012,3456789.");

	return true;
}

std::string Tesseract::Recognize(const char* trainedData,IplImage* img,RecogType type)
{
	string  parseStr = "";

	if(!this->Init(trainedData))
	{
		std::cout << "TessBaseAPI init failed" << std::endl;
		return "";
	}

	if( !img ) 
	{
		std::cout << "input image is null" << std::endl;
		return "";
    }

	this->api->SetImage((unsigned char*)img->imageData, img->width,
                   img->height, img->nChannels, img->widthStep);

    // be aware of tesseract coord systems starting at left top corner!
    this->api->SetRectangle(0, 0, img->width, img->height);

	char* outText = "";

	switch(type)
	{
	case RAW:
		outText = this->api->GetUTF8Text();
		//std::cout << "outText ........" << outText << std::endl;
		//std::cout << "confidence ........" << this->api->MeanTextConf() << std::endl;
		break;
	case BOX:
		outText = this->api->GetBoxText(0);
		break;
	default:
		break;
	}

	string str(outText);
	//parseStr = str;
	//parseStr = CommonUtil::UTF8ToGBK(str);

	//std::cout << parseStr << std::endl;

	//int* res = this->api->AllWordConfidences();

	//for(int i = 0 ;i < sizeof(res) - 1 ; ++i)
	//{
	//	std::cout << res[i] << " " ;
	//}

	//std::cout << "-------------------------" << std::endl;

	//std::cout << str << std::endl;

    api->Clear();

    api->End();

    delete [] outText;

	return str;

}

 