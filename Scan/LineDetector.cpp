#include "StdAfx.h"
#include "LineDetector.h"
#include <highgui.h>
#include "define.h"
#include "ThresholdController.h"
 
int LineDetector::FindShape(const IplImage* img,std::vector<Line>& shapes)
{
	CvSeq* lines = NULL;
	
	CvMemStorage* storage = cvCreateMemStorage(0);

    if( !img )
	{
        return -1;
	}
 
	CvSize sz = cvSize( img->width & -2, img->height & -2 ); 
  
	// π˝¬À‘Î“Ù(Œﬁ–ß)
	IplImage* src = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, img->nChannels);
	cvSmooth(img, src, CV_GAUSSIAN, 7, src->nChannels);
	/*
	IplImage* timg = cvCloneImage( src );
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
	cvPyrDown( timg, pyr, 7 );
	cvPyrUp( pyr, timg, 7 );
	*/
	
	IplImage* cannyImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1); 
	IplImage* dilateImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);  
	
	cvCanny(src, cannyImg, ThresholdController::GetInstance().CANNY_THRESHOLD1, ThresholdController::GetInstance().CANNY_THRESHOLD2, 3);
	//cvCanny(src, cannyImg, 50, 200, 3);
	//cvShowImage("cannyImg",cannyImg);
 
	cvDilate( cannyImg,dilateImg, NULL,2); //≈Ú’Õ
	cvErode(dilateImg,dilateImg, NULL,2);
	//cvShowImage("dilateImg",dilateImg);
    //lines = cvHoughLines2(dilateImg, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 10, 1, 10);
	lines = cvHoughLines2(dilateImg, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 
						  ThresholdController::GetInstance().HOUGH_THRESHOLD1, 
						  ThresholdController::GetInstance().HOUGH_THRESHOLD2, 
						  ThresholdController::GetInstance().HOUGH_THRESHOLD3);
    for (int i = 0; i < lines->total; i++)
    {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
	
		Line _line(line[0],line[1]);
		if ( _line.endPoint.y > 10 &&  _line.endPoint.y < src->height -10)
		{
			if(!this->IsExist(_line,shapes))
			{
				shapes.push_back(_line);
			}
		}
    }
 
	this->HorizontalLines = this->GetLinesByType(shapes,HORIZONTAL);
	this->VerticalLines = this->GetLinesByType(shapes,VERTICAL);

	cvReleaseImage( &src );
	cvReleaseImage( &cannyImg );
	cvReleaseImage( &dilateImg );
	cvClearMemStorage( storage );
	//std::cout << "HorizontalLines : " << HorizontalLines.size() << std::endl;
	//std::cout << "lines : " << shapes.size() << std::endl;
	return shapes.size();
}

int LineDetector::FindShape(const IplImage* src,std::vector<Line>& shapes,LineFilterAlgo algo)
{
	std::vector<Line> tmpShapes;
	this->FindShape(src,tmpShapes);
	if(algo == WITHOUT_NO_JOINT)
	{
		for(int i = 0 ; i < this->HorizontalLines.size(); ++i)
		{
			for(int j = 0 ; j <  this->VerticalLines.size(); ++j)
			{
				if(this->HorizontalLines[i].HasJoint(this->VerticalLines[j]))
				{
					shapes.push_back(this->HorizontalLines[i]);
					continue;
				}
			}
		}

		for(int j = 0 ; j <  this->VerticalLines.size(); ++j)
		{
			shapes.push_back(this->VerticalLines[j]);
		}
	}

	return shapes.size();
}

void LineDetector::DrawShape(IplImage* src,const std::vector<Line>& shapes)
{
	for(int i = 0 ; i < shapes.size(); ++i)
	{
		cvLine(src, shapes[i].bgnPoint, shapes[i].endPoint, CV_RGB(0, 0, 0), 2, CV_AA, 0);
	}
}

int LineDetector::GetLongLine(double minLen,const std::vector<Line>& lines,std::vector<Line>& longLines)
{
	 
	//sort(lines.begin(),lines.end(),cmp_func_line_len);

	for(int i = 0 ; i < lines.size(); ++i)
	{
		if(lines[i].lineLen > minLen)
		{
			longLines.push_back(lines[i]);
		}
	}

	return lines.size();
}
std::vector<Line>  LineDetector::FilterShortLine(const std::vector<Line>& lines,int minLen)
{
	std::vector<Line> tmp;
	for(int i = 0 ; i < lines.size();++i)
	{
		if(lines[i].lineLen > minLen)
		{
			tmp.push_back(lines[i]);
		}
	}

	return tmp;
}

std::vector<Line>  LineDetector::GetLinesByType(const std::vector<Line>& lines,LineType type)
{
	std::vector<Line> lineWithType;
	for(int i = 0 ; i < lines.size(); ++i)
	{
		if(lines[i].lineType == type)
		{
			lineWithType.push_back(lines[i]);
		}
	}

	return lineWithType;
}

bool LineDetector::IsExist(Line& line,const std::vector<Line>& list)
{
	bool isexist = false;
	for(int i = 0 ; i < list.size();++i)
	{
		if(line == list[i])
		{
			isexist = true;
			break;
		}
	}

	return isexist;
}

int LineDetector::Merge(std::vector<Line>& srcList,std::vector<Line>& dstList,LineType lineType)
{

	const int std_offset = 10;
	int offset = 0;

	switch(lineType)
	{
	case HORIZONTAL:
		sort(srcList.begin(),srcList.end(),Line::CmpLessPointY);
		break;
	case VERTICAL:
		sort(srcList.begin(),srcList.end(),Line::CmpLessPointX);
		break;
	default:
		return 0;
	}

	for(int i = 0 ; i < srcList.size();++i)
	{

		if(dstList.size() == 0)
		{
			dstList.push_back(srcList[i]);
		}
		else
		{
			if(HORIZONTAL == lineType)
			{
				offset = abs((dstList[dstList.size() - 1].bgnPoint.y + dstList[dstList.size() - 1].endPoint.y) / 2 - (srcList[i].bgnPoint.y + srcList[i].endPoint.y) / 2);
			}
			if(VERTICAL == lineType)
			{
				offset = abs((dstList[dstList.size() - 1].bgnPoint.x + dstList[dstList.size() - 1].endPoint.x) / 2 - (srcList[i].bgnPoint.x + srcList[i].endPoint.x) / 2);
			}
			
			if(std_offset >= offset)
			{
				if(HORIZONTAL == lineType)
				{
					dstList[dstList.size() - 1].bgnPoint = (dstList[dstList.size() - 1].bgnPoint.x < srcList[i].bgnPoint.x ) ? dstList[dstList.size() - 1].bgnPoint : srcList[i].bgnPoint;
					dstList[dstList.size() - 1].endPoint = (dstList[dstList.size() - 1].endPoint.x > srcList[i].endPoint.x ) ? dstList[dstList.size() - 1].endPoint : srcList[i].endPoint;
				}

				if(VERTICAL == lineType)
				{
					dstList[dstList.size() - 1].bgnPoint = (dstList[dstList.size() - 1].bgnPoint.y < srcList[i].bgnPoint.y ) ? dstList[dstList.size() - 1].bgnPoint : srcList[i].bgnPoint;
					dstList[dstList.size() - 1].endPoint = (dstList[dstList.size() - 1].endPoint.y > srcList[i].endPoint.y ) ? dstList[dstList.size() - 1].endPoint : srcList[i].endPoint;
				}
			}
			else
			{
				dstList.push_back(srcList[i]);
			}
		}
	}

	//update the line length
	for(int i = 0 ; i < dstList.size(); ++i)
	{
		dstList[i].lineLen = Line::CalcLineLen(dstList[i].bgnPoint,dstList[i].endPoint);
	}

	return dstList.size();
}
