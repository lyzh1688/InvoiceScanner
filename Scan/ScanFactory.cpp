#include "StdAfx.h"
#include "ScanFactory.h"
#include "InvoiceImg.h"
#include "StatementImg.h"
#include "ThresholdController.h"
#include "AccntStatementImg.h"
ScanFactory::ScanFactory(void)
{
}


ScanFactory::~ScanFactory(void)
{
}

BaseImage* ScanFactory::CreateImage(string imgType,string columnConf,char* imgfile)
{
	BaseImage* pImage = NULL;
	ThresholdController::SetImgType(imgType); 
	if("010101" == imgType)
	{
		 pImage = new StatementImg(imgfile,columnConf);
	}
	if("010102" == imgType)
	{
		 pImage = new AccntStatementImg(imgfile);
	}
	if("020101" == imgType)
	{
		
		pImage = new InvoiceImg(imgfile);
	}

	return pImage;
}