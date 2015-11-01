#include "StdAfx.h"
#include "RectDetector.h"

int RectDetector::FindShape(const IplImage* img,std::vector<RectShape>& shapes)
{
	if( !img )
	{
        return -1;
	}

	int thresh = 50;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours;
	int i, c, l, N = 11;
	CvSize sz = cvSize( img->width & -2, img->height & -2 ); 
 
	IplImage* timg = cvCloneImage( img );
	IplImage* gray = cvCreateImage( sz, 8, 1 );
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
	IplImage* tgray;
	CvSeq* result;
	double s, t;
	// ����һ�����������ڴ洢�����ǵ�
	//CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );

	cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));
	// ��������
	//cvPyrDown( timg, pyr, 5 );
	//cvPyrUp( pyr, timg, 5 );
	tgray = cvCreateImage( sz, 8, 1 );
	//cvShowImage("test",timg);
 

	// ��ȡ the c-th color plane
	cvSetImageCOI( timg, 0 );
	cvCopy( timg, tgray, 0 );

	// ���Ը�����ֵ��ȡ�õ��ģ�N=11��
	for( l = 0; l < N; l++ )
	{
		// apply Canny. Take the upper threshold from slider
		// Canny helps to catch squares with gradient shading  
		if( l == 0 )
		{
			cvCanny( tgray, gray, 0, thresh, 5 );
			//ʹ������ṹԪ������ͼ��
			cvDilate( gray, gray, 0, 2 );
			//cvShowImage("test",gray);
		}
		else
		{
			// apply threshold if l!=0:
			cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
 
		}

		// �ҵ������������Ҵ洢��������
		cvFindContours( gray, storage, &contours, sizeof(CvContour),
		CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

		// �����ҵ���ÿ������contours
		while( contours )
		{
			//��ָ�����ȱƽ����������
			result = cvApproxPoly( contours, sizeof(CvContour), storage,
			CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
                  

			if( result->total == 4 &&
				fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 500 &&
				fabs(cvContourArea(result,CV_WHOLE_SEQ)) < 100000 &&
				cvCheckContourConvexity(result) )
			{
				s = 0;

				for( i = 0; i < 5; i++ )
				{
					// find minimum angle between joint edges (maximum of cosine)
					if( i >= 2 )
					{
						t = fabs(this->CalcAngle((CvPoint*)cvGetSeqElem( result, i ),
										(CvPoint*)cvGetSeqElem( result, i-2 ),
										(CvPoint*)cvGetSeqElem( result, i-1 )));
						s = s > t ? s : t;
					}
				}

				 // if ����ֵ �㹻С�������϶��Ƕ�Ϊ90��ֱ��
				 //cos0.1=83�ȣ��ܽϺõ�����ֱ��
				if( s < 0.1 ) 
				{
					RectShape RectShape(*(CvPoint*)cvGetSeqElem( result, 0 ),*(CvPoint*)cvGetSeqElem( result, 1 ),*(CvPoint*)cvGetSeqElem( result, 2 ),*(CvPoint*)cvGetSeqElem( result, 3 ));
					
					bool isExist = false;
					//ȥ��
					for(int j = 0 ; j < shapes.size(); ++j)
					{
						if(RectShape == shapes[j] )
						{
							isExist = true;
							continue;
						}
						//remove the rectangle which contains others
						if(RectShape.isNear(shapes[j]))
						{
							if(RectShape.ctPoint.x > shapes[j].ctPoint.x || RectShape.ctPoint.y > shapes[j].ctPoint.y)
							{
								isExist = true;
								continue;
							}
							else
							{
								shapes[j] = RectShape;
							}
						}
	 

					}

					if(!isExist)
					{
						shapes.push_back(RectShape);
					}
					
				}
			}

			// ����������һ������
			contours = contours->h_next;
		

		}
	}

	cvReleaseImage( &gray );
	cvReleaseImage( &pyr );
	cvReleaseImage( &tgray );
	cvReleaseImage( &timg );
	cvClearMemStorage( storage );
	


	return shapes.size();
}

void RectDetector::DrawShape(IplImage* dst,const std::vector<RectShape>& shapes)
{
		// read 4 sequence elements at a time (all vertices of a square)
	for(int i = 0; i < shapes.size(); ++i )
	{
		CvPoint pt[4], *RectShape = pt;
		int count = 4;

		pt[0] = shapes[i].ltPoint;
		pt[1] = shapes[i].lbPoint;
		pt[2] = shapes[i].rbPoint;
		pt[3] = shapes[i].rtPoint;

		// draw the square as a closed polyline
		cvPolyLine( dst, &RectShape, &count, 1, 1, CV_RGB(0,255,0), 2, CV_AA, 0 );
	}

}

//angle�����������أ���������֮���ҵ��Ƕȵ�����ֵ��
double RectDetector::CalcAngle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt0->x;
	double dy2 = pt2->y - pt0->y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

//int RectDetector::SplitIntoRows(int columnCnt,const std::vector<RectShape>& cellRects,std::vector<std::map<int,RectShape>>& rows)
//{
//	sort(cellRects.begin(),cellRects.end(),)
//	for(int i = 0 ; i < cellRects.size();++i)
//	{}
//}
