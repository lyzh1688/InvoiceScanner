#include "StdAfx.h"
#include "InvoiceImg.h"
#include "LineDetector.h"
#include "RectDetector.h"
#include "SpecialShapeDetector.h"
#include "StringShapeDetector.h"
#include <algorithm>
#include "CommonUtil.h"
#include  "define.h"
//InvoiceImg::InvoiceImg(char* picName) 
//	:BaseImage(picName)
//{
//	
//}


InvoiceImg::~InvoiceImg(void)
{
	//std::cout << "InvoiceImg release image" << std::endl;
}


StringShape InvoiceImg::GetInvoiceNumber(const std::vector<StringShape>& shapes,SpecialShape shape)
{
	//SpecialShape* pShape = (SpecialShape*) p;
	int index = -1;
	double distance = 0;
	for(int i = 0; i < shapes.size(); ++i)
	{
		if( shape.ctPoint.y >= shapes[i].wordRect.ltPoint.y - OFFSET_NO_POS
			&& shape.ctPoint.y <= shapes[i].wordRect.lbPoint.y + OFFSET_NO_POS)
		{
			double tmpdistance = Line::CalcLineLen(shapes[i].wordRect.ctPoint,shape.ctPoint);

			if(tmpdistance < distance || distance == 0)
			{
				distance = tmpdistance;
				index = i;
			}
		}
	}

	if(index != -1)
	{
		return shapes[index];
	}
	else
	{
		return shapes[0];
	}
}

std::string InvoiceImg::RecognizeImage()
{
	std::string retStr = "{";

	retStr += "\"No\":";

	retStr += "\"" + this->RecognizeRect(this->noShape.wordRect,"chi_sim") + "\",";

	retStr += "\"Purchase\":";

	retStr += "\"" + this->RecognizeRect(this->purchaseCompany,"chi_sim") + "\",";

	retStr += "\"Sell\":";

	retStr += "\"" + this->RecognizeRect(this->sellCompany,"chi_sim") + "\",";

	retStr += "\"Amount\":";

	retStr += "\"" + this->RecognizeRect(this->costAccnt,"chi_sim") + "\"";

	retStr +="}";

	return retStr;

}



void InvoiceImg::ScanImage()
{
	LineDetector lineDetector;
	RectDetector rectDetector;
	SpecialShapeDetector specialShapeDetector;
	StringShapeDetector stringShapeDetector;

	std::vector<Line> AllLines;
	std::vector<Line> HorizontalLines;
	std::vector<Line> VerticalLines;
	std::vector<Line> LongLines;
	//std::vector<StringShape> stringShapes;

	lineDetector.FindShape(this->contoursImg,AllLines);

	HorizontalLines = lineDetector.GetLinesByType(AllLines,HORIZONTAL);

	VerticalLines =  lineDetector.GetLinesByType(AllLines,VERTICAL);

	LongLines = lineDetector.FilterShortLine(HorizontalLines,INVOICE_SHORTEST_LINE_SIZE);

	lineDetector.DrawShape(this->clearImg,LongLines);

	lineDetector.DrawShape(this->clearImg,VerticalLines);


 	//cvShowImage("getHoughLines",temp1);
	//resize(temp1,"getHoughLines");
	int rectcount = rectDetector.FindShape(this->clearImg,this->rects);

	//std::cout << "RectShape count : " << rectcount;
	
	
	this->specialShape = specialShapeDetector.FindShape(this->resizedImg,NO);

	stringShapeDetector.FindShape(this->resizedImg,this->stringShapes);

	if(this->stringShapes.size() > 0)
	{
		this->noShape = this->GetInvoiceNumber(this->stringShapes,this->specialShape);
	}
	 
	//sort(this->rects.begin(),this->rects.end(),RectShape::RectPosCmp);

	std::vector<std::vector<RectShape>> allRowRects = this->SplitIntoRow(this->rects);

	this->purchaseCompany = this->GetRectByIndex(allRowRects,0,1).MoveOffset(150,0,LEFT);

	this->sellCompany = this->GetRectByIndex(allRowRects,3,1).MoveOffset(150,0,LEFT);

	this->costAccnt = this->GetRectByIndex(allRowRects,2,1);
	
	
	
	//rectDetector.DrawShape(this->resizedImg,this->rects);

	//specialShapeDetector.DrawShape(this->resizedImg,this->specialShape);

	//this->noShapes.Draw(this->resizedImg);
	
}




