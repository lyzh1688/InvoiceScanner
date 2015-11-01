#pragma once
#include <string>
#include <baseapi.h>  
#include <strngs.h>  
#include <cv.h>
#include "define.h"


#ifdef _DEBUG_
	#pragma comment(lib,"libtesseract302d.lib")
	#pragma comment(lib,"liblept168d.lib") 
#else
	#pragma comment(lib,"libtesseract302.lib")
	#pragma comment(lib,"liblept168.lib") 
#endif

typedef enum RECOGTYPE
{
	RAW = 0,
	BOX = 1

}RecogType;

class Tesseract
{
public:
	Tesseract(void);
	~Tesseract(void);
	std::string Recognize(const char* trainedData,IplImage* img,RecogType type = RAW );
	bool Init(const char* trainedData);

	static char* GetTrainedData(char i)
	{
		if(i == '0')
		{
			return NULL;
		}
		else if(i == '1')
		{
			return "num";
		}
		else if(i == '2')
		{
			return "eng";
		}
		else if(i == '3')
		{
			return "chi_sim";
		}
		else
		{
			return NULL;
		}
	}

private:
	tesseract::TessBaseAPI *api;
};

