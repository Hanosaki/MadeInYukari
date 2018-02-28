#pragma execution_character_set("utf-8")
#include "Converter.h"
#include "FileReadClass.h"

USING_NS_CC;

char* Converter::replaceString2Char(std::string str)
{
	auto returnFileName = new char[str.length() + 1];
	memcpy(returnFileName, str.c_str(), str.length() + 1);
	return returnFileName;
}