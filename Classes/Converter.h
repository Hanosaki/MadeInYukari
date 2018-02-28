#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include "cocos2d.h"

/// <summary>
/// リネーム，string→char*変換を行うクラス
/// 一部Windowsのみで動作するため，Windows以外の環境では使用しないこと
/// </summary>

class Converter
{
public:
	char* replaceString2Char(std::string);
};


#endif