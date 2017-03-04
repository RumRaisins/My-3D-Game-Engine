#pragma once
#include"ECommon.h"
#include<map>
#include"EGraphics.h"

namespace E3D {
	struct EMaterial {
		EString name;
		EColor ambient;      //������ɫ
		EColor diffuse;      //��������ɫ
		EColor specular;     //���淴��

		EBitmap *bitmap;
		EMaterial();
		~EMaterial();
		inline bool useTexture() { return bitmap&&bitmap->isValid(); }
		EColor gitPixel(EFloat u, EFloat v);
	};

	typedef std::map<EString, EMaterial*>::iterator MaterialIter;
	extern std::map<EString, EMaterial*>*GMaterials;

	extern EMaterial *GetMaterial(const EString &name);
	extern bool SetMaterial(const EString &name, EMaterial *material);
	extern bool DestoryMaterial(const EString &name);
	extern void DestoryALLMatreials();

	//�������ʽű�����ȡ�������ʺ���ͼ
	//���ʽű������ģ��������ͬ ��ogreHead.mesh ogreHead.material
	extern void ParseMareialScript(const EString &scriptName);
}