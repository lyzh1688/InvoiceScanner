#include "StdAfx.h"
#include "StringRow.h"


StringRow::StringRow(void)
	:words(""),top(0),bottom(0)
{

}

StringRow::StringRow(string str,int _top,int _bottom)
	:words(str),top(_top),bottom(_bottom)
{

}

StringRow::~StringRow(void)
{
}
