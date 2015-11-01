#pragma once
#include <string>
using namespace std;

class StringRow
{
public:
	StringRow(void);
	StringRow(string str,int _top,int _bottom);
	~StringRow(void);

public:
	string words;
	int top;
	int bottom;
	int confidence;
};

