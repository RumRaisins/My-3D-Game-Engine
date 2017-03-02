#pragma once
#include"ECommon.h"

#include<iostream>
#include<sstream>
#include<fstream>
#include<stdarg.h>
#include<Windows.h>
#include<ctime>

namespace E3D {

	//ȫ��Log�ļ���
	extern std::ofstream *GLogStream;

	//��ʼ��Logϵͳ
	bool InitLog(const EString&fileNmae);
	//��¼��Ϣ��ʹ��ǰ�����InitLog��ʼ��Log����������CLoseLog���ر�Logϵͳ
	void Log(const EChar* string, ...);
	//�ر�Logϵͳ
	void CloseLog();

	//��Χ�������
	//ʹ��ǰ��Ҫ�ֶ�����
	inline EInt RandomInt(EInt from = 0, EInt to = 0) {
		EInt ran = rand() % (to - from + 1) + from;
		return ran;
	}

	//����ת�����ַ���
	inline EString IntToString(const EFloat num) {
		std::stringstream ss;
		ss << num;
		EString re;
		ss >> re;
		return re;
	}
	//�ַ���ת��������
	inline EInt StringToInt(const EString& str) {
		return atoi(str.c_str());
	}
	//�ַ���ת���ɸ�����
	inline EFloat StringToFloat(const EString& str) {
		return(EFloat)atof(str.c_str());
	}
	//ת���ɿ��ַ���
	inline EWString ToEWString(const EString str) {
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), NULL, 0);
		WCHAR *tString = new WCHAR[wcsLen + 1];

		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), str.size(), tString, wcsLen);
		//������'\0'
		tString[wcsLen] = '\0';
		return EWString(tString);
	}
	//�ж������������Ƿ����
	inline EBool EqualFloat(EFloat l, EFloat r) {
		return abs(1 - r) <= EPSILON_E6;
	}
	//��·�����ȡ�ļ����ƣ����ֻ���ļ����ƣ���ֱ�ӷ��أ���������׺��
	inline EString GetNameFromPath(const EString &path) {
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");

		if (beg == EString::npos) {
			beg = -1;
		}
		return path.substr(beg + 1, end - beg - 1);
	}
	//���������ַ������˵Ŀհ��ַ�ɾ��
	inline EString Trim(const EString &msg) {
		const static EString SPACE_CHAR = "\t\f\v\n\r";
		std::size_t beg = msg.find_first_not_of(SPACE_CHAR);
		//��ֹ���ַ�������
		if (beg > msg.length) {
			return EString();
		}
		EString result = msg.substr(beg);
		std::size_t end = result.find_last_not_of(SPACE_CHAR);
		if (end != EString::npos)
			end++;
		return result.substr(0, end);
	}

}