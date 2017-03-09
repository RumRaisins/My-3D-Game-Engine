#pragma once
#include"ECommon.h"
#include"EVector.h"
#include"EMatrix.h"
#include"ECore.h"

namespace E3D {
		
	enum CAMERA_TYPE {
		CAMERA_MODEL_ELUA = 0x0001,        // ŷ�������ģ��
		CAMERA_MODEL_UVN = 0x0002,		  //UVN�����ģ��
	};
	struct EFrustum {
		EInt            state;         //�����״̬
		EInt            attribute;     //���������
		CAMERA_TYPE     camMode;

		EVector4D       position;      //����������������λ��
		EVector4D       direction;     //�����ע�ӵķ���--����

		//UVNģ��ʹ�ã����ڶ���UVN�����
		EVector4D       camUp;		   //UVN�������ģ�͵ĳ���������  �������Y��
		EVector4D       camRight;      //UVN�������ģ�͵ĳ���������  �������X��
		EVector4D       camLook;	   //UVN�������ģ�͵ĳ���ע�ӷ��� �������Z��
		EVector4D       camTarget;     //UVN�������ģ�͵�ע�ӵ�   Z���ָ��

		EFloat          view_dist;     //�ӵ㵽ͶӰ��ľ���

		EFloat          fov;           //ˮƽ����ʹ�ֱ�������Ұ

		EFloat          clip_z_near;   //���ü���
		EFloat          clip_z_far;   //Զ�ü���

		EPlane3D        clip_plane_R;  //�Ҳü���
		EPlane3D        clip_plane_L;  //��ü���
		EPlane3D        clip_plane_T;  //�ϲü���
		EPlane3D        clip_plane_B;  //�²ü���

		EFloat          viewplane_widght;//ͶӰ����
		EFloat          viewplane_height;//���ڹ�һ��ͶӰ��Ϊ2X2�������С����Ļ���ڴ�С��ͬ

		EFloat          viewport_widght; //��Ļ/�ӿڵĴ�С
		EFloat          viewport_height;
		EFloat          viewport_center_X;  //��Ļ��������
		EFloat          viewport_center_Y;

		EFloat          aspect_ratio;      //��Ļ�ĸ߿��

		EMatrix44       mWorldToCamera;         //�������굽�������任����
		EMatrix44       mCameraToPerspective;   //������굽ͶӰ����任����
		EMatrix44       mPerspectiveToScreen;   //͸�����굽��Ļ����任����

		EFrustum(CAMERA_TYPE mode, const EVector4D &pos, const EVector4D &dir, const EVector4D &target,
			EFloat nearZ, EFloat farZ, EFloat ffov, EFloat viewportWidth, EFloat viewportHeight);
	};

}