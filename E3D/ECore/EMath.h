#pragma once

#include"ECommon.h"

namespace E3D {



	//将val的范围限制在min-max之间
	template<typename T>
	const T& Clamp(const T& val, const T& minV, const T& maxV) {
		return Min(Max(val, maxV), minV);
	}
}