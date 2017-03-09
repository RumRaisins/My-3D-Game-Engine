#pragma once

#include"ECommon.h"
#include"EOgreMeshInfo.h"
#include"ECore.h"
#include<fstream>

namespace E3D {

	//��stream�ж�ȡ�ַ���
	//size��ʾҪ��ȡ�ַ����ĸ��������size==-1�����ȡһ���ַ���
	EString ReadString(std::ifstream &in, EInt size = -1);

	//��ȡOgre mesh�ļ�ʱʹ��
	//��stream�ж�ȡ��һ����ʶ������Ӧ OGERMESHID
	EUShort ReadChunk(std::ifstream &in);

	//��stream�ж�ȡһ��bool����ֵ
	EBool ReadBool(std::ifstream &in);

	//��stream�ж�ȡһ��unsigned int���͵���ֵ
	EUInt ReadInt(std::ifstream &in);
	//��stream�ж�ȡһ��unsigned short���͵���ֵ
	EUShort ReadShort(std::ifstream &in);
	//��stream�ж�ȡһ��T���͵�һ������
	//size��ʾ����Ĵ�С
	template<typename T>
	std::vector<T>ReadArray(std::ifstream &in, EInt size) {
		std::vector<T>v(size, 0);
		in.read((char*)(&v[0]), sizeof(T)*size);
		return v;
	}
	//�����������һ��Ogre.Mesh��ʽ�Ķ������ļ�������ʵ���˶�Ogre.Mesh�ļ���̬ģ�͵���ȷ��ȡ
	//���Ի�ȡģ�͵Ķ��������Ͷ������ݵ�ģ�����ݣ�������EObject�������ģ���ж��SubMesh�������ͨ��EObject.nextObject����ȡ��һ��submesh��ָ��
	EObject4D* LoadOgreMesh(const EString meshName, const EVector4D &worldPos = EVector4D::ZERO, const EVector4D &scale = EVector4D(1, 1, 1));

	//.Xģ�ͽṹ
	//xof 0302txt 0063
	//xof:�̶� 0302����ʾDirectX3.2�汾��ģ��
	//txt�� txt��ʽ��X�ļ���Ϊtxt��ʽ�Ͷ����Ƹ�ʽ���֣�
	//0064����ʾ�����͵�λ����64λ
	//template Header{
	//	<3D82AB43 - 62DA - 11cf - AB39 - 0020AF71E433>
	//	//��GUID(Global Unique Identifier,ȫ��Ψһ��ʶ��
	//	WORD major;         //X�ļ������汾
	//	WORD minor;			//X�ļ�����С�汾
	//	DWORD flags;		//���巽ʽ
	//}
	//template Mesh{
	//	<3D82AB43 - 62DA - 11cf - AB39 - 0020AF71E433>
	//	DWOED nVertices;                     //�������
	//	array Vector vertices[nVertices];	//��������
	//	DWOED nFaces;						//�����
	//	array MeshFace face[nFace];			//������
	//	[...]								//��ʾ��ģ���ǿ�����ģ�壬�����������������Ϣ
	//}
	/*template Vector{
		<3D82AB43 - 62DA - 11cf - AB39 - 0020AF71E433>
		float x; float y; float z;
	}*/
	//template MeshFace{
	//	<3D82AB43 - 62DA - 11cf - AB39 - 0020AF71E433>
	//	DWORD nfaceVertexIndices;                          //���������������������������㣬��������Ϊ3
	//	array DWORD faceVertexIndices[nfaceVerIndices];	   //���������б�
	//}
}