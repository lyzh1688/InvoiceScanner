#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <direct.h>
#include <windows.h>
#include <string>


typedef enum SHAPE_TYPE 
{
	NO = 0

} ShapeType;


static class ShapeMap
{
public:

	//static string GetProgramDir()  
	//{   
	//	LPSTR path = new char[MAX_PATH];//path[MAX_PATH] = {0};
	//	ZeroMemory(path,MAX_PATH);
	//	::GetModuleFileNameA(NULL,path,MAX_PATH);

	//	string curPath(path);
	//	delete[] path;
	//	return curPath;
	//}   


	static cv::Mat GetTrainedImg(ShapeType shapeType)
	{
		cv::Mat trainedImg;

		std::string home = _pgmptr;

		home = home.substr(0,home.find_last_of("\\"));

		switch(shapeType)
		{
			case NO:
				trainedImg = cv::imread(home + "\\resource\\NO.png",1);
				break;
			default:
				break;
		}

		if(!trainedImg.data)
		{
			std::cout << "trainedImg does not exist" << std::endl;
		}

		return trainedImg;
	};
};