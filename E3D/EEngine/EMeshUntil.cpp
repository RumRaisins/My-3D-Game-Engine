#include"EMeshUtil.h"
#include"EUtil.h"
#include"EMaterial.h"

namespace E3D {
	EString ReadString(std::ifstream &in, EInt size) {

		EString ss;
		if (size == -1){
			EChar c;
			while (in.read(&c, sizeof(EChar)) && c != '\n'&&c != '\r')
				ss += c;
		}
		else {
			EChar c[256];
			in.read(c, sizeof(EChar)*size);
			ss.append(c); //在字符串后追加字符串
		}
		return ss;
	}
	EUShort ReadChunk(std::ifstream &in) {
		EUShort id;
		in.read((EChar*)&id, sizeof(EUShort));

		EUInt length;
		in.read((EChar*)&length, sizeof(EInt));

		return id;
	}
	EBool ReadBool(std::ifstream &in) {
		EBool re = false;
		in.read((EChar*)&re, sizeof(EBool));
		return re;
	}
	EUInt ReadInt(std::ifstream &in) {
		EUInt re;
		in.read((char*)&re, sizeof(EUInt));
		return re;
	}
	EUShort ReadShort(std::ifstream &in) {
		EUShort re;
		in.read((EChar*)&re, sizeof(EUShort));
		return re;
	}
	EObject4D* LoadOgreMesh(const EString &meshName, const EVector4D &worldPos, const EVector4D &scale) {
		std::ifstream in;
		//模型文件，说先获取默认路径                      二进制方式打开        读入方式（文件向程序读入
		in.open(GetPath(meshName).c_str(), std::ios_base::binary | std::ios_base::in);
		if (in.bad()) {
			in.close();
			return NULL;
		}
		EString name = GetNameFromPath(meshName);
		//加载素材
		ParseMaterialScript(name + ".material");
		EObject4D *head = NULL;
		{
			//网个头信息
			EUShort HEADER = ReadShort(in);
			//网格版本号
			EString Version = ReadString(in);

			//网格标记
			EUShort MESH = ReadChunk(in);

			//蒙皮骨骼
			EBool SkeletallyAnimated = ReadBool(in);

			//子网格标记
			EUShort SUBMESH = ReadChunk(in);
			EObject4D* curObject = NULL;
			while (!in.eof() && (SUBMESH == M_SUBMESH)) {
				std::vector<EUInt>indexArray;
				std::vector<EFloat>vertexArray;

				if (head == NULL) {
					head = new EObject4D;
					curObject = head;
				}
				else {
					curObject->nextObject = new EObject4D;
					curObject = curObject->nextObject;
				}
				curObject->name = name;
				curObject->scale = scale;

				//材质信息
				EString Materia = ReadString(in);
				//读取多边形材质
				curObject->materialName = Materia;

				//顶点是否共享
				EBool UseShareVertices = ReadBool(in);

				//无符号顶点数量
				EUInt indexCount = ReadInt(in);
				//获取模型多边形个数
				curObject->polyonNumber = indexCount / 3;
				curObject->polyonList.reserve(indexCount / 3);//预分配n个元素的存储空间
				
				//索引是否是32位
				EBool indexes32Bit = ReadBool(in);
				if (indexes32Bit) {
					//存储面的顶点索引
					std::vector<EUInt>faceVertexIndices = ReadArray<EUInt>(in, indexCount);
					//读取模型顶点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());//将区间[first,last)的元素赋值到当前的vector容器中
				}
				else {
					std::vector<EUShort>faceVertexIndices = ReadArray<EUShort>(in,indexCount);
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}
				//网格几何信息
				EUShort GEOMETRY = ReadChunk(in);

				//顶点数量
				EUInt vertexCount = ReadInt(in);
				//模型顶点个数

				curObject->vertexNumber = vertexCount;
				curObject->localList.reserve(vertexCount);
				curObject->transformList.reserve(vertexCount);

				//网格几何顶点声明
				EUShort GEOMETRY_VERTEX_DECLARATION = ReadChunk(in);
				//网格集合顶点元素
				EUShort GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				while (!in.eof() && GEOMETRY_VERTEX_ELEMENT == M_GEOMETRY_VERTEX_ELEMENT)
				{
					//绑定源缓冲区
					EUShort source = ReadShort(in);
					//顶点类型
					EUShort type = ReadShort(in);
					//顶点语义
					EUShort semantic = ReadShort(in);
					//在缓存中的偏移值
					EUShort offset = ReadShort(in);
					//颜色和纹理坐标的索引
					EUShort index = ReadShort(in);

					GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				}
				//顶点缓存
				EUShort GEOMETRY_VERTEX_BUFFER = GEOMETRY_VERTEX_ELEMENT;
				//把索引绑定到改缓存中
				EUShort bindIndex = ReadShort(in);
				//每个顶点大小必须与索引的声明保持一直
				EUShort vertexSize = ReadShort(in);

				//集合顶点缓存数据
				EUShort GEOMETRY_VERTEX_BUFFER_DATA = ReadChunk(in);
				//每个顶点使用EFloat数量
				EInt vertexNum = vertexSize / sizeof(EFloat);
				//数据缓存大小
				EInt bufferSize = vertexCount*vertexNum;

				//读取顶点数据
				vertexArray = ReadArray<EFloat>(in, bufferSize);

				for (EInt i = 0; i < (EInt)vertexArray.size(); i += vertexNum) {
					EVertex4D vex;
					//顶点坐标
					vex.x = vertexArray[i];
					vex.y = vertexArray[i + 1];
					vex.z = vertexArray[i + 2];
					//顶点UV
					vex.u = vertexArray[i + 6];
					vex.v = vertexArray[i + 7];
					curObject->localList.push_back(vex);
					curObject->transformList.push_back(vex);
				}
				for (EInt i = 0; i < curObject->polyonNumber; i++)
				{
					EInt index = i * 3;
					EPolyon4D poly;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.attribute = EPOLY_ATTR_VERTEX_NORMAL | EPOLY_ATTR_VERTRX_POSITION | EPOLY_ATTR_VETTEX_UV;
					poly.verIndex[0] = indexArray[index];
					poly.verIndex[1] = indexArray[index + 1];
					poly.verIndex[2] = indexArray[index + 2];
					//这里讲变换后的顶点指针交给每个多边形，是为了防止变换之后定点索引依旧是最初的顶点坐标
					poly.verList = &curObject->transformList;
					curObject->polyonList.push_back(poly);
				}
				//子网格位置
				EUShort SUBMESH_OPERATION = ReadChunk(in);
				//操作类型
				EUShort operationType = ReadShort(in);
				SUBMESH = ReadChunk(in);
				//网格包围盒
				EUShort MESH_BOUNDS = SUBMESH;
				if (MESH_BOUNDS == M_MESH_BOUNDS) {
					//最小包围盒和最大包围盒及计算半径
					std::vector<EFloat>bounds = ReadArray<EFloat>(in, 7);
					head->minBoundingBox = EVector4D(bounds[0], bounds[1], bounds[2]);
					head->maxBoundingBox = EVector4D(bounds[3], bounds[4], bounds[5]);
					head->avgRadius = (head->maxBoundingBox - head->minBoundingBox).length() / 2;

					EFloat minR = head->minBoundingBox.length();
					EFloat maxR = head->maxBoundingBox.length();
					head->avgRadius = maxR > minR ? maxR : minR;

					//子网格名字列表
					EUShort SUBMESH_NAME_TABLE = ReadChunk(in);
					if (SUBMESH_NAME_TABLE == M_SUBMESH_NAME_TABLE) {
						//子网格名字列表
						EUShort M_SUNMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
						while (!in.eof() && M_SUNMESH_NAME_TABLE_ELEMENT == M_SUBMESH_NAME_TABLE) {
							//名字和索引
							EShort index = ReadShort(in);
							EString name = ReadString(in);
							M_SUNMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
						}
					}
					
					
				}
			}
		}
		in.close();
		return head;
	}
}
