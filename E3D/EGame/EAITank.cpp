#include"EAITank.h"
#include"ESceneManager.h"
#include"EGameManager.h"
#include"EPlayerController.h"

namespace E3D {
	EAITank::EAITank(const EString &name, const EString &meshName, EGameManager *gameMgr) :ETank(name, meshName, gameMgr) {
		mFireFrequency = 15;
		mSpeed*= 0.75f;
		setBulletType(BULLET_BALL);
		mTankType = TANK_AL;
	}
	
	void EAITank::onHited() {
		mCurrentLive += 26;
	}

	void EAITank::fire() {
		 ETank::fire();
	}
	void EAITank::updateAI() {
		//��ȡ��ǰAITank������ҵľ���
		EVector3D length = getPosition() - mGameMgr->getPlayerController()->getPosition();

		//��⵱ǰAI Tank����ҵľ���
		if (length.squaredLength() < 1000.0f) {
			EFloat degree = length.angleBetween(mFireDir);
			degree = degree *180.0f / PI - 180.0f;
			//�鿴�н�����
			if (length.crossProduct(mFireDir).y > 0)
				degree = -degree;
			if (Abs(degree) > 2.0f) {
				//������Ͳ�н�
				updateFireDir(degree);
				mHead->yaw(mHead->getRotateVec().y + degree);
			}
			fire();
		}
		else
			fireAimRight();
		//��ǰ�ƶ�
		if (!moveUp()) {
			//���ǰ�����ϰ�������ת
			if (RandomInt() % 2 == 0)
				turnRight();
			else
				turnLeft();
	
		}
		else {
			//��һ���ĸ��������ת
			EInt ran = RandomInt(0, 100);
			if (ran == 50)
				turnRight();
			else if (ran == 25)
				turnLeft();
		}
		
	}
	void EAITank::update() {
		//����ETank����
		ETank::update();
		//����AI����
		updateAI();
	}
}