// Scan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "cv.h"
#include "highgui.h"
//#include "InvoiceImg.h"
//#include "AccntStatementImg.h"
#include "Tesseract.h"
#include "CommonUtil.h"
#include "define.h"

#include "ScanFactory.h"

using namespace std;



int main(int argc, char** argv)
{

	#ifdef _DEBUG_

		char* imgfile = "invoice/28.png";


		//imgType|bank|template
		char* imgType = "010101";

		char* columnConf = "0101011110";
	#else 

		if(argc != 4)
		{
			std::cout << argc << std::endl;
			std::cout << "input arguments is missing..." << std::endl;
			return -1;
		}

		string imgType = argv[1];

		char* columnConf = argv[2];

		char* imgfile = argv[3];

	#endif

	BaseImage* pImg = ScanFactory::CreateImage(string(imgType),string(columnConf) , imgfile);
	//pImg->Recognize();
	std::cout <<CommonUtil::UTF8ToGBK(pImg->Recognize() ) << std::endl;
	 
	delete pImg;
	//std::cout << imgType << " " << imgfile << std::endl;
	//if(imgType == "101001")
	//{

	//}

	//if(imgType == "A")
	//{
	//	AccntStatementImg accntStatementImg(imgfile);

	//	std::cout <<accntStatementImg.Recognize()  << std::endl;

	//	return 0;
	//}

	//if(imgType == "020101")
	//{
	//	InvoiceImg invoiceImg(imgfile);

	//	std::cout <<CommonUtil::UTF8ToGBK(invoiceImg.Recognize() ) << std::endl;

	//	return 0;
	//}

	//InvoiceImg invoiceImg(imgfile);

	//std::cout << std::endl << invoiceImg.Recognize() << std::endl;
	
	//AccntStatementImg accntStatementImg(imgfile);

	//std::cout << std::endl <<accntStatementImg.Recognize()  << std::endl;

	//int c = cvWaitKey(0);

	return 0;

}
