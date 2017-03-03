#pragma once

#include"ECommon.h"

namespace E3D {

	template<class T>
	const T& Min(const T& a, const T& b) {
		return (a < b) ? a : b;
	}
	template<class T>
	const T& Max(const T& a, const T& b) {
		return (b < a) ? a : b;
	}

	//��val�ķ�Χ������min-max֮��
	template<typename T>
	const T& Clamp(const T& val, const T& minV, const T& maxV) {
		return Min(Max(val, maxV), minV);
	}
	//��ȡƽ�Ʊ任����
	void GetTranslateMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z);

	//��ȡ���ű任����
	void GetScaleMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z);

	//��ȡ��X����ת�任����
	//��ת����Ϊ�Ƕȣ��ǻ���

	
}