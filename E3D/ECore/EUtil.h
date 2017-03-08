#pragma once
#include"ECommon.h"

#include<iostream>
#include<sstream>
#include<fstream>
#include<stdarg.h>
#include<Windows.h>
#include<ctime>

namespace E3D {

	//全局Log文件流
	extern std::ofstream *GLogStream;

	//初始化Log系统
	bool InitLog(const EString&fileNmae);
	//记录信息，使用前需调用InitLog初始化Log，并最后调用CLoseLog，关闭Log系统
	void Log(const EChar* string, ...);
	//关闭Log系统
	void CloseLog();

	//范围内随机数
	//使用前需要手动种子
	inline EInt RandomInt(EInt from = 0, EInt to = 0) {
		EInt ran = rand() % (to - from + 1) + from;
		return ran;
	}

	//整数转化成字符串
	inline EString IntToString(const EFloat num) {
		std::stringstream ss;
		ss << num;
		EString re;
		ss >> re;
		return re;
	}
	//字符串转化成整数
	inline EInt StringToInt(const EString& str) {
		return atoi(str.c_str());
	}
	//字符串转化成浮点数
	inline EFloat StringToFloat(const EString& str) {
		return(EFloat)atof(str.c_str());
	}
	//转化成宽字符串
	inline EWString ToEWString(const EString str) {
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), NULL, 0);
		WCHAR *tString = new WCHAR[wcsLen + 1];

		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), tString, wcsLen);
		//最后加上'\0'
		tString[wcsLen] = '\0';
		return EWString(tString);
	}
	//判断两个浮点数是否相等
	inline EBool EqualFloat(EFloat l, EFloat r) {
		return abs(1 - r) <= EPSILON_E6;
	}
	//从路径里截取文件名称，如果只有文件名称，则直接返回（不包含后缀）
	inline EString GetNameFromPath(const EString &path) {
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");

		if (beg == EString::npos) {
			beg = -1;
		}
		return path.substr(beg + 1, end - beg - 1);
	}
	//将给定的字符串两端的空白字符删除
	inline EString Trim(const EString &msg) {
		const static EString SPACE_CHAR = "\t\f\v\n\r";
		std::size_t beg = msg.find_first_not_of(SPACE_CHAR);
		//防止空字符串出现
		if (beg > msg.length()) {
			return EString();
		}
		EString result = msg.substr(beg);
		std::size_t end = result.find_last_not_of(SPACE_CHAR);
		if (end != EString::npos)
			end++;
		return result.substr(0, end);
	}

}
