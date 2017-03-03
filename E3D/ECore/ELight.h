#pragma once

#include"ECommon.h"
#include"EVector.h"

namespace E3D {

	enum LIGHT_TYPE {
		LIGHT_AMBIENT = 0,  //������
		LIGHT_DIRECTION = 1,// ƽ�й�
		LIGHT_POINT = 2,    //��ƹ�
	};

	struct ELight {

		EInt       id;                   //�ƹ�ķ���ID,ȫ��Ϊ1
		EBool      lightOn;              //�ƹ��Ƿ���

		LIGHT_TYPE lightType;            //�ƹ�����

		EFloat     power;               //�ƹ�ǿ�ȣ�Ĭ�� 1.0f
		EFloat     shadowFactor;        //�ƹⱳ��ǿ�ȣ�Ĭ��0.05f
		EColor     ambient;             //�ƹ�Ļ���ɫ��������ʹ�ã�
		EColor     diffuse;             //�ƹ���������ɫ
		EColor     specular;            //�ƹ�߹���ɫ

		EVector4D  position;            //�ƹ���������λ��
		EVector4D  direction;           //��Դ���򣬵�ƹ���Ч


		EVector4D  transPosion;        //�ƹ�����������ϵλ��
		EVector4D  transDirection;     //���������ϵ��Դ���򣬵�ƹ���Ч

		EFloat     kc, kl, kq;       //˥������

		EFloat     spot_inner;         //�۹����׶��
		EFloat     spot_outer;         //�۹����׶��
		EFloat     pf;                 //�۹��ָ������

		ELight(EInt lightId, LIGHT_TYPE type);
	};


	//���ƹ���
#define MAX_LIGHTS 8
	//ȫ�ֵƹ����
	extern std::vector<ELight*>*GLights;
	typedef std::vector<ELight*>::iterator LightItr;


	//����һ���µĵƹ⣬�����ɹ����صƹ��ID(ȫ��Ψһ)������ʧ�ܷ���-1
	//����������Դ��Ϊ MAX_LIGHTS

	extern EInt CreateLight(LIGHT_TYPE lightType);
	//���һ���ƹ⣬����ƹⲻ���ڣ�����NULL
	extern ELight* GetLight(EInt id);
	extern EInt    GetLightSize();
	//���ȫ���ƹ�
	extern void DestoryAllLights();
}