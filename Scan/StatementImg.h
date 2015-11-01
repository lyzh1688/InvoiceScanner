#pragma once
#include "BaseImage.h"
#include <string>
class StatementImg: public BaseImage
{
public:
	StatementImg(char* picName,std::string _columnConf):BaseImage(picName),columnConf(_columnConf)
	{};

	~StatementImg(void);

	void ScanImage();

	std::string RecognizeImage();

private:
	int CalcVerticalLines(std::vector<Line>& _lines,std::vector<Line>& VerticalLines);

	int CalcRects(std::vector<Line>& _vlines,std::vector<Line>& _hlines,std::vector<std::vector<RectShape>>& allRowRects);

public:
	std::string columnConf;

private:
	std::vector<std::vector<RectShape>> allRowRects;

	std::vector<RectShape> allColumnRects;

};

