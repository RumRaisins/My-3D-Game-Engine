#pragma once

#include"ECommon.h"

namespace E3D {



	//��val�ķ�Χ������min-max֮��
	template<typename T>
	const T& Clamp(const T& val, const T& minV, const T& maxV) {
		return Min(Max(val, maxV), minV);
	}
}