#pragma once

#include"EObejct.h"
#include"EGameManager.h"
#include"ESceneManager.h"

namespace E3D {
	//�ӵ�����
	class ETank;
	class EBullet :public EObject {
	public :
		virtual ~EBullet();
		void setScale(const EVector3D &scale) { mMesh->setScale(scale); }
		//���ó�ʼ����λ��
		void setPosition(const EVector3D &pos);
		EVector3D getPosition()const { return mMesh->getPosition(); }
		//���ó�ʼ���䷽��
		void setDirection(const EVector3D &dir);
		//��y����ӵ�������ת
		void yaw(EFloat degree);
		void update();

		EBool isVisible()const { return mMesh->isVisible(); }
		void setVisible(EBool visible) { mMesh->setVisible(visible); }
	protected:
		friend class EGameManager;
		EBullet(ETank *ownwe, const EString &meshName, EGameManager *scene);
	protected:
		EGameManager *mGameMgr;
		EMesh	*mMesh;

		EVector3D mDirection;     //�����ƶ�����
		EFloat mSpeed;            //�ƶ��ٶ�

		ETank  *mOwner;

		static EInt mID;			//����ID
	};
}