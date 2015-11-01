#include "StdAfx.h"
#include "Eclipse.h"


Eclipse::Eclipse(void)
{
	this->threshold = 0;
}


Eclipse::~Eclipse(void)
{
}

int Eclipse::FindEclipse(IplImage* src)
{
	CvMemStorage *stor;
    CvSeq *cont;
    CvBox2D32f *box;
    CvPoint *PointArray;
    CvPoint2D32f *PointArray2D32f;

    stor = cvCreateMemStorage(0);
    cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),stor);

    cvThreshold(src,src,this->threshold,255,CV_THRESH_BINARY);

    cvFindContours(src,stor,&cont,sizeof(CvContour),
        CV_RETR_LIST,CV_CHAIN_APPROX_NONE,cvPoint(0,0));

    //cvZero(image02);
    //cvZero(image04);

    //������������������Բ���
    for (;cont;cont = cont ->h_next)
    {
        int i;
        int count= cont->total;//��������
        CvPoint center;
        CvSize size;

        /*�����������6������cvFitEllipse_32f��Ҫ��*/
        if (count < 6)
        {
            continue;
        }
   
        //�����ڴ���㼯
        PointArray = (CvPoint *)malloc(count*sizeof(CvPoint));
        PointArray2D32f = (CvPoint2D32f*)malloc(count*sizeof(CvPoint2D32f));
   
        //�����ڴ����Բ����
        box = (CvBox2D32f *)malloc(sizeof(CvBox2D32f));

        //�õ��㼯���������ֵ�ý����
        cvCvtSeqToArray(cont,PointArray,CV_WHOLE_SEQ);
   
        //��CvPoint�㼯ת��ΪCvBox2D32f����
        for (i=0;i<count;i++)
        {
            PointArray2D32f[i].x=(float)PointArray[i].x;
            PointArray2D32f[i].y=(float)PointArray[i].y;
        }

        //��ϵ�ǰ����
        cvFitEllipse(PointArray2D32f,count,box);
		/*
        //���Ƶ�ǰ����
        cvDrawContours(image04,cont,CV_RGB(255,255,255),CV_RGB(255,255,255),
            0,1,8,cvPoint(0,0));

        //����Բ���ݴӸ���ת��Ϊ������ʾ
        center.x = cvRound(box->center.x);
        center.y = cvRound(box->center.y);
        size.width = cvRound(box->size.width*0.5);
        size.height = cvRound(box->size.height*0.5);
        box->angle = -box->angle;

        //����Բ
        cvEllipse(image04,center,size,box->angle,0,360,CV_RGB(0,0,255),1,CV_AA,0);

        free(PointArray);
        free(PointArray2D32f);
        free(box);
    }
    cvShowImage("Result",image04);
	*/

	}
	return 0;
}