#pragma once
#include <string>
#include "BaseImage.h"
using namespace std;
class ScanFactory
{
public:
	ScanFactory(void);
	~ScanFactory(void);

	static BaseImage* CreateImage(string imgType,string columnConf ,char* imgfile);
 
 
};

