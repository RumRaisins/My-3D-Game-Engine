#include"ECore.h"
#include"EFrustum.h"
#include"EGraphics.h"
#include"ELight.h"
#include"EMaterial.h"

namespace E3D {

	EVertex4D::EVertex4D() :x(0.0f), y(0.0f), z(0.0f), w(1.0f), u(1.0f), v(0.0f) {}
	EVertex4D::EVertex4D(const EVector4D &v) : x(v.x), y(v.y), z(v.z), w(v.w), u(0.0f), v(0.0f) {}
	EVertex4D::EVertex4D(EFloat xf, EFloat yf, EFloat zf) : x(xf), y(yf), z(zf), w(1.0f), u(0.0f), v(0.0f) {}

	EVertex4D EVertex4D::operator*(const EVector4D &right) {
		EVertex4D temp = *this;
		temp.x *= right.x, temp.y *= right.y, temp.z *= right.z;
		return temp;
	}
	EVertex4D EVertex4D::operator-(const EVertex4D & right)
	{
		EVertex4D temp = *this;
		temp.x -= right.x, temp.y -= right.y, temp.z -= right.z;
		return temp;
	}
	EVertex4D EVertex4D::operator+(const EVector4D &right) {
		EVertex4D temp = *this;
		temp.x += right.x, temp.y += right.y, temp.z += right.z;
		return temp;
	}
	EVertex4D EVertex4D::operator+(const EVertex4D &right) {
		EVertex4D temp = *this;
		temp.x *= right.x, temp.y *= right.y, temp.z *= right.z;
		return temp;
	}
	EVertex4D EVertex4D::operator/(EFloat right) {
		EVertex4D temp = *this;
		temp.x /= right, temp.y /= right, temp.z /= right;
		return temp;
	}
	EVertex4D &EVertex4D::operator=(const EVertex4D &right) {
		x = right.x, y = right.y, z = right.z, w = right.w;
		u = right.y, v = right.v;
		return *this;
	}
	EVector4D EVertex4D::toVector4D()const {
		return EVector4D(x, y, z, w);
	}

	EPlane3D::EPlane3D(const EVector4D &p, const EVector4D &nor) :point(p) {
		normal = nor;
		normal.normalize();
	}
	EPlane3D &EPlane3D::operator=(const EPlane3D &right) {
		point = right.point;
		normal = right.normal;
		return *this;
	}

	EPolyon4D::EPolyon4D() :state(0), attribute(0), color(0xffffff), verList(NULL) {}
	EPolyonF4D::EPolyonF4D() : state(0), attribute(0), color(0xffffff)/*,pre(NULL),next(NULL)*/ {}

	EObject4D::EObject4D() : nextObject(NULL), materialName(DEFAULT_NAME), scale(1, 1, 1), direction(EVector4D::UNIT_X), needCull(true) {}

	void Transform_Object4D(EObject4D* object, const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType/*,EBool tansformBasis*/) {
		EObject4D* obj = object;
		switch (transformType) {
		case TRANSFORM_LOCAL: {
			while (obj) {
				//ֻ�Ծֲ��������б任
				for (EInt i = 0; i < obj->vertexNumber; i++) {
					GetVertex4DMulMatrix44(obj->localList[i], mat, obj->localList[i]);
				}
				obj = obj->nextObject;
			}

		}break;
		case TRANSFORM_TRANS: {
			while (obj) {
				//�Ա任���������б任
				for (EInt i = 0; i < obj->vertexNumber; i++) {
					GetVertex4DMulMatrix44(obj->transformList[i], mat, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}

		}break;
		case TRANSFORM_LOCAL_TO_TRANS: {
			while (obj)
			{
				//���ֲ�����任��洢ֵ�任������궨���б�
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DMulMatrix44(obj->localList[i], mat, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}

		}break;
		}
	}
	void Transform_RenderList4D(ERenderList4D* renderList, const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType) {
		EPolyonF4D *poly = NULL;
		switch (transformType) {
		case TRANSFORM_LOCAL: {
			for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DMulMatrix44(poly->localList[0], mat, poly->localList[0]);
				GetVertex4DMulMatrix44(poly->localList[1], mat, poly->localList[1]);
				GetVertex4DMulMatrix44(poly->localList[2], mat, poly->localList[2]);
			}
		} break;
		case TRANSFORM_TRANS: {
			for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DMulMatrix44(poly->transformList[0], mat, poly->transformList[0]);
				GetVertex4DMulMatrix44(poly->transformList[1], mat, poly->transformList[1]);
				GetVertex4DMulMatrix44(poly->transformList[2], mat, poly->transformList[2]);
			}
		}break;
		case TRANSFORM_LOCAL_TO_TRANS: {
			for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DMulMatrix44(poly->localList[0], mat, poly->transformList[0]);
				GetVertex4DMulMatrix44(poly->localList[1], mat, poly->transformList[1]);
				GetVertex4DMulMatrix44(poly->localList[2], mat, poly->transformList[2]);
			}
		}break;
		}
	}
	void Inert_Object4D_To_RenderList4D(ERenderList4D *renderList, EObject4D *object, OBJ_TRANSFORM_TYPE transformType,
		bool transformScaleAndPosition) {
		if (!object)
			return;
		EVector4D scale(1, 1, 1);
		EVector4D pos(0, 0, 0);
		if (transformScaleAndPosition) {
			scale = object->scale;
			pos = object->worldPosition;
		}
		//���ݵƹ������
		ELight *light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			light->transPosion = light->position;
			light->transDirection = light->direction;
		}

		EObject4D *obj = object;
		EPolyon4D *base;
		EPolyonF4D poly;
		switch (transformType)
		{
		case TRANSFORM_LOCAL: {
			while (obj) {
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.localList[0] = obj->localList[base->verIndex[0]];
					poly.localList[1] = obj->localList[base->verIndex[1]];
					poly.localList[2] = obj->localList[base->verIndex[2]];
					//���ò���
					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}
				obj = obj->nextObject;
			}
		} break;
		case TRANSFORM_LOCAL_TO_TRANS: {
			while (obj) {
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.localList[0] = obj->localList[base->verIndex[0]] * scale + pos;
					poly.localList[1] = obj->localList[base->verIndex[1]] * scale + pos;
					poly.localList[2] = obj->localList[base->verIndex[2]] * scale + pos;
					//���ò���
					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}
				obj = obj->nextObject;
			}
		} break;
		case TRANSFORM_TRANS: {

			while (obj) {
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->transformList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->transformList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->transformList[base->verIndex[2]] * scale + pos;
					//���ò���
					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}
				obj = obj->nextObject;
			}
		} break;
		}
	}
	void Local_To_World_Object4D(EObject4D *object, OBJ_TRANSFORM_TYPE transformType) {
		if (!object)
			return;
		EObject4D *obj = object;
		EVector4D pos = obj->worldPosition;
		if (TRANSFORM_LOCAL_TO_TRANS == transformType) {
			while (obj) {
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					//���ö�������ֵ�����ڱ�����������
					GetVertex4DAddVector4D(obj->localList[i] * obj->scale, pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
		else if (TRANSFORM_TRANS == transformType) {
			while (obj) {
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					//���ö�������ֵ�����ڱ�����������
					GetVertex4DAddVector4D(obj->transformList[i] * obj->scale, pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
	}
	void Local_To_World_RenderList4D(ERenderList4D *renderList, const EVector4D &pos, OBJ_TRANSFORM_TYPE transformType) {
		EPolyonF4D *poly = NULL;
		switch (transformType) {
		case TRANSFORM_LOCAL: {
			for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DAddVector4D(poly->localList[0], pos, poly->localList[0]);
				GetVertex4DAddVector4D(poly->localList[1], pos, poly->localList[1]);
				GetVertex4DAddVector4D(poly->localList[2], pos, poly->localList[2]);
			}
		} break;
		case TRANSFORM_TRANS: {
			for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DAddVector4D(poly->transformList[0], pos, poly->transformList[0]);
				GetVertex4DAddVector4D(poly->transformList[1], pos, poly->transformList[1]);
				GetVertex4DAddVector4D(poly->transformList[2], pos, poly->transformList[2]);
			}
		}break;
		case TRANSFORM_LOCAL_TO_TRANS: {
			for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr)
			{
				poly = &(*itr);
				GetVertex4DAddVector4D(poly->localList[0], pos, poly->transformList[0]);
				GetVertex4DAddVector4D(poly->localList[1], pos, poly->transformList[0]);
				GetVertex4DAddVector4D(poly->localList[2], pos, poly->transformList[0]);
			}
		}break;
		}
	}

	bool Cull_Object4D(EObject4D *object, EFrustum *camera, CULL_TYPE cullType) {
		if (!object)
			return true;
		if (!object->needCull)
			return false;
		//�Ȱѵ�ǰ������������������
		EVector4D centerPos;
		GetVector4DMulMatrix44(object->worldPosition, camera->mWorldToCamera, centerPos);
		//���ȰѰ뾶��������
		EFloat scaleRadiusZ;
		EFloat scaleRadiusX;
		EFloat scaleRadiusY;

		if (cullType = CULL_SPHERE) {
			scaleRadiusX = object->maxRadius*object->scale.x;
			scaleRadiusY = object->maxRadius*object->scale.y;
			scaleRadiusZ = object->maxRadius*object->scale.z;
		}
		else if (cullType = CULL_BOX) {
			//���޳�ƫ����ģ�����󶥵�ĸ���Ϊ��׼
			scaleRadiusX = object->maxBoundingBox.x*object->scale.x;
			scaleRadiusY = object->maxBoundingBox.y*object->scale.y;
			scaleRadiusZ = object->maxBoundingBox.z*object->scale.z;
		}
		else return true;
		//Զ�����޳�
		if ((centerPos.z + scaleRadiusZ) < camera->clip_z_near || (centerPos.z + scaleRadiusZ) > camera->clip_z_far)
			return true;
		//�������޳�
		//����Ĳ���ʱ������xzƽ���ͶӰ�������
		EFloat x_test = 0.5f*camera->viewplane_widght*centerPos.z / camera->view_dist;
		if ((centerPos.x - scaleRadiusX) > x_test || (centerPos.x + scaleRadiusX) < -x_test)
			return true;
		//�������޳�
		//����zyƽ��
		EFloat y_test = 0.5f*camera->viewplane_height*centerPos.z / camera->view_dist;
		if ((centerPos.y - scaleRadiusY) > y_test || (centerPos.y + scaleRadiusY) < y_test)
			return true;

		return false;
	}
	void World_To_Camera_Object4D(EObject4D *object, EFrustum *camera) {
		EObject4D *obj = object;
		while (obj) {
			for (EInt i = 0; i < obj->vertexNumber; i++)
			{
				GetVertex4DMulMatrix44(object->transformList[i], camera->mWorldToCamera, obj->transformList[i]);
			}
			obj = obj->nextObject;
		}
	}
	void World_To_Camera_RenderList4D(ERenderList4D *renderList, EFrustum *camera) {
		//����Եƹ�λ�ý��б任������任�����������ϵ��
		EMatrix44 mat = camera->mWorldToCamera;
		//����ֻ����ת��任��������ƽ�Ʊ任���������ֵƹ��������ƶ�Ч��
		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0;

		ELight *light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr) {
			light = *itr;
			GetVector4DMulMatrix44(light->position, mat, light->transPosion);
			GetVector4DMulMatrix44(light->direction, mat, light->transDirection);
		}
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr) {
			poly = &(*itr);
			GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);
		}
	}
	void Sort_RenderList4D(ERenderList4D *renderList) {
		renderList->polyData.sort(CompEPolyonF4D());
	}
	bool Remove_Backface(EPolyon4D *poly, EFrustum *camera) {
     	EInt index0 = poly->verIndex[0]; EInt index1 = poly->verIndex[0]; EInt index2 = poly->verIndex[0];
		//����ʹ�ñ任��Ķ���ζ����б���Ϊ��������ֻ���ڶ��㱻�任Ϊ��������֮����ܽ���
		//��������һ�������������ϵ���ж��䱳������
		//���㰴�� ˳ʱ�뷽������ u=p0->p1 v=p0->p2 n=uXv
		EVector4D u = ((poly->verList->at(index0) - poly->verList->at(index1)).toVector4D());
		EVector4D v = ((poly->verList->at(index0) - poly->verList->at(index2)).toVector4D());
		//����η���
		EVector4D n = u.crossProduct(v); //n=u���v
		EVector4D view = camera->position - poly->verList->at(index0).toVector4D();

		//������������ĵ�������С��0����ô��Ϊ����汳���������������ʾ��
		EFloat dot = n.dotProduct(view);
		if (dot < 0.0f) {
			poly->state |= EPOLY_STATE_BACKFACE;
			return true;
		}
		return false;
	}
	bool Remove_Backface(EPolyonF4D *poly, EFrustum *camera) {
		if (!(poly->state&EPOLY_STATE_ACTIVE) || (poly->state&EPOLY_STATE_BACKFACE) || (poly->state&EPOLY_STATE_CLIPPED))
			return true;
		//����ʹ�ñ任��Ķ���ζ����б���Ϊ��������ֻ���ڶ��㱻�任Ϊ��������֮����ܽ���
		//��������һ�����������ϵ�ڶ�ȥ���б�������
		//���㰴˳ʱ�뷽������ 
		EVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		EVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();

		EVector4D n = u.crossProduct(v);
		EVector4D view = camera->position - poly->transformList[0].toVector4D();

		//TODO
		EFloat dot = n.dotProduct(view);
		if (dot < 0) {
			poly->state |= EPOLY_STATE_BACKFACE;
		}
		return false;
	}
	void Remove_Backface_Object4D(EObject4D *object, EFrustum *camera) {
		EObject4D *obj = object;
		EPolyon4D *poly = NULL;
		while (obj) {
			//����û�б��޳�
			if (!(obj->state&EOBJECT_STATE_CULLED)) {
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					poly = &obj->polyonList[i];
					Remove_Backface(poly, camera);
				}
			}
			obj = obj->nextObject;
		}
	}

	void Remove_Backface_RenderList4D(ERenderList4D *renderList, EFrustum *camera) {
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end();) {
			poly = &(*itr);
			//�����Ƴ����ɼ��Ķ����
			//������RenderList֮��Ĳ�������Ҫ�жϵ�ǰ������Ƿ�ɼ�
			if (Remove_Backface(poly, camera)) {
				renderList->polyData.erase(itr++);
			}
			else {
				++itr;
			}
		}
	}
	void Light_PolyonF4D(EPolyonF4D *poly, EFrustum *camera) {
		EBool lightOn = false;
		EColor baseColor(poly->color);

		//����ʹ����EInt���Ͷ�ûʹ��EColor���ͣ�ԭ���Ǻ���Ҫ������ɫ�ۼ�
		//ʹ��unsigned char���ܻᵼ��������Ӷ�������ʾ������
		EInt sumColorR = 0, sumColorG = 0, sumColorB = 0;

		//���㰴˳ʱ�뷽��������� u=p0->p1 v=p0->p2 n=uXv
		EVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		EVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();
		EVector4D n = u.crossProduct(v);

		//���߳���
		EFloat length = n.length();

		ELight *light = NULL;

		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr) {
			light = *itr;
			//�ƹ�ر��򲻴���
			if (!light->lightOn)
				continue;
			lightOn = true;
			//������
			if (light->lightType == LIGHT_AMBIENT) {
				//��ÿ��������������ɫ��ˣ�������256��ȷ�������0~256֮��
				sumColorR += light->ambient.r*baseColor.r >> 8;
				sumColorG += light->ambient.g*baseColor.g >> 8;
				sumColorB += light->ambient.b*baseColor.b >> 8;
			}
			//ƽ�й�
			else if (light->lightType == LIGHT_DIRECTION) {
				//����Զ�ĵƹ⣬�����Ҫ֪���淢�ֺ͹�Դ����
				//����ʹ�õƹⷽ�����������Ϊ���淢�ּнǵ�����
				//���ﵱ�н�С��90�ȣ�����>0
				EFloat dot = n.dotProduct(-light->transDirection);

				//ƽ�й����ģ��
				//I(d)dir=IOdir*Cldir
				//ɢ���ļ��㹫ʽ
				//Itotal =Rsdiffuse*Idiffuse*��n*1��
				//�����������˼���
				if (dot > 0.0f) {
					EFloat dirLeng = light->transDirection.length();

					//���չ��յ�ǿ�ȣ�����η�������շ���ļн�ԽС����ô�����ǿ��ȴ��
					//�н�ԽС������ǿ��ȴС�������Խ��
					EFloat temp = light->power*dot / (length*dirLeng * 256);
					sumColorR += temp*light->diffuse.r*baseColor.r;
					sumColorG += temp*light->diffuse.g*baseColor.g;
					sumColorB += temp*light->diffuse.b*baseColor.b;
				}
				else
				{
					//���ﵱ������Ǳ�����Դʱ��Ҳ������һЩ����ֻ�ǰ�������ɫ���ĺܰ�
					//�����űȽ�Э��
					sumColorR += baseColor.r*light->shadowFactor;
					sumColorG += baseColor.g*light->shadowFactor;
					sumColorB += baseColor.b*light->shadowFactor;
				}
			}
			else if (light->lightType == LIGHT_POINT) {
				//���Դ����ģ��
				// Iopoint*Clpoint
				//I(d)point=kc+kl*d+kq*d*d
				//����d=|p-s|�����Դ������ξ���

				//����ε����Դ�ķ�������
				EVector4D dir = light->transPosion - poly->transformList[0].toVector4D();
				EFloat dot = n.dotProduct(dir);
				if (dot > 0.0f)
				{
					//ɢ���ļ��㹫ʽ
					//Itotal =Rsdiffuse*Idiffuse*��n*1)
					//������������˼���
					//���Դ������ξ���
					EFloat dist = dir.length();
					EFloat atten = light->kc + light->kl*dist + light->kq*dist*dist;
					EFloat temp = dot / (length*dist*atten * 256);

					sumColorR += light->diffuse.r*baseColor.r*temp;
					sumColorG += light->diffuse.g*baseColor.g*temp;
					sumColorB += light->diffuse.b*baseColor.b*temp;
				}
				else {
					//���ﵱ������Ǳ�����Դʱ��Ҳ���д���ֻ�ǰ�������ɫ���ĺܰ���������Э��
					sumColorR += baseColor.r*light->shadowFactor;
				}
			}
		}
		if (lightOn) {
			//�����ٴν��м��
			if (sumColorR > 255)sumColorR = 255;
			if (sumColorG > 255)sumColorG = 255;
			if (sumColorB > 255)sumColorB = 255;

			poly->color = ECOLOR_16BIT(sumColorR, sumColorG, sumColorB);
		}
	}
	void Light_RenderList4D(ERenderList4D *renderList, EFrustum *camera) {
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr	 = renderList->polyData.begin(); itr !=renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			Light_PolyonF4D(poly, camera);
		}
	}
	void Camera_To_Perspective_Object4D(EObject4D *object, EFrustum *camera) {
		if (!object) return;
		EObject4D *obj = object;
		EFloat d = camera->view_dist;
		EFloat temp = 0;
		while (obj) {
			for (EInt i = 0; i < obj->vertexNumber; i++) {
				temp = d / obj->transformList[i].z;
				//�任x��y�ᣬz�᲻��
				obj->transformList[i].x = temp*obj->transformList[i].x;
				obj->transformList[i].y = camera->aspect_ratio*temp*obj->transformList[i].y;
			}
			obj = obj->nextObject;
		}
	}
	void Camera_To_Perspective_RenderList4D(ERenderList4D *renderList, EFrustum *camera) {
		EFloat d = camera->view_dist;
		EFloat temp;
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin(); itr != renderList->polyData.end(); ++itr) {
			poly = &(*itr);
			for (EInt i = 0; i < 3; i++) {
				temp = d / poly->transformList[i].z;
				poly->transformList[i].x = temp*poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio*temp*poly->transformList[i].y;
			}
		}
	}
	void Perspective_To_Screen_Object4D(EObject4D *object, EFrustum *camera) {
		if (!object) return;
		EFloat xOffset = 0.5f*(camera->viewport_widght - 1);
		EFloat yOffset = 0.5f*(camera->viewport_height - 1);

		EObject4D *obj = object;
		while (obj) {
			for (EInt i = 0; i < obj->vertexNumber; i++)
			{
				//�����x��yӦ������һ��Ϊ[-1,1]�ķ�Χ��
				obj->transformList[i].x = xOffset*(1 + obj->transformList[i].x);
				obj->transformList[i].y = yOffset*(1 - obj->transformList[i].y);
			}
			obj = obj->nextObject;
		}
	}
	void Perspective_To_Screen_RenderList4D(ERenderList4D *renderList4D, EFrustum *camera) {
		EFloat xOffset = 0.5f*(camera->viewport_widght - 1);
		EFloat yOffset = 0.5f*(camera->viewport_height - 1);
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr Itr = renderList4D->polyData.begin(); Itr != renderList4D->polyData.end(); ++Itr) {
			poly = &(*Itr);
			for (EInt i = 0; i < 3; i++) {
				poly->transformList[i].x = xOffset*(1 + poly->transformList[i].x);
				poly->transformList[i].y = yOffset*(1 - poly->transformList[i].y);
			}
		}
	}
	//�жϴ˶�����Ƿ����������Χ��
	EBool isClipped(EPolyonF4D *poly, EPlane3D &plane) {
		EInt flag = 0;
		for (EInt i = 0; i < 3; i++) {
			EFloat temp = plane.normal.dotProduct(poly->transformList[i].toVector4D() - plane.point);
			if (temp < 0)
				flag++;
		}
		if (flag == 3)
			return true;
		else
			return false;
	}
	EBool isClipped(EPolyonF4D *point, EFloat znear, EFloat zfar) {
		EInt num = 0;
		for (int i = 0;	 i < 3; i++)
		{
			if (point->transformList[i].z<znear || point->transformList[i].z>zfar)
				num++;
		}
		if (num == 3)
			return true;
		return false;
	}
	//�жϴ˶�����Ƿ����������Χ��
	EBool isClipped(EPolyonF4D *poly, EFrustum *camera) {
		EVertex4D v0, v1, v2;
		GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, v0);
		GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, v1);
		GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, v2);
		if (camera->clip_z_near > v0.z&&camera->clip_z_near > v1.z&&camera->clip_z_near > v2.z ||
			camera->clip_z_far < v0.z&&camera->clip_z_far < v1.z&&camera->clip_z_far < v2.z || 
			camera->clip_plane_L.normal.dotProduct(v0.toVector4D() - camera->clip_plane_L.point)<0 &&
			camera->clip_plane_L.normal.dotProduct(v1.toVector4D() - camera->clip_plane_L.point)<0 &&
			camera->clip_plane_L.normal.dotProduct(v2.toVector4D() - camera->clip_plane_L.point)<0 ||
			camera->clip_plane_R.normal.dotProduct(v0.toVector4D() - camera->clip_plane_R.point)<0 &&
			camera->clip_plane_R.normal.dotProduct(v1.toVector4D() - camera->clip_plane_R.point)<0 &&
			camera->clip_plane_R.normal.dotProduct(v2.toVector4D() - camera->clip_plane_R.point)<0 ||
			camera->clip_plane_T.normal.dotProduct(v0.toVector4D() - camera->clip_plane_T.point)<0 &&
			camera->clip_plane_T.normal.dotProduct(v1.toVector4D() - camera->clip_plane_T.point)<0 &&
			camera->clip_plane_T.normal.dotProduct(v2.toVector4D() - camera->clip_plane_T.point)<0 ||
			camera->clip_plane_B.normal.dotProduct(v0.toVector4D() - camera->clip_plane_B.point)<0 &&
			camera->clip_plane_B.normal.dotProduct(v1.toVector4D() - camera->clip_plane_B.point)<0 &&
			camera->clip_plane_B.normal.dotProduct(v2.toVector4D() - camera->clip_plane_B.point)<0	)
			return true;
		return false;

	}
	
	


}