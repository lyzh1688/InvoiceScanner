#pragma once
#include "BaseImage.h"
#include <string>
#include "StringRow.h"

using namespace std;

class AccntStatementImg: public BaseImage
{
public:
	AccntStatementImg(char* picName):BaseImage(picName){};

	~AccntStatementImg(void);

	void ScanImage();

	std::string RecognizeImage();

	void SplitIntoRows(string strblock,std::vector<StringRow>& strRows);

	void Merge( std::vector<std::map<std::string,std::string>>& records,
				const std::vector<StringRow>& dateRows,
				const std::vector<StringRow>& accntRows,
                const std::vector<StringRow>& otherAccntRows,
                const std::vector<StringRow>& amountRows,
				const std::vector<StringRow>& balRows);

	
private:

	std::vector<RectShape> rects;

private:
	int CalcRects(std::vector<Line> _lines,std::vector<RectShape>& allRects);

	std::vector<CvPoint> GetExtremePoint(std::vector<CvPoint> allpt,LineType type);
 
public:
	const static int verticalLineNum = 10;
};

