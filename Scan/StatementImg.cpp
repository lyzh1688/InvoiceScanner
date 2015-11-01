#include "StdAfx.h"

#include "StatementImg.h"

#include "LineDetector.h"
#include "RectDetector.h"
#include <sstream>
#include "define.h"
StatementImg::~StatementImg(void)
{
}

void StatementImg::ScanImage()
{
	double minLineScale = 0.5;

	LineDetector lineDetector;

	RectDetector rectDetector;

	std::vector<Line> AllLines;

	std::vector<Line> VerticalLines;

	std::vector<Line> HorizontalLines;

	std::vector<RectShape> cellRects;

	
	lineDetector.FindShape(this->contoursImg,AllLines);

	//lineDetector.Merge(lineDetector.GetLinesByType(AllLines,VERTICAL),VerticalLines,VERTICAL);

	lineDetector.Merge(lineDetector.GetLinesByType(AllLines,HORIZONTAL),HorizontalLines,HORIZONTAL);
	
	HorizontalLines = lineDetector.FilterShortLine(HorizontalLines,200);

	this->CalcVerticalLines(HorizontalLines,VerticalLines);

	if(HorizontalLines.size() < 3)
	{
		std::cout << ERROR_HLINE_NOT_FOUND << std::endl;
		return;
	}
	
	
	//std::cout << "HLINE COUNT : " << HorizontalLines.size() << std::endl;
	//this->clearImg = cvCloneImage(this->resizedImg);

	//lineDetector.DrawShape(this->clearImg,lineDetector.FilterShortLine(VerticalLines,100));

	//lineDetector.DrawShape(this->clearImg,lineDetector.FilterShortLine(HorizontalLines,100));

	//lineDetector.DrawShape(this->resizedImg,VerticalLines);
	//lineDetector.DrawShape(this->resizedImg,HorizontalLines);
	//this->resizedImg = ImgProcessor::ResizeImg(this->resizedImg,0.5);

	//cvShowImage("contoursImg",this->resizedImg);
	//cvWaitKey(0);

	//this->contoursImg = ImgProcessor::ResizeImg(this->contoursImg,0.5);

	//cvShowImage("contoursImg",this->contoursImg);

	//int rectcount = rectDetector.FindShape(this->clearImg,cellRects);

	//this->allRowRects = this->SplitIntoRow(cellRects);

	CalcRects(VerticalLines,HorizontalLines,this->allRowRects);

	cvThreshold(this->resizedImg,this->resizedImg,THRESHOLD,255,CV_THRESH_TOZERO);
	//lineDetector.DrawShape(this->resizedImg,HorizontalLines);
	//lineDetector.DrawShape(this->resizedImg,VerticalLines);
	//cvSaveImage( "c:\\28.png", this->resizedImg );
	
	//lineDetector.FindShape(this->contoursImg,AllLines);

	//lineDetector.Merge(lineDetector.GetLinesByType(AllLines,VERTICAL),VerticalLines,VERTICAL);

	//if(VerticalLines.size() != this->columnConf.size() + 1)
	//{
	//	std::cout << ERROR_COLUMN_NOT_FOUND << std::endl;

	//	return;
	//}

	//this->CalcRects(VerticalLines,this->allColumnRects);
}

int StatementImg::CalcRects(std::vector<Line>& _vlines,std::vector<Line>& _hlines,std::vector<std::vector<RectShape>>& allRowRects)
{
	sort(_vlines.begin(),_vlines.end(),Line::CmpLessPointX);

	sort(_hlines.begin(),_hlines.end(),Line::CmpLessPointY);

	int offset = 3 ;

	for(int i = 0 ; i <  _hlines.size() - 1 ; ++i)
	{
		std::vector<RectShape> rowRects;
		
		for(int j = 0 ; j < _vlines.size() - 1 ; ++j)
		{
			CvPoint p1,p2,p3,p4;

			p1.x = _vlines[j].bgnPoint.x + offset;

			p1.y = _hlines[i].bgnPoint.y;

			p2.x = _vlines[j + 1].bgnPoint.x - offset;

			p2.y = _hlines[i].bgnPoint.y;

			p3.x = _vlines[j].bgnPoint.x + offset;

			p3.y = _hlines[i + 1].bgnPoint.y;

			p4.x = _vlines[j + 1].endPoint.x - offset;

			p4.y = _hlines[i + 1].endPoint.y;

			RectShape rect(p1,p2,p3,p4);

			rowRects.push_back(rect);
		}

		allRowRects.push_back(rowRects);
	}

	return allRowRects.size();
}

int StatementImg::CalcVerticalLines(std::vector<Line>& _hLines,std::vector<Line>& VerticalLines)
{
	
	double offestArray[11] = {0,0.0461,0.0821,0.154,0.2134,0.337,0.445,0.552,0.660,0.8687,1};

	if(_hLines.size() >= 2)
	{
		sort(_hLines.begin(),_hLines.end(),Line::CmpLessPointY);
		for(int i = 0; i < 11; ++i)
		{
			int offset = abs(_hLines[0].bgnPoint.x - _hLines[0].endPoint.x)  * offestArray[i];
			CvPoint p1,p2;
			p1.x = _hLines[0].bgnPoint.x + offset;
			p1.y = _hLines[0].bgnPoint.y;

			p2.x = _hLines[_hLines.size() - 1].bgnPoint.x + offset;
			p2.y = _hLines[_hLines.size() - 1].bgnPoint.y;

			Line vline(p1,p2);

			VerticalLines.push_back(vline);
		}
		 
		
		
		

	}
	 

	return VerticalLines.size();
}

std::string StatementImg::RecognizeImage()
{
	std::string colConf = this->columnConf;

	std::vector<std::map<std::string,std::string>> allrows;

	for(int i = 0; i < this->allRowRects.size();++i)
	{
		std::map<std::string,std::string> rowMap;

		if(allRowRects[i].size() == colConf.size())
		{
			for(int j = 0; j < colConf.size();++j)
			{
				std::stringstream ss;
				ss << j;
				if(colConf[j] == '0')
				{
					rowMap[ss.str()] = "";
				}
				else
				{
					std::string resstr = this->RecognizeRect(this->allRowRects[i][j],Tesseract::GetTrainedData(colConf[j]));
					rowMap[ss.str()] = resstr;
					//std::cout << resstr;
				}
				
			}
			
		}

		allrows.push_back(rowMap);
	}

	//std::cout << "tess end" <<std::endl;

	
	return this->ToJString(allrows);

	//return "";

	// 	std::string retStr = "{";

	//retStr += "\"No\":";

	//retStr += "\"" + this->RecognizeRect(this->allColumnRects[8],"eng") + "\",";

	//retStr += "\"Purchase\":";

	//retStr += "\"" + this->RecognizeRect(this->allColumnRects[1],"eng") + "\",";

	// 

	//retStr +="}";

	//return retStr;
}

 