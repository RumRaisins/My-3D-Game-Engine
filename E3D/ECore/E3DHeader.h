#pragma once   //头文件只能被引用一次

namespace E3D {          //模块化封装

	struct EColor;      //颜色
	struct EVector2D;   //二维向量
	struct EVector3D;   //三维向量
	struct EVector4D;   //四维向量
	struct EMatrix44;   //4x4矩阵


	struct EVertex4D;  //顶点
	struct EPolyon4D;  //基于顶点多边形
	struct EPolyonF4D; //基于索引多边形
	struct EObject4D;  //对象
	struct EPlane3D;   //三维平面
	struct ERenderList4D; //渲染列表


	struct EFrustum;   //透视体

}

