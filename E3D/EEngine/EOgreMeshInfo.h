#pragma once


namespace E3D {
	//Ogre Mesh�ļ������Ϣ
	enum OGREMESHID {
		//�޷��Ŷ�����
		M_HEADER =0x1000,
		//�汾��Ϣ
		M_MESH=0x3000,
			//��������Ϣ
			M_SUBMESH=0x4000,
			//M_GEOMETRY�飨����useSharedVectice =falseʱ�Ż���֣�
			M_GEOMETRY =0x5000,//������Ǳ�Ƕ�����������������
				//��������
				M_GEOMETRY_VERTEX_DECLARATION =0x5100,
				M_GEOMETRY_VERTEX_ELEMENT =0X5110,  //�ظ�����
				M_GETMETRY_VERTEX_BUFFER =0x5200,   //�ظ�����
				M_GETMETRY_VERTEX_BUFFER_DATA =0x5210,//ԭ��������

			M_SUBMESH_OPERATION=0x4010,

		M_MESH_BOUNDS =0x9000,
		M_SUBMESH_NAME_TABLE =0xA000,
			//�ӿ������б�ÿ���ӿ�����������ַ���
			M_SUNMESH_NAME_TABLE_ELEMENT =0xA1000,
	};

	//��Ⱦ����ö��
	enum OperationType {
		//����б�
		OT_POINT_LIST =1,
		//�ߵ��б�һ����Ҳ������������
		OT_LINE_LIST =2,
		//��״������
		OT_LINE_STRIP = 3,
		//�������б�
		OT_TRIANGLE_LIST =4,
		//����������
		OT_TRIANGLE_STRIP =5,
		//��������
		OT_TRIANGLE_FAN =6,
	};
}