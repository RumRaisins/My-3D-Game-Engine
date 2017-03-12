#pragma once

#include"EObejct.h"
#include"ESceneManager.h"
#include<list>

namespace E3D {
	//子弹类型
	enum BulletType {
		BULLET_BALL,	//球星子弹
		BULLET_ROCKET,	//导弹型子弹
	};
	enum TankType {
		TANK_AL,
		TANK_PLAYER,
	};
	//TANK的基类，分为两部分，一部分是脑袋，一部分是车身，脑袋可以旋转
	//机身是基于车身移动的
	class EBullet;
	class ECamera;
	class EGameManager;
	class ETank :public EObject {
		typedef std::list<EBullet*>::iterator BulletIter;
	public:
		virtual ~ETank();

		void move(const EVector3D &mov);
		//向前移动Tank，朝向为前
		EBool moveUp();
		//向后移动Tank，朝向反方向
		EBool moveDown();

		//向左转Tank车体
		void turnLeft();
		//向右转Tank车体
		void turnRight();
		//瞄准器左转
		void fireAimLeft();
		//瞄准器右转
		void fireAimRight();

		//获得当前Tank的世界坐标
		EVector3D getPosition() const { return mBody->getPosition(); }
		void setPosition(const EVector3D &pos);

		//Tank绕一周旋转，参数为角度
		void yaw(EFloat degree);

		//设置子弹类型
		void setBulletType(BulletType type) { mBulletType = type; }
		BulletType getBulletType()const { return mBulletType; }
		TankType getTankType() { return mTankType; }
		//是否可见
		EBool isVisible()const { return mBody->isVisible(); }
		void setVisible(EBool visible);

		//给定的点是否在Tank的包围盒内
		EBool intersect(const EVector3D &point);

		//b被击中后自动调用
		virtual void onHited();
		//开火
		virtual void fire();
		//更新Tank
		virtual void update();
		//获取摄像机应该移动的向量
		EVector3D getCameraSrep()const { return mMoveVec; }


	protected:
		friend class EGameManager;
		friend class EPlayerController;
		ETank(const EString &name, const EString &meshName, EGameManager*mGameMgr);
		//更新瞄准器朝向，子弹发射方向
		void updateFireDir(EFloat degree);


	protected:
		EMesh *mHead;
		EMesh *mBody;

		EVector3D mMoveVec;      //摄像机应该移动的向量

		EVector3D mFireDir;    //开火方向
		EFloat mSpeed;     //移动速度
		EFloat mFireFrequency;

		//ECamera      *mCamera;

		EInt mLastFire;      //开火时间记录
		EFloat  mColOffset;  //碰撞偏移量

		ESceneManager   *mScene;
		EGameManager    *mGameMgr;

		BulletType   mBulletType;
		TankType     mTankType;

	};
}