#pragma once

#include"ECommon.h"
#include"EOgreMeshInfo.h"
#include"ECore.h"
#include<fstream>

namespace E3D {

	//从stream中读取字符串
	//size表示要读取字符串的个数，如果size==-1，则读取一行字符串
	EString ReadString(std::ifstream &in, EInt size = -1);

	//读取Ogre mesh文件时使用
	//从stream中读取下一个标识符，对应 OGERMESHID
	EUShort ReadChunk(std::ifstream &in);

	//从stream中读取一个bool型数值
	EBool ReadBool(std::ifstream &in);

	//从stream中读取一个unsigned int类型的数值
	EUInt ReadInt(std::ifstream &in);
	//从stream中读取一个unsigned short类型的数值
	EUShort ReadShort(std::ifstream &in);
	//从stream中读取一个T类型的一组数据
	//size表示数组的大小
	template<typename T>
	std::vector<T>ReadArray(std::ifstream &in, EInt size) {
		std::vector<T>v(size, 0);
		in.read((char*)(&v[0]), sizeof(T)*size);
		return v;
	}
	//这个函数加载一个Ogre.Mesh格式的二进制文件，函数实现了对Ogre.Mesh文件静态模型的正确读取
	//可以获取模型的顶点索引和顶点数据等模型数据，并返回EObject对象，如多模型有多个SubMesh，则可以通过EObject.nextObject来获取下一个submesh的指针
	EObject4D* LoadOgreMesh(const EString meshName, const EVector4D &worldPos = EVector4D::ZERO, const EVector4D &scale = EVector4D(1, 1, 1));

	//.X模型结构
	//xof 0302txt 0063
	//xof:固定 0302：表示DirectX3.2版本的模板
	//txt： txt格式（X文件分为txt格式和二进制格式两种）
	//0064：表示浮点型的位数是64位
	//template Header{
	//	<3D82AB43 - 62DA - 11cf - AB39 - 0020AF71E433>
	//	//是GUID(Global Unique Identifier,全局唯一标识符
	//	WORD major;         //X文件的最大版本
	//	WORD minor;			//X文件的最小版本
	//	DWORD flags;		//定义方式
	//}
	//template Mesh{
	//	<3D82AB43 - 62DA - 11cf - AB39 - 0020AF71E433>
	//	DWOED nVertices;                     //顶点个数
	//	array Vector vertices[nVertices];	//顶点数组
	//	DWOED nFaces;						//面个数
	//	array MeshFace face[nFace];			//面数组
	//	[...]								//表示该模板是开发的模板，即还可以添加其他信息
	//}
	/*template Vector{
		<3D82AB43 - 62DA - 11cf - AB39 - 0020AF71E433>
		float x; float y; float z;
	}*/
	//template MeshFace{
	//	<3D82AB43 - 62DA - 11cf - AB39 - 0020AF71E433>
	//	DWORD nfaceVertexIndices;                          //索引个数（如三角形有三个顶点，则索引数为3
	//	array DWORD faceVertexIndices[nfaceVerIndices];	   //顶点索引列表
	//}
}