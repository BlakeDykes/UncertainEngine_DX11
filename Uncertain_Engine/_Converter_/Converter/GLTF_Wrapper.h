#ifndef GLTF_WRAP_H
#define GLTF_WRAP_H

#include "tiny_gltf.h"
#include "VBOData.h"
#include "MathEngine.h"
#include "AnimData.h"

class TextureData;
class ModelTextureData;
class MeshData;
class ModelData;
class BoundingObjectData;
class ArmatureData;

using namespace tinygltf;
using namespace Uncertain;

struct GLB_Header
{
	uint32_t magic;
	uint32_t version;
	uint32_t length;
};

struct Chunk_Header
{
	uint32_t chunkLength;
	uint32_t chunkType;
	unsigned char chunkData[];
};

struct Model_Query
{
	char* poFileName;
	char* poOutputFileName;
	char* poPositionAttrib;
	char* poNormAttrib;
	char* poUVAttrib;
	char* poJointAttrib;
	char* poWeightAttrib;
	char* poIndexAttrib;
	unsigned char* pBinary;
	char* poRawBuffer;
	char* poJson;
	
	Model_Query(const char* _pFileName, 
		const char* outputFileName, 
		const char* positionAttrib = "POSITION", 
		const char* normAttrib = "NORMAL", 
		const char* uVAttrib = "TEXCOORD_0",
		const char* jointAttrib = "JOINTS_0",
		const char* weightAttrib = "WEIGHTS_0",
		const char* indexAttrib = "indices");

	Model_Query() = delete;
	Model_Query(const Model_Query&) = delete;
	Model_Query& operator=(const Model_Query&) = delete;
	~Model_Query();
};

struct fVect2
{
	fVect2() = default;
	~fVect2() = default;

	float u;
	float v;
};

struct fVect3
{
	fVect3() = default;
	~fVect3() = default;

	fVect3(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z)
	{
	}

	float x;
	float y;
	float z;
};

struct fVect4
{
	fVect4() = default;
	~fVect4() = default;

	fVect4(float _x, float _y, float _z, float _w)
		:x(_x), y(_y), z(_z), w(_w)
	{

	}

	float x;
	float y;
	float z;
	float w;
};

struct iuVect3
{
	iuVect3() = default;
	~iuVect3() = default;

	iuVect3(unsigned int _v0, unsigned int _v1, unsigned int _v2)
		: v0(_v0), v1(_v1), v2(_v2)
	{

	}

	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
};

struct isVect3
{
	isVect3() = default;
	~isVect3() = default;

	isVect3(unsigned short _x, unsigned short _y, unsigned short _z)
		:x(_x), y(_y), z(_z)
	{

	}
	unsigned short x;
	unsigned short y;
	unsigned short z;

};

struct isVect4
{
	isVect4() = default;
	~isVect4() = default;

	isVect4(unsigned short _x, unsigned short _y, unsigned short _z, unsigned short _w)
		:x(_x), y(_y), z(_z), w(_w)
	{

	}
	unsigned short x;
	unsigned short y;
	unsigned short z;
	unsigned short w;
};

struct fMat4
{
	float m0;
	float m1;
	float m2;
	float m3;
	float m4;
	float m5;
	float m6;
	float m7;
	float m8;
	float m9;
	float m10;
	float m11;
	float m12;
	float m13;
	float m14;
	float m15;
};

struct ReverseHierarchy
{

	ReverseHierarchy();
	ReverseHierarchy(unsigned int jointDepth, unsigned int jointCount);
	const ReverseHierarchy& operator=(const ReverseHierarchy& b);
	ReverseHierarchy(const ReverseHierarchy& b);
	~ReverseHierarchy();

	// Returns Joint index to be added to JointTable
	unsigned int Add(tinygltf::Skin* pSkin, unsigned int nodeIndex);
	void CopyHierarchy(unsigned int* pJointTableIndexInOut);

	unsigned int JointDepth;
	unsigned int* poParents;

private:
	unsigned int NextOpen;

};

class GLTF_Wrapper
{
public:

	static bool ConvertAndWriteGLTF(Model_Query& modelQ, bool getTexture, bool getArmature, int singleMeshIndex = -1);
	static bool ConvertAndWrite(Model_Query& modelQ, bool getTexture, bool getArmature, int singleMeshIndex = -1);
	static bool ConvertAndWriteAnimations(const char* const pModelFolder, const char* pFileNames[][2], unsigned int AnimationCount, AnimData::TYPE animType);
	static bool ConvertAndWriteAnimationsGLTF(const char* const pModelFolder, const char* pFileNames[][2], unsigned int AnimationCount, AnimData::TYPE animType);
	static bool Load(Model& modelOut, Model_Query& modelQ);
	static bool LoadGLTF(Model& modelOut, Model_Query& modelQ);
	static bool GetRawBuffer(char*& dataOut, uint32_t& sizeOut, const char* const pFile);
	static bool GetGLBHeader(GLB_Header& headerOut, const char* const data, const uint32_t dataSize);
	static bool GetRawJSON(char*& poJson, uint32_t& jsonSizeOut, const char* const data, const uint32_t dataSize);
	//static bool GetRawJSONGLTF(char*& poJson, uint32_t& jsonSizeOut, const char* const data, const uint32_t dataSize);
	static bool GetBinaryBuffPtr(unsigned char*& binPtrOut, uint32_t& binSizeOut, const char* const data, const uint32_t dataSize);

	static bool GetModelData(Model& gltfModel, ModelData& modelOut, Model_Query& modelQ, int singleMeshIndex = -1);
	static bool GetMeshData(Model& gltfModel, MeshData& meshOut, Model_Query& modelQ, unsigned int meshIndex = 0, unsigned int primitiveIndex = 0);
	static bool GetModelTextureData(Model& gltfModel, ModelTextureData& modelTextOut, unsigned char*& pBinaryBuffer);
	static bool GetTextureData(Model& gltfModel, TextureData& textOut, unsigned int materialIndex, unsigned char*& pBinaryBuffer);
	static bool GetArmatureData(Model& gltfModel, ArmatureData& armOut, unsigned int skinIndex = 0);

	static bool SetVBO(Model& gltfModel, unsigned int meshIndex, const char* pKey, VBOData& vboOut, unsigned char*& pBinaryBuffer,
		VBOData::VBO_TARGET target = VBOData::VBO_TARGET::UNINITIALIZED, VBOData::VBO_COMPONENT componentReq = VBOData::VBO_COMPONENT::UNINITIALIZED);
	static bool SetVBO(Model& gltfModel, const char* pKey, VBOData& vboOut, unsigned char* pBinaryBuffer, unsigned int byteLength, unsigned int count,
		VBOData::VBO_TARGET target = VBOData::VBO_TARGET::UNINITIALIZED, VBOData::VBO_COMPONENT componentReq = VBOData::VBO_COMPONENT::UNINITIALIZED);
	static bool SetVBO(Model& gltfModel, unsigned int accessorIndex, VBOData& vboOut, unsigned char*& pBinaryBuffer);
	static void GetJointDepth(Model& gltfModel, Node* root, unsigned int depth, unsigned int& maxDepthOut);
	static void GenerateJointHierarchy(Model& gltfModel, ArmatureData& armOut, tinygltf::Skin* pSkin, ReverseHierarchy& hierarchy, unsigned int nodeIndex);
	static bool SetBounding(const VBOData& vertVBO, BoundingObjectData*& boundingOut);
	static bool SetAnimation(Model& gltfModel, unsigned int animationIndex, AnimData& animOut, unsigned char*& pBinaryBuffer, AnimData::TYPE animType);

	static void Print(GLB_Header& header);
};

#endif