#pragma once

#include "RectShape.h"
#include "SpecialShape.h"
#include "StringShape.h"
#include "BaseImage.h"

class InvoiceImg: public BaseImage
{
public:
	InvoiceImg(char* picName):BaseImage(picName){};
	~InvoiceImg(void);
	void ScanImage();
	std::string RecognizeImage();
	
private:
	std::vector<RectShape> rects;
	std::vector<StringShape> stringShapes;
	StringShape noShape;
	StringShape dateShape;
	SpecialShape specialShape;
	RectShape sellCompany;
	RectShape purchaseCompany;
	RectShape costAccnt;

	std::vector<std::vector<RectShape>> imgMap;
private:
	StringShape GetInvoiceNumber(const std::vector<StringShape>& shapes,SpecialShape shape);

 
};

