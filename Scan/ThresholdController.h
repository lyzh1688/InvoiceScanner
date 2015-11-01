#pragma once
#include <string>
using namespace std;

class ThresholdController
{
private:
	ThresholdController()   //构造函数是私有的
	{
	}
	ThresholdController(const ThresholdController &);

	ThresholdController & operator = (const ThresholdController &);

public:
	static ThresholdController & GetInstance()
	{
		static ThresholdController instance;   //局部静态变量

		instance.SetThreadshold();

		return instance;
	}

	static void SetImgType(string _imgType)
	{
		ThresholdController::GetInstance().imgType = _imgType;
	}

 
private:
	void SetThreadshold()
	{
		if(this->imgType == "010101")
		{
			this->CANNY_THRESHOLD1 = 50;
			this->CANNY_THRESHOLD2 = 100;
			this->HOUGH_THRESHOLD1 = 50;
			this->HOUGH_THRESHOLD2 = 50;
			this->HOUGH_THRESHOLD3 = 10;

			this->RECT_DISTANCE_Y_OFFSET_1 = 2;
		}
		if(this->imgType == "020101")
		{
			this->CANNY_THRESHOLD1 = 50;
			this->CANNY_THRESHOLD2 = 200;
			this->HOUGH_THRESHOLD1 = 50;
			this->HOUGH_THRESHOLD2 = 50;
			this->HOUGH_THRESHOLD3 = 10;

			this->RECT_DISTANCE_Y_OFFSET_1 = 10;

		}
	}

public:
	string imgType;
	//LINE DETECTOR 
	int CANNY_THRESHOLD1;
	int CANNY_THRESHOLD2;
	int HOUGH_THRESHOLD1;
	int HOUGH_THRESHOLD2;
	int HOUGH_THRESHOLD3;
	//OFFSET
	int RECT_DISTANCE_Y_OFFSET_1;
};