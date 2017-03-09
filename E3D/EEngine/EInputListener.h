#pragma once
#include"ECommon.h"

namespace E3D {
	//Êó±ê¼üÅÌ¼àÌýÆ÷
	class EInputListener {
		public:
			virtual EBool KeyPress(EInt key) = 0;
			virtual EBool KeyRlease(EInt key) = 0;

			virtual EBool mouseButtonDown(EInt mouseButton) = 0;
			virtual EBool mouseButtonRelease(EInt mouseButton) = 0;
			virtual EBool mouseMove(EInt x, EInt y) = 0;
			virtual EBool mouseWheel(EInt delta) = 0;

			virtual ~EInputListener() {}
	};
}