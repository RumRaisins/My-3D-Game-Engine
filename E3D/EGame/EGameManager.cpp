#include"EGameManager.h"
#include"EBullet.h"
#include"ETank.h"
#include"EPlayerController.h"
#include"ESceneManager.h"
#include"EAITank.h"

namespace E3D {

	const EString Bullet_Ball = "BallBullet.mesh";
	const EString Bullet_Rocket = "RocketBullet.mesh";

	const EVector3D InitPosition(-20.0f, 1.0f, 15.0f);

	const EVector3D RandowPos[3] = { EVector3D(10,1.5f,10),EVector3D(45,1.5f,-5), EVector3D(-20,1.5f,45) };

	EGameManager::EGameManager(ESceneManager *scene) :mScene(scene), mBlockSize(0.0f), mX(0),mZ(0), mTerrainMesh(NULL), mCurrentEnemyNumber(0),
		mMaxEnemyNumber(0), mVisibleEnemyNumber(3) {
		mMainPlayer->setPosition(InitPosition);

		EBullet *bullet= new EBullet(NULL, "RocketBullet.mesh", this);
		bullet->setVisible(false);
		bullet = new EBullet(NULL, "BallBullet.mesh", this);
		bullet->setVisible(false);

	}

	EGameManager::~EGameManager() {
		SafeDelete(mMainPlayer);
		for (BulletIter itr = mBullets.begin(); itr != mBullets.end(); ++itr) {
			SafeDelete(*itr);
		}
		for (TankIter itr = mTanks.begin(); itr != mTanks.end(); ++itr) {
			SafeDelete(*itr);
		}
		mBullets.clear();
		mTanks.clear();
	}

	void EGameManager::startGame(EInt maxEnemyNumber) {
		//清空当前子弹和坦克
		for (BulletIter itr = mBullets.begin(); itr != mBullets.end(); ++itr) {
			(*itr)->mCurrentLive = (*itr)->mMaxLive++;
		}
		for (TankIter itr = mTanks.begin(); itr != mTanks.end(); ++itr) {
			(*itr)->mCurrentLive = (*itr)->mMaxLive++;
		}
		mMaxEnemyNumber = maxEnemyNumber;
		mCurrentEnemyNumber = 0;
	}
	EBool EGameManager::finishGame()const {
		return mCurrentEnemyNumber == mMaxEnemyNumber;
	}

	EBullet *EGameManager::createBullet(ETank *owner) {
		EString meshname = (owner->getBulletType() == BULLET_ROCKET ? Bullet_Rocket : Bullet_Ball);
		EBullet *bullet = new EBullet(owner, meshname, this);
		if (meshname == Bullet_Ball) {
			bullet->setScale(EVector3D(1.5f, 1.5f, 1.5f));
		}
		mBullets.push_back(bullet);
		return bullet;
	}
	ETank *EGameManager::createAITank(const EVector3D &pos) {
		static EInt ID = 0;
		EString name = "AI#" + IntToString(ID++);
		ETank *tank = new EAITank(name, "Tank1", this);
		tank->setPosition(pos);
		mTanks.push_back(tank);
		return tank;
	}

	void EGameManager::update() {
		if (isGameBegin() && !finishGame()) {
			//不足最大显示，创建
			if ((EInt)mTanks.size() < mVisibleEnemyNumber) {
				EInt curNum = (EInt)mTanks.size();
				for (EInt i = 0; i < mVisibleEnemyNumber; i++)
				{
					if (mCurrentEnemyNumber + (EInt)mTanks.size() < mMaxEnemyNumber)
						createAITank(RandowPos[RandomInt(0, 2)]);
				}
			}

			//更新坦克，如果坦克存活，那么就更新，否则删除
			for (TankIter i = mTanks.begin(); i != mTanks.end(); )
			{
				if ((*i)->isAlive())
				{
					(*i)->update();
					++i;
				}
				else {
					mCurrentEnemyNumber++;
					SafeDelete(*i);
					mTanks.erase(i++);
				}
			}

		}
		//更新子弹，如果子弹存活马么就更新，否则将其删除
		for (BulletIter i = mBullets.begin(); i != mBullets.end(); )
		{
			if ((*i)->isAlive())
			{
				(*i)->update();
				++i;
			}
			else {
				mCurrentEnemyNumber++;
				SafeDelete(*i);
				mBullets.erase(i++);
			}
		}
		mMainPlayer->update();
	}

	void EGameManager::changeMap(const EString& mapName) {
		//删除场景
		SafeDelete(mMainPlayer);
		for (BulletIter itr = mBullets.begin(); itr != mBullets.end(); ++itr) {
			SafeDelete(*itr);
		}
		for (TankIter itr = mTanks.begin(); itr != mTanks.end(); ++itr) {
			SafeDelete(*itr);
		}
		mBullets.clear();
		mTanks.clear();

		mCollisionValue.clear();
		mCollisionMeshs.clear();

		//敌人至于0
		mCurrentEnemyNumber = 0;

		//清理实际的模型
		mScene->clearMesh();

		//创建新角色
		mMainPlayer = new EPlayerController(this);
		mMainPlayer->setPosition(InitPosition);
		//加载场景
		loadMap(mapName);
	}

	EString getBlock(const EString &line, char ltip, char rtip) {
		EInt beg = line.find(ltip);
		EInt end = line.rfind(rtip);
		return line.substr(beg + 1, end - beg - 1);
	}
	EVector3D getPos(const EString &line) {
		//<Position X="-49.6499979"Y="4.2231"Z="-5.433"/>
		EInt xb = line.find("X");
		EInt yb = line.find("Y");
		EInt zb = line.find("Z");
		
		EInt end = line.find("/");
		EString xs = line.substr(xb + 3, yb - 5 - xb);
		EString ys = line.substr(yb + 3, zb - 5 - yb);
		EString zs = line.substr(zb + 3, end - zb - 5);
		return EVector3D(StringToFloat(xs), StringToFloat(ys), StringToFloat(zs));
	}

	void getXZ(const EString &line, EInt &x, EInt &z, EFloat &blockSize) {
		//<Grid X="15"Z="14"Size="10"/>
		EInt xb = line.find("X");
		EInt zb = line.find("Z");
		EInt sb = line.find("Size");
		EInt end = line.find("/");
		EString xs = line.substr(xb + 3, zb - 5 - xb);
		EString zs = line.substr(zb + 3, sb - 5 - zb);
		EString ss = line.substr(sb + 6, end - sb - 8);

		x = StringToFloat(xs);
		z = StringToFloat(zs);
		blockSize = StringToFloat(ss);
	}

	void getValue(const EString& line, EInt& x, EInt& z, EInt& value) {
		//<Map X="6"Z="2"Value="0"/>
		EInt xb = line.find("X");
		EInt zb = line.find("Z");
		EInt sb = line.find("Value");
		EInt end = line.find("/");
		EString xs = line.substr(xb + 3, zb - 5 - xb);
		EString zs = line.substr(zb + 3, sb - 5 - zb);
		EString ss = line.substr(sb + 9, end - sb - 9);

		x = StringToFloat(xs);
		z = StringToFloat(zs);
		value = StringToFloat(ss);
	}
	


}