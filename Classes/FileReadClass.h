#ifndef __FILE_READ_CLASS_H__
#define __FILE_READ_CLASS_H__

#include "cocos2d.h"

class FileRead
{
public:
	cocos2d::ValueVector readCSV(const char* fileName);
	cocos2d::ValueVector split(const std::string str, const std::string &delim);
};

#endif