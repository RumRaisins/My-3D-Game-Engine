#pragma once
#include"ECamera.h"
#include"ECommon.h"
#include"EMesh.h"
#include"ELight.h"
#include"ECore.h"
#include<map>

namespace E3D {

	//������Դ������
	class ESceneManager {
	
		typedef std::map<EString, EMesh*>::iterator MeshIter;

	public:
		ESceneManager();
		~ESceneManager();
		
		//��������
		void drawString(const EString &str, EInt x, EInt y, const EColor &c = EColor(255, 255, 255));

		//����һ��ģ��
		EMesh *createMesh(const EString &name, const EString &meshName);
		//��ȡָ����ģ��ʵ��
		EMesh *getMesh(const EString &name);
		//����ָ����ģ��ʵ��
		void destoryMesh(const EString &name);

		void clearMesh();

		//ָ������/�رյƹ⣨ȫ�֣�
		void enableLishts(EBool enable);
		//��ǰ�Ƿ����ƹ⣨ȫ�֣�
		EBool isLishtsEnable()const { return mLishtEnable; }
		//����һյ�ƹ�
		ELight* createLight(LIGHT_TYPE lightType);
		//���һ���ƹ⣬����ƹⲻ���ڣ�����null
		ELight* getLight(EInt id);

		//��ȡ�����ָ��
		ECamera *getCamera() { return mCamera; }
		//��ȡ���У��ɼ������ɼ����������
		EInt getTotalObjectNumber() const { return mMeshs.size(); }
		//��ȡ��ǰ�ɼ��������
		EInt getVisibleObjetNumber()const {return mVisibleObjectNumber;	}
		//��ȡ��ǰ�ɼ�����θ���
		EInt getVisiblePolyonNumber()const { return mVisiblePolyonNumber; }
		//��ȡ���У��ɼ�/���ɼ�������θ���
		EInt getTotalPolyonNumber()const { return mTotalPolyonNumber; }

		//��������
		EMesh * createTerrain(const EString &rawFileName, const EString &heightMapFileName,
			EFloat uTitle = 1, EFloat vTitle = 1, EFloat blockSize = 5.0f);
		EMesh *getTerrain() { return mTerrainMesh; }

		//����

		void update();
	protected:
		std::map<EString, EMesh*>mMeshs;
		std::map<EString, EMesh*>mUsedMesh;

		ERenderList4D *mRenderList;

		EMesh    *mTerrainMesh;

		ECamera    *mCamera;
		EBool   mLishtEnable;

		EInt mVisibleObjectNumber;
		EInt mVisiblePolyonNumber;
		EInt mTotalPolyonNumber;

	};
}