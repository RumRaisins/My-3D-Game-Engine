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
				//只对局部变量进行变换
				for (EInt i = 0; i < obj->vertexNumber; i++) {
					GetVertex4DMulMatrix44(obj->localList[i], mat, obj->localList[i]);
				}
				obj = obj->nextObject;
			}

		}break;
		case TRANSFORM_TRANS: {
			while (obj) {
				//对变换后的坐标进行变换
				for (EInt i = 0; i < obj->vertexNumber; i++) {
					GetVertex4DMulMatrix44(obj->transformList[i], mat, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}

		}break;
		case TRANSFORM_LOCAL_TO_TRANS: {
			while (obj)
			{
				//将局部坐标变换后存储值变换后的坐标定点列表
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
		//备份灯光的数据
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
					//设置材质
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
					//设置材质
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
					//设置材质
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
					//设置定点缩放值，不在保留坐标设置
					GetVertex4DAddVector4D(obj->localList[i] * obj->scale, pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
		else if (TRANSFORM_TRANS == transformType) {
			while (obj) {
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					//设置定点缩放值，不在保留坐标设置
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
		//先把当前物体的坐标变成相机坐标
		EVector4D centerPos;
		GetVector4DMulMatrix44(object->worldPosition, camera->mWorldToCamera, centerPos);
		//首先把半径进行缩放
		EFloat scaleRadiusZ;
		EFloat scaleRadiusX;
		EFloat scaleRadiusY;

		if (cullType = CULL_SPHERE) {
			scaleRadiusX = object->maxRadius*object->scale.x;
			scaleRadiusY = object->maxRadius*object->scale.y;
			scaleRadiusZ = object->maxRadius*object->scale.z;
		}
		else if (cullType = CULL_BOX) {
			//各剔除偏移以模型做大顶点的各项为基准
			scaleRadiusX = object->maxBoundingBox.x*object->scale.x;
			scaleRadiusY = object->maxBoundingBox.y*object->scale.y;
			scaleRadiusZ = object->maxBoundingBox.z*object->scale.z;
		}
		else return true;
		//远近面剔除
		if ((centerPos.z + scaleRadiusZ) < camera->clip_z_near || (centerPos.z + scaleRadiusZ) > camera->clip_z_far)
			return true;
		//左右面剔除
		//这里的测试时基于在xz平面的投影来计算的
		EFloat x_test = 0.5f*camera->viewplane_widght*centerPos.z / camera->view_dist;
		if ((centerPos.x - scaleRadiusX) > x_test || (centerPos.x + scaleRadiusX) < -x_test)
			return true;
		//上下面剔除
		//基于zy平面
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
		//这里对灯光位置进行变换，将其变换到摄像机坐标系下
		EMatrix44 mat = camera->mWorldToCamera;
		//这里只保留转变变换，不进行平移变换，否则会出现灯光跟随相机移动效果
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
		//这里使用变换后的多边形顶点列表，因为背面消除只能在定点被变换为世界坐标之后才能进行
		//而在这里一般在摄像机坐标系进行对其背面消除
		//顶点按照 顺时针方向排序 u=p0->p1 v=p0->p2 n=uXv
		EVector4D u = ((poly->verList->at(index0) - poly->verList->at(index1)).toVector4D());
		EVector4D v = ((poly->verList->at(index0) - poly->verList->at(index2)).toVector4D());
		//多边形法线
		EVector4D n = u.crossProduct(v); //n=u叉乘v
		EVector4D view = camera->position - poly->verList->at(index0).toVector4D();

		//求得两个向量的点积，如果小于0，那么认为这个面背朝摄像机，不予显示。
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
		//这里使用变换后的多边形定点列表，因为背面消除只能在定点被变换为世界坐标之后才能进行
		//而在这里一般在相机坐标系内对去进行背面消除
		//顶点按顺时针方向排列 
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
			//物体没有被剔除
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
			//这里移除不可见的多边形
			//这里在RenderList之后的操作不需要判断当前多变性是否可见
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

		//这里使用了EInt类型而没使用EColor类型，原因是后面要进行颜色累加
		//使用unsigned char可能会导致溢出，从而导致显示不正常
		EInt sumColorR = 0, sumColorG = 0, sumColorB = 0;

		//顶点按顺时针方向排序，因此 u=p0->p1 v=p0->p2 n=uXv
		EVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		EVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();
		EVector4D n = u.crossProduct(v);

		//法线长度
		EFloat length = n.length();

		ELight *light = NULL;

		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr) {
			light = *itr;
			//灯光关闭则不处理
			if (!light->lightOn)
				continue;
			lightOn = true;
			//环境光
			if (light->lightType == LIGHT_AMBIENT) {
				//将每个分量与多边形颜色相乘，并除以256，确保结果在0~256之间
				sumColorR += light->ambient.r*baseColor.r >> 8;
				sumColorG += light->ambient.g*baseColor.g >> 8;
				sumColorB += light->ambient.b*baseColor.b >> 8;
			}
			//平行光
			else if (light->lightType == LIGHT_DIRECTION) {
				//无穷远的灯光，因此需要知道面发现和光源方向
				//这里使用灯光方向的逆向量作为与面发现夹角的向量
				//这里当夹角小于90度，其点积>0
				EFloat dot = n.dotProduct(-light->transDirection);

				//平行光光找模型
				//I(d)dir=IOdir*Cldir
				//散射光的计算公式
				//Itotal =Rsdiffuse*Idiffuse*（n*1）
				//将它们像个相乘即可
				if (dot > 0.0f) {
					EFloat dirLeng = light->transDirection.length();

					//接收光照的强度，多边形发现与光照方向的夹角越小，那么其接收强大却大
					//夹角越小，接受强度却小，多边形越暗
					EFloat temp = light->power*dot / (length*dirLeng * 256);
					sumColorR += temp*light->diffuse.r*baseColor.r;
					sumColorG += temp*light->diffuse.g*baseColor.g;
					sumColorB += temp*light->diffuse.b*baseColor.b;
				}
				else
				{
					//这里当多边形是背朝光源时，也进行了一些处理，只是把它的颜色跳的很暗
					//这样才比较协调
					sumColorR += baseColor.r*light->shadowFactor;
					sumColorG += baseColor.g*light->shadowFactor;
					sumColorB += baseColor.b*light->shadowFactor;
				}
			}
			else if (light->lightType == LIGHT_POINT) {
				//点光源光照模型
				// Iopoint*Clpoint
				//I(d)point=kc+kl*d+kq*d*d
				//其中d=|p-s|即点光源到多边形距离

				//多边形到点光源的方向向量
				EVector4D dir = light->transPosion - poly->transformList[0].toVector4D();
				EFloat dot = n.dotProduct(dir);
				if (dot > 0.0f)
				{
					//散射光的计算公式
					//Itotal =Rsdiffuse*Idiffuse*（n*1)
					//将他们两个相乘即可
					//点光源到多边形距离
					EFloat dist = dir.length();
					EFloat atten = light->kc + light->kl*dist + light->kq*dist*dist;
					EFloat temp = dot / (length*dist*atten * 256);

					sumColorR += light->diffuse.r*baseColor.r*temp;
					sumColorG += light->diffuse.g*baseColor.g*temp;
					sumColorB += light->diffuse.b*baseColor.b*temp;
				}
				else {
					//这里当多边形是背朝光源时，也进行处理，只是把他的颜色调的很暗，这样才协调
					sumColorR += baseColor.r*light->shadowFactor;
				}
			}
		}
		if (lightOn) {
			//这里再次进行检测
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
				//变换x、y轴，z轴不变
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
				//在这里，x、y应景被归一化为[-1,1]的范围内
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
	//判断此多边形是否在摄像机范围内
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
	//判断此多边形是否在摄像机范围内
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