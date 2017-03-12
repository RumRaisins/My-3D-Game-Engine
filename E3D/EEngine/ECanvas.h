#pragma once
#include"EGraphics.h"

namespace E3D {
	//绘图接口
	class ECanvas {
	public:
		virtual~ECanvas(){}
		//这里指负责绘制模型
		virtual void onPaint() = 0;
		//更新创景
		virtual void update() = 0;
	};
}