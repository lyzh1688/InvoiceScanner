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

    //绘制所有轮廓并用椭圆拟合
    for (;cont;cont = cont ->h_next)
    {
        int i;
        int count= cont->total;//轮廓个数
        CvPoint center;
        CvSize size;

        /*个数必须大于6，这是cvFitEllipse_32f的要求*/
        if (count < 6)
        {
            continue;
        }
   
        //分配内存给点集
        PointArray = (CvPoint *)malloc(count*sizeof(CvPoint));
        PointArray2D32f = (CvPoint2D32f*)malloc(count*sizeof(CvPoint2D32f));
   
        //分配内存给椭圆数据
        box = (CvBox2D32f *)malloc(sizeof(CvBox2D32f));

        //得到点集（这个方法值得借鉴）
        cvCvtSeqToArray(cont,PointArray,CV_WHOLE_SEQ);
   
        //将CvPoint点集转化为CvBox2D32f集合
        for (i=0;i<count;i++)
        {
            PointArray2D32f[i].x=(float)PointArray[i].x;
            PointArray2D32f[i].y=(float)PointArray[i].y;
        }

        //拟合当前轮廓
        cvFitEllipse(PointArray2D32f,count,box);
		/*
        //绘制当前轮廓
        cvDrawContours(image04,cont,CV_RGB(255,255,255),CV_RGB(255,255,255),
            0,1,8,cvPoint(0,0));

        //将椭圆数据从浮点转化为整数表示
        center.x = cvRound(box->center.x);
        center.y = cvRound(box->center.y);
        size.width = cvRound(box->size.width*0.5);
        size.height = cvRound(box->size.height*0.5);
        box->angle = -box->angle;

        //画椭圆
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