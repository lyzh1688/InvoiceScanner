#include "StdAfx.h"
#include "BaseImage.h"
#include "define.h"
#include "CommonUtil.h"
#include "ThresholdController.h"

#ifdef _DEBUG_	
#include <fstream>
#include <sstream>
static int abc = 0;
#endif

BaseImage::BaseImage(char* picName)
{
	this->srcImg = cvLoadImage(picName, 0 );

	if( !this->srcImg )
	{
		std::cout<<"不能载入"<< picName <<"继续下一张图片"<< std::endl;
	}
	
	ImgProcessor::ResizeImg(this->srcImg,this->resizedImg);

	this->contoursImg = cvCloneImage(this->resizedImg );

	cvZero(this->contoursImg);

	ImgProcessor::GetImgContours(THRESHOLD,this->resizedImg,this->contoursImg);

	
	this->clearImg = cvCloneImage(this->resizedImg );
	 
	cvZero(this->clearImg);
}


BaseImage::~BaseImage(void)
{
	cvReleaseImage( &this->srcImg );
	cvReleaseImage( &this->resizedImg );
	cvReleaseImage( &this->clearImg );
	cvReleaseImage( &this->contoursImg);
}

std::string BaseImage::Recognize()
{
	this->ScanImage();

	string result = this->RecognizeImage();
#ifdef _DEBUG_	
	char filename[] = "..."; // 此处写入文件名 
	ofstream fout("c:\\result.txt");
	fout << result; // fout用法和cout一致, 不过是写到文件里面去
#endif
	return result;
}

std::string BaseImage::RecognizeRect(RectShape& _rect,char* tranedData,RecogType type)
{
	if(!_rect.IsVaild)
	{
		return "";
	}

	CvRect rect;

	rect = CommonUtil::CvtRectShape2CvRect(_rect);

	CvSize size;

	size.width = rect.width;

    size.height = rect.height;

	IplImage* pImg = cvCreateImage( size, this->resizedImg->depth, this->resizedImg->nChannels );

	ImgProcessor::GetImageRect(this->resizedImg, rect, pImg);

	pImg = ImgProcessor::ResizeImg(pImg,2);
#ifdef _DEBUG_	
	abc++;
 
	stringstream ss;
	string s;
	ss << abc;
	ss >> s;
	string filename = "c:\\test\\" + s + ".png";
	cvSaveImage( filename.c_str(), pImg );
#endif	
	string retData = this->tess.Recognize(tranedData,pImg,type);

	cvReleaseImage(&pImg);

	return retData;
}

std::vector<std::vector<RectShape>> BaseImage::SplitIntoRow(std::vector<RectShape>& rects)
{
	sort(rects.begin(),rects.end(),RectShape::RectPosCmp);

	std::vector<std::vector<RectShape>> allRowRects;
	std::vector<RectShape> oneRowRects;
	//double lastY = 0;
	for(int i = 0 ; i < rects.size(); ++i)
	{
		if (i == 0)
		{
			oneRowRects.push_back( rects[i]);
		}
		if(i > 0)
		{
			if(abs(rects[i].ctPoint.y - rects[i - 1].ctPoint.y) < ThresholdController::GetInstance().RECT_DISTANCE_Y_OFFSET_1 )
			{
				oneRowRects.push_back( rects[i]);
			}
			else
			{
				allRowRects.push_back(oneRowRects);
				oneRowRects.clear();
				oneRowRects.push_back( rects[i]);
			}
		}
	}

	if(rects.size() > 0)
	{
		allRowRects.push_back(oneRowRects);
	}
	

	return allRowRects;
}

RectShape BaseImage::GetRectByIndex(const std::vector<std::vector<RectShape>>& allRects,int i , int j )
{
	RectShape tmpRect;
	if(allRects.size() > i)
	{
		if(allRects[j].size() > j)
		{
			tmpRect = allRects[i][j];
		}
	}

	return tmpRect;
}

std::string BaseImage::ToJString(const std::vector<std::map<std::string,std::string>>& records)
{
	string jstr = "[";

	for(int i = 0 ; i < records.size(); ++i)
	{
		//std::cout << "i : " << i << std::endl;
		jstr += "{";

		std::map<std::string,std::string> record = records[i];
		if(record.size() == 0)
		{
			continue;
		}
		//std::cout << "record has been found  "  << std::endl;

		std::map<std::string,std::string>::iterator it = record.begin();

		//std::cout << "begin has been found  "  << std::endl;

		std::map<std::string,std::string>::iterator iend = record.end();

		//std::cout << "record size : " << record.size()  << std::endl;
		
		iend--;

		//std::cout << "end--  "  << std::endl;

		for(; it != record.end(); ++it)
		{
			//std::cout << "it->first : " << it->first << std::endl;
			string elem = "\"" + it->first + "\":" + "\"" + it->second + "\"";
			//std::cout << "elem : " << elem << std::endl;
			jstr += elem;

			if(it != iend)
			{
				jstr += ",";
			}

		 

		}

        //cout<<"key: "<<it->first <<" value: "<<it->second<<endl;

		jstr += "}";
	}


	jstr += "]";

	return jstr;
}