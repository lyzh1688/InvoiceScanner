#include "StdAfx.h"
#include "ImgProcessor.h"
#include "SpecialShape.h"
#include "StringShape.h"

ImgProcessor::ImgProcessor(void)
{
}


ImgProcessor::~ImgProcessor(void)
{
}

IplImage* ImgProcessor::ResizeImg(IplImage* &img, double scale)
{
	IplImage* dst = 0;
	CvSize dst_cvsize;   //Ŀ��ͼ��ߴ�
	dst_cvsize.width = img->width * scale;  //Ŀ��ͼ��Ŀ�ΪԴͼ����scale��  
    dst_cvsize.height = img->height * scale; //Ŀ��ͼ��ĸ�ΪԴͼ��ߵ�scale��  
    dst = cvCreateImage( dst_cvsize, img->depth,img->nChannels); //����Ŀ��ͼ��  
    cvResize(img, dst, CV_INTER_LINEAR); //����Դͼ��Ŀ��ͼ�� 
	cvReleaseImage(&img);
	return dst;
}


void ImgProcessor::ResizeImg(const IplImage* src,IplImage* &dst)
{
	//the src image must be resize to 1600*1200,as some threshold is based on the size
	CvSize dst_cvsize;   //Ŀ��ͼ��ߴ�
	dst_cvsize.width = 1600;  
	dst_cvsize.height = 1200;  
	
	dst = cvCreateImage( dst_cvsize, src->depth,src->nChannels); //����Ŀ��ͼ��  
	
	cvResize(src, dst, CV_INTER_LINEAR); //����Դͼ��Ŀ��ͼ�� 
}

void ImgProcessor::GetImageRect(IplImage* orgImage, CvRect rectInImage, IplImage* imgRect)
{
	//��ͼ��orgImage����ȡһ�飨rectInImage����ͼ��imgRect
	IplImage *result = imgRect;

	CvSize size;

	size.width=rectInImage.width;

	size.height=rectInImage.height;
	//result=cvCreateImage( size, orgImage->depth, orgImage->nChannels );
	//��ͼ������ȡ��ͼ��
	cvSetImageROI(orgImage,rectInImage);
	cvCopy(orgImage,result);



	cvResetImageROI(orgImage);
}

void ImgProcessor::GetImgContours(int threshold,const IplImage* src,IplImage* &dst)
{
	CvMemStorage *stor;
    CvSeq *cont;

    stor = cvCreateMemStorage(0);
    cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),stor);

    cvThreshold(src,dst,threshold,255,CV_THRESH_BINARY);
	 
    cvFindContours(dst,stor,&cont,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE,cvPoint(0,0));

   

    //������������������Բ���
    for (;cont;cont = cont ->h_next)
    {
        //���Ƶ�ǰ����
        cvDrawContours(dst,cont,CV_RGB(255,0,255),CV_RGB(0,255,0),0,1,8,cvPoint(0,0));
    }
	
	cvClearMemStorage(stor);
    
}

void ImgProcessor::GetInvoiceNo(const std::vector<StringShape>& shapes,void* p,std::vector<StringShape>& resShapes )
{
	SpecialShape* pShape = (SpecialShape*) p;
	int index = -1;
	double distance = 0;
	for(int i = 0; i < shapes.size(); ++i)
	{
		if( (*pShape).ctPoint.y >= shapes[i].wordRect.ltPoint.y - OFFSET_NO_POS
			&& (*pShape).ctPoint.y <= shapes[i].wordRect.lbPoint.y + OFFSET_NO_POS)
		{
			double tmpdistance = Line::CalcLineLen(shapes[i].wordRect.ctPoint,(*pShape).ctPoint);
			//if(i == 0)
			//{
			//	distance = tmpdistance;
			//	index = i;

			//}
			if(tmpdistance < distance || distance == 0)
			{
				distance = tmpdistance;
				index = i;
			}
		}
	}

	if(index != -1)
	{
		resShapes.push_back(shapes[index]);
	}
	
	
}
