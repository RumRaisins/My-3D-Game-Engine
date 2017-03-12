#include "ESceneManager.h"
#include "EGraphics.h"
#include "EMaterial.h"
#include "EUtil.h"
#include"ECore.h"

namespace E3D {
	ESceneManager::ESceneManager() :mLishtEnable(true), mVisibleObjectNumber(0),
		mVisiblePolyonNumber(0), mTotalPolyonNumber(0), mTerrainMesh(NULL) {
		mCamera = new ECamera();
		mRenderList = new  ERenderList4D();
	}
	ESceneManager::~ESceneManager() {
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter) {
			SafeDelete(iter->second);
		}
		for (MeshIter iter = mUsedMesh.begin(); iter != mUsedMesh.end(); ++iter) {
			SafeDelete(iter->second);
		}
	}
	void ESceneManager::drawString(const EString &str, EInt x, EInt y, const EColor &c) {
		EGraphics::drawString(str, x, y);
	}

	//����һ��ģ��ʵ��
	//�������б� 1.�������ʹ�õ���Ϸ����2.���������Ϸ����
	//��������ʱ���ȴӴ�����ж�����б���Ѱ�ң�if not find���������ʹ�õ�����в��ң�if find clone&insert in���ж����б�
	//else new a new one and insert in���ж����б�&����ʹ�õ��б�
	EMesh* ESceneManager::createMesh(const EString &name, const EString &meshName) {
		MeshIter iter = mMeshs.find(name);
		if (iter == mMeshs.end()) {
			EMesh *mesh = NULL;
			MeshIter meshItr = mUsedMesh.find(meshName);
			if (meshItr != mUsedMesh.end()) {
				mesh = meshItr->second->clone();
				mesh->mName = name;
			}
			else {
				mesh = new EMesh(name, meshName);
				mUsedMesh.insert(std::make_pair(meshName, mesh->clone()));
			}
			mMeshs.insert(std::make_pair(name, mesh));
			return mesh;
		}
		return iter->second;
	}

	void ESceneManager::clearMesh() {
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
			SafeDelete(iter->second);
		mMeshs.clear();

	}
	//��ȡָ����ģ��ʵ��
	EMesh* ESceneManager::getMesh(const EString &name) {
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
			return iter->second;
		return NULL;
	}
	//����ָ����ģ��ʵ��
	void ESceneManager::destoryMesh(const EString &name) {
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
			SafeDelete(iter->second);
		mMeshs.erase(name);
	}
	//ָ������/�رյƹ⣨ȫ�֣�
	void ESceneManager::enableLishts(EBool enable) {
		mLishtEnable = enable;
		for (int i = 0; i < GetLightSize(); i++)
		{
			getLight(i)->lightOn = enable;
		}
	}
	//����һյ�ƹ�
	ELight *ESceneManager::createLight(LIGHT_TYPE lightType) {
		EInt light = CreateLight(lightType);
		return getLight(light);
	}
	//��ȡһյ�ƹ⣬����ƹⲻ���ڣ�����NULL
	ELight* ESceneManager::getLight(EInt id) {
		return GetLight(id);
	}
	//����
	void ESceneManager::update() {
		//camera->update();
		mVisibleObjectNumber = 0;
		mVisiblePolyonNumber = 0;
		mCamera->update();
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter) {
			mTotalPolyonNumber += iter->second->getPolyonNumber();
			//ֻ����Ұ�ڵ�������뵽��Ⱦ�б�
			if (iter->second->isVisible() && !Cull_Object4D(iter->second->getObject(), mCamera->getFrustumm(), CULL_BOX)) {
				mVisibleObjectNumber++;
				Inert_Object4D_To_RenderList4D(mRenderList, iter->second->getObject(), TRANSFORM_TRANS, true);

			}
		}

		//��Ⱦ�б����3D��ˮ�ߴ���
		//�ֲ�����任����������ϵ
		Local_To_World_RenderList4D(mRenderList, EVector4D::ZERO, TRANSFORM_TRANS);
		//��������ϵֱ�ӱ任����Ļ����ϵ
		//��������ϵ-����������-�����������ϵ�任-���ü�-�����ռ���-��͸�ӱ任-��ƽĿ�껻
		World_To_Screen_RenderList4D(mRenderList, mCamera->getFrustumm());

		//��ǰ�ɼ�����θ���
		mVisiblePolyonNumber = mRenderList->polyData.size();

		//ִ����Ⱦ
		if (mCamera->getRenderMode() == RENDER_WIRE)
			Draw_RenderList4D_Wire(mRenderList);
		else
			Draw_RenderList4D_Teture_Solid(mRenderList);
	}
	EMesh* ESceneManager::createTerrain(const EString &heightMapFileName, const EString &textureFileName, EFloat uTitle,
		EFloat vTitle, EFloat blockSize) {
		if (mTerrainMesh)
			SafeDelete(mTerrainMesh);
		Log("Load Terrain HeightMap:#%s Texture:...", heightMapFileName.c_str(), textureFileName.c_str());

		EBitmap *heightMap = new EBitmap(heightMapFileName);
		if (heightMap->isValid() || heightMap->getWidth() % 2 == 0 || heightMap->getHeight() % 2 == 0) {
			SafeDelete(heightMap);
			return NULL;
		}
		EMaterial *mat = new EMaterial();
		mat->bitmap = new EBitmap(textureFileName);
		SetMaterial("Terrain", mat);

		EInt row = heightMap->getHeight();
		EInt col = heightMap->getWidth();

		//Y =0.299R+0.587G+0.114B
		mTerrainMesh = new EMesh();
		EObject4D *obj = new EObject4D();
		mTerrainMesh->mObject = obj;

		obj->name = "Terrian";
		obj->materialName = "Terrian";
		obj->state = EOBJECT_STATE_ACTIVE | EOBJECT_STATE_VISIBLE;
		obj->scale = EVector4D(1, 1, 1);
		obj->worldPosition = EVector4D::ZERO;
		obj->vertexNumber = row*col;
		obj->polyonNumber = (row - 1)*(col - 1) * 2;

		mTerrainMesh->mVertexNumber = obj->vertexNumber;
		mTerrainMesh->mPolyonNumber = obj->polyonNumber;

		//�����б�
		obj->polyonList.reserve(obj->vertexNumber);
		obj->transformList.reserve(obj->vertexNumber);
		//������б�
		obj->polyonList.reserve(obj->polyonNumber);

		EFloat offsetX = (col - 1)*blockSize / 2;
		EFloat offsetZ = (row - 1)*blockSize / 2;

		EFloat minY = 0, maxY = 0;
		EFloat u = uTitle / (col - 1);
		EFloat v = vTitle / (row - 1);
		for (EInt r = 0; r < row; r++) {
			for (EInt c = 0; c < col; c++) {
				EVertex4D vex;
				//��������
				EColor color = heightMap->getPixel(c, r);
				vex.x = c*blockSize - offsetX;
				vex.y = 0.1f*(0.299f*color.r + 0.587f*color.g + 0.114f*color.b);
				vex.z = r*blockSize - offsetZ;
				if (vex.y < minY)
					minY = vex.y;
				if (vex.y > maxY)
					maxY = vex.y;
				//����UV
				vex.u = c*u;
				vex.v = r*v;
				obj->localList.push_back(vex);
				obj->transformList.push_back(vex);
				//���㶥������
				if (r < row - 1 && c < col - 1) {
					EPolyon4D poly;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.attribute = EPOLY_ATTR_VERTRX_POSITION | EPOLY_ATTR_VETTEX_UV;
					poly.verList = &obj->transformList;
					poly.verIndex[0] = r*col + c;
					poly.verIndex[1] = (r + 1)*col + c;
					poly.verIndex[2] = r*col + c + 1;
					obj->polyonList.push_back(poly);

					poly.verIndex[0] = r*col + c+1;
					poly.verIndex[1] = (r + 1)*col + c;
					poly.verIndex[2] = (r+1)*col + c + 1;
					obj->polyonList.push_back(poly);

				}

			}
			obj->maxBoundingBox = EVector4D(offsetX, maxY, offsetZ);
			obj->minBoundingBox = EVector4D(-offsetX, minY, -offsetZ);
			obj->maxRadius = std::sqrt(offsetX*offsetX + offsetZ*offsetZ);

			SafeDelete(heightMap);
			mMeshs.insert(std::make_pair("Terrain", mTerrainMesh));
			//���ò������޳�����
			mTerrainMesh->setCullFlag(false);
			Log("Terrain Load Succed!");
			return mTerrainMesh;
		}
	
	}

}