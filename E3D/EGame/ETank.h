#pragma once

#include"EObejct.h"
#include"ESceneManager.h"
#include<list>

namespace E3D {
	//�ӵ�����
	enum BulletType {
		BULLET_BALL,	//�����ӵ�
		BULLET_ROCKET,	//�������ӵ�
	};
	enum TankType {
		TANK_AL,
		TANK_PLAYER,
	};
	//TANK�Ļ��࣬��Ϊ�����֣�һ�������Դ���һ�����ǳ����Դ�������ת
	//�����ǻ��ڳ����ƶ���
	class EBullet;
	class ECamera;
	class EGameManager;
	class ETank :public EObject {
		typedef std::list<EBullet*>::iterator BulletIter;
	public:
		virtual ~ETank();

		void move(const EVector3D &mov);
		//��ǰ�ƶ�Tank������Ϊǰ
		EBool moveUp();
		//����ƶ�Tank�����򷴷���
		EBool moveDown();

		//����תTank����
		void turnLeft();
		//����תTank����
		void turnRight();
		//��׼����ת
		void fireAimLeft();
		//��׼����ת
		void fireAimRight();

		//��õ�ǰTank����������
		EVector3D getPosition() const { return mBody->getPosition(); }
		void setPosition(const EVector3D &pos);

		//Tank��һ����ת������Ϊ�Ƕ�
		void yaw(EFloat degree);

		//�����ӵ�����
		void setBulletType(BulletType type) { mBulletType = type; }
		BulletType getBulletType()const { return mBulletType; }
		TankType getTankType() { return mTankType; }
		//�Ƿ�ɼ�
		EBool isVisible()const { return mBody->isVisible(); }
		void setVisible(EBool visible);

		//�����ĵ��Ƿ���Tank�İ�Χ����
		EBool intersect(const EVector3D &point);

		//b�����к��Զ�����
		virtual void onHited();
		//����
		virtual void fire();
		//����Tank
		virtual void update();
		//��ȡ�����Ӧ���ƶ�������
		EVector3D getCameraSrep()const { return mMoveVec; }


	protected:
		friend class EGameManager;
		friend class EPlayerController;
		ETank(const EString &name, const EString &meshName, EGameManager*mGameMgr);
		//������׼�������ӵ����䷽��
		void updateFireDir(EFloat degree);


	protected:
		EMesh *mHead;
		EMesh *mBody;

		EVector3D mMoveVec;      //�����Ӧ���ƶ�������

		EVector3D mFireDir;    //������
		EFloat mSpeed;     //�ƶ��ٶ�
		EFloat mFireFrequency;

		//ECamera      *mCamera;

		EInt mLastFire;      //����ʱ���¼
		EFloat  mColOffset;  //��ײƫ����

		ESceneManager   *mScene;
		EGameManager    *mGameMgr;

		BulletType   mBulletType;
		TankType     mTankType;

	};
}