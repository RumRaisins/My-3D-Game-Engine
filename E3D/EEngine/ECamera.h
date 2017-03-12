#pragma once

#include"EMath.h"
#include"EFrustum.h"

namespace E3D {

	enum RenderMode {
		RENDER_WIRE,    //�߿���Ⱦ
		RENDER_SOLID,   //ʵ����Ⱦ
	};

	class ECamera {
	public:
		~ECamera();
		void update();

		//���ý��ü������
		void setZNear(EFloat znear);
		//����Զ�ü������
		void setZFar(EFloat zfar);

		//���������������
		void setPosition(const EVector3D &pos);
		//����������ι۵㣬��lockTargetΪtrueʱ�������۲��
		void setTarget(const EVector3D &target, EBool lockTargetv = false);
		void releaseTarget() { mLockTarget = false; }

		//������������ϵ�ƶ�
		void move(const EVector3D &mov);
		//�����������������ϵ�ƶ�
		void moveRelative(const EVector3D &mov);

		//��y����ת
		void yaw(EFloat yDegree);
		//��x����ת
		void pitch(EFloat pDegree);

		//������Ⱦģʽ���߿��ʵ��
		void setRenderMode(RenderMode mode) { mRenderMode = mode; }
		RenderMode getRenderMode() const { return mRenderMode; }

		EFrustum *getFrustumm() const { return mFrustum; }
	private:
		friend class ESceneManager;
		ECamera();
		EFrustum *mFrustum;

		EBool mLockTarget;
		EBool mNeedUpdate;

		RenderMode mRenderMode;
	};

}