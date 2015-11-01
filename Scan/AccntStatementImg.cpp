#include "StdAfx.h"
#include "AccntStatementImg.h"
#include "InvoiceImg.h"
#include "LineDetector.h"
#include "RectDetector.h"
#include "SpecialShapeDetector.h"
#include "StringShapeDetector.h"
#include <algorithm>
#include "CommonUtil.h"
#include "ImgProcessor.h"
#include  "define.h"

AccntStatementImg::~AccntStatementImg(void)
{
}

int AccntStatementImg::CalcRects(std::vector<Line> _lines,std::vector<RectShape>& allRects)
{
	std::vector<CvPoint> allpt;
	std::vector<CvPoint> toppts;
	std::vector<CvPoint> btmpts;
	for(int i = 0 ; i < _lines.size();++i)
	{
		allpt.push_back(_lines[i].bgnPoint);
		allpt.push_back(_lines[i].endPoint);
	}

	sort(allpt.begin(),allpt.end(),RectShape::CmpLargerPointY);

	toppts = this->GetExtremePoint(allpt,VERTICAL);

	reverse(allpt.begin(),allpt.end());

	btmpts = this->GetExtremePoint(allpt,VERTICAL);

	sort(toppts.begin(),toppts.end(),RectShape::CmpLargerPointX);

	sort(btmpts.begin(),btmpts.end(),RectShape::CmpLargerPointX);

	//for(int i = 0 ; i < btmpts.size();++i)
	//{
	//	cvCircle( this->resizedImg, btmpts[i] ,10 , CV_RGB(0,255,0),1, 8, 3 );
	//}
	//for(int i = 0 ; i < toppts.size();++i)
	//{
	//	cvCircle( this->resizedImg, toppts[i] ,10 , CV_RGB(255,255,0),1, 8, 3 );
	//}
	//this->resizedImg = ImgProcessor::ResizeImg(this->resizedImg,0.5);
	//cvShowImage("bbb",this->resizedImg);

	if(toppts.size() == btmpts.size())
	{
		double offset = abs(btmpts[0].y - toppts[0].y) * 1 / 15;

		for(int i = 0 ; i < toppts.size() - 1; ++i)
		{

			CvPoint p1,p2,p3,p4;

			p1.x = btmpts[i].x;
			
			p1.y = btmpts[0].y + offset;
			
			p2.x = btmpts[i + 1].x;
			
			p2.y = btmpts[1].y + offset;
			
			p3.x = toppts[i].x;
			
			p3.y = toppts[0].y;

			p4.x = toppts[i].x;
			
			p4.y = toppts[0].y;

			RectShape rect(p1,p2,p3,p4);
			//RectShape rect(p1,p2,toppts[i],toppts[i + 1]);
			
			allRects.push_back(rect);

		}
	}
 
	return allRects.size();
}

std::vector<CvPoint> AccntStatementImg::GetExtremePoint(std::vector<CvPoint> allpt,LineType type)
{
	std::vector<CvPoint> tmppt;
	bool flag = false;
	if(type == VERTICAL)
	{
		for(int i = 0 ; i < allpt.size(); ++i)
		{
			if(i == 0)
			{
				tmppt.push_back(allpt[i]);
			}
			else
			{
				for(int j = 0 ; j < tmppt.size(); ++j)
				{
					if(abs(tmppt[j].x - allpt[i].x) < LINE_DISTANCE_X_OFFSET)
					{
						flag = false;
						break;
					}
					if(flag == false && (tmppt.size() - 1) == j)
					{
						tmppt.push_back(allpt[i]);
						break;
					}
				}
			}

			if(AccntStatementImg::verticalLineNum == tmppt.size())
			{
				break;
			}
		}
	}

	return tmppt;
}

void AccntStatementImg::ScanImage()
{
	LineDetector lineDetector;

	RectDetector rectDetector;

	std::vector<Line> AllLines;

	std::vector<Line> VerticalLines;

	lineDetector.FindShape(this->contoursImg,AllLines);

	//cvShowImage("contoursImg",this->contoursImg);
	//int c = cvWaitKey(0);
	VerticalLines =  lineDetector.GetLinesByType(AllLines,VERTICAL);

	int rectcount = this->CalcRects(VerticalLines,this->rects);

	sort(this->rects.begin(),this->rects.end(),RectShape::RectPosCmp);

	//std::cout << "RectShape count : " << rectcount;

}

std::string AccntStatementImg::RecognizeImage()
{

 
	std::string retStr ;

	std::vector<StringRow> dateRows;

	std::vector<StringRow> accntRows;

	std::vector<StringRow> otherAccntRows;

	std::vector<StringRow> amountRows;

	std::vector<StringRow> balRows;

	if(this->rects.size() == 0)
	{
		return "";
	}

	string dateList = this->RecognizeRect(this->rects[0],"num",BOX);

	this->SplitIntoRows(dateList,dateRows);

	//std::cout << "-----------------" << std::endl;

	//for(int i = 0 ; i < dateRows.size(); ++i)
	//{
	//	std::cout << dateRows[i].words << " top : " << dateRows[i].top << " bottom : " << dateRows[i].bottom << std::endl;
	//}

	string accntList = this->RecognizeRect(this->rects[1],"num",BOX);

	this->SplitIntoRows(accntList,accntRows);

	//std::cout << "-----------------" << std::endl;
	//for(int i = 0 ; i < accntRows.size(); ++i)
	//{
	//	std::cout << accntRows[i].words << " top : " << accntRows[i].top << " bottom : " << accntRows[i].bottom <<  std::endl;
	//}

	string otherAccntList = this->RecognizeRect(this->rects[2],"num",BOX);

	this->SplitIntoRows(otherAccntList,otherAccntRows);

	//std::cout << "-----------------" << std::endl;

	//for(int i = 0 ; i < otherAccntRows.size(); ++i)
	//{
	//	std::cout << otherAccntRows[i].words << " top : " << otherAccntRows[i].top << " bottom : " << otherAccntRows[i].bottom <<  std::endl;
	//}

	string amount = this->RecognizeRect(this->rects[3],"num",BOX);

	this->SplitIntoRows(amount,amountRows);

	//std::cout << "-----------------" << std::endl;

	//for(int i = 0 ; i < amountRows.size(); ++i)
	//{
	//	std::cout << amountRows[i].words << " top : " << amountRows[i].top << " bottom : " << amountRows[i].bottom <<  std::endl;
	//}

	
	string balance = this->RecognizeRect(this->rects[7],"num",BOX);

	this->SplitIntoRows(balance,balRows);

	//std::cout << "-----------------" << std::endl;

	//for(int i = 0 ; i < balRows.size(); ++i)
	//{
	//	std::cout << balRows[i].words << " top : " << balRows[i].top << " bottom : " << balRows[i].bottom <<  std::endl;
	//}

	std::vector<std::map<std::string,std::string>> records;

	this->Merge(records,dateRows,accntRows,otherAccntRows,amountRows,balRows);

	return this->ToJString(records);
}
 


void AccntStatementImg::Merge(std::vector<std::map<std::string,std::string>>& records,
							  const std::vector<StringRow>& dateRows,
							  const std::vector<StringRow>& accntRows,
                              const std::vector<StringRow>& otherAccntRows,
                              const std::vector<StringRow>& amountRows,
							  const std::vector<StringRow>& balRows)
{
	for(int i = 0 ; i < dateRows.size(); ++i)
	{
		std::map<std::string,std::string> record;
		int basetop = 0;
		//filter some wrong words which is not date;
		if(dateRows[i].words.length() >= 8)
		{
			//std::map<std::string,std::string> date;
			record["date"] = dateRows[i].words;
			//record.push_back(date);

			//find the match account words
			for(int j = 0 ; j < accntRows.size(); ++j)
			{
				basetop = dateRows[i].top;
				if(abs(basetop - accntRows[j].top) <= WORD_HALF_HEIGHT)
				{
					//std::map<std::string,std::string> accnt;
					record["accnt"] = accntRows[j].words;
					//record.push_back(accnt);
					basetop = accntRows[j].top;
					break;
				}

		/*		if(accntRows.size() - 1 == j)
				{
					basetop += 10;
				}*/
			}

			//find the match other account words
			for(int j = 0 ; j < otherAccntRows.size(); ++j)
			{
				if(abs(basetop - otherAccntRows[j].top) <= WORD_HALF_HEIGHT)
				{
					//std::map<std::string,std::string> other_accnt;
					record["other_accnt"] = otherAccntRows[j].words;
					//record.push_back(other_accnt);
					basetop = otherAccntRows[j].top;
					break;
				}

				/*if(otherAccntRows.size() - 1 == j)
				{
					basetop += 10;
				}*/
			}

			//find the match amount words
			for(int j = 0 ; j < amountRows.size(); ++j)
			{
				if(abs(basetop - amountRows[j].top) <= WORD_HALF_HEIGHT)
				{
					//std::map<std::string,std::string> amount;
					record["amount"] = amountRows[j].words;
					//record.push_back(amount);
					basetop = amountRows[j].top;
					break;
				}
			}

						//find the match amount words
			for(int j = 0 ; j < balRows.size(); ++j)
			{
				if(abs(basetop - balRows[j].top) <= WORD_HALF_HEIGHT)
				{
					//std::map<std::string,std::string> amount;
					record["balance"] = balRows[j].words;
					//record.push_back(amount);
					basetop = balRows[j].top;
					break;
				}
			}

			records.push_back(record);
		}
	}
	//std::vector<std::map<std::string,std::string>> record;

}


void AccntStatementImg::SplitIntoRows(string strblock,std::vector<StringRow>& strRows)
{
	std::vector<std::string> rows;
	
	CommonUtil::Split(strblock,"\n",rows);

	string result = "";

	int top = 0;

	int bottom = 0;

	for(int i = 0 ; i < rows.size();++i)
	{
		string rowStr = rows[i];

		std::vector<std::string> words;

		CommonUtil::Split(rowStr," ",words);

		if(words.size() == 6)
		{
			if(result == "")
			{
				result += words[0];

				top = atoi(words[2].c_str()); 

				bottom = atoi(words[4].c_str());
			}
			else
			{
				//check the word belong to one row
				int half = abs(top - bottom) / 2;
				int offset = abs(atoi(words[2].c_str()) - top);
				//int offset2 = abs(atoi(words[4].c_str()) - bottom);
				if(offset  < half  )
				{
					result += words[0];
				}
				else
				{
					//it is another row
					//first save it into strRows
					StringRow strrow(result,top,bottom);
					strRows.push_back(strrow);
					//second re-init variable result top bottom 
					result = words[0];

					top = atoi(words[2].c_str()); 

					bottom = atoi(words[4].c_str());
				}

			}
		}

	}

	StringRow strrow(result,top,bottom);
	
	strRows.push_back(strrow);
}