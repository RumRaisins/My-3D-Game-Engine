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

	//将val的范围限制在min-max之间
	template<typename T>
	const T& Clamp(const T& val, const T& minV, const T& maxV) {
		return Min(Max(val, maxV), minV);
	}
	//获取平移变换矩阵
	void GetTranslateMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z);

	//获取缩放变换矩阵
	void GetScaleMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z);

	//获取沿X轴旋转变换矩阵
	//旋转参数为角度，非弧度

	
}