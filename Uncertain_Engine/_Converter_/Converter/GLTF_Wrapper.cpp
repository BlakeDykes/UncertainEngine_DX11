#include "GLTF_Wrapper.h"
#include "File.h"
#include "Converter_Wrapper.h"
#include "MeshData.h"
#include "ModelData.h"
#include "ModelTextureData.h"
#include "BoundingObjectData.h"
#include "AnimData.h"
#include "ArmatureData.h"
#include "Converter_AnimData.h"
#include "ProtoData_Wrapper.h"

using namespace Uncertain;

bool GLTF_Wrapper::ConvertAndWriteGLTF(Model_Query& modelQ, bool getTexture, bool getArmature, int singleMeshIndex)
{
	Model gltfModel;

	bool res = GLTF_Wrapper::LoadGLTF(gltfModel, modelQ);
	assert(res);

	// Write model mesh(es)
	ModelData model;
	GLTF_Wrapper::GetModelData(gltfModel, model, modelQ, singleMeshIndex);
	ProtoData_Wrapper::WriteToFile(modelQ.poOutputFileName, model);

	// Write armature
	if (getArmature)
	{
		ArmatureData arm;
		GLTF_Wrapper::GetArmatureData(gltfModel, arm);
		ProtoData_Wrapper::WriteToFile(modelQ.poOutputFileName, arm);
	}

	// Get texture(s)
	if (getTexture)
	{
		bool success = false;
		TextureData texture;

		if (singleMeshIndex >= 0)
		{
			success = GLTF_Wrapper::GetTextureData(gltfModel, texture, model.Meshes->MaterialIndex, modelQ.pBinary);
			assert(success);

			ProtoData_Wrapper::WriteToFile(modelQ.poOutputFileName, texture);
		}
		else
		{
			ModelTextureData modelTextures;
			memcpy_s(modelTextures.pName,
					 ModelTextureData::FILE_NAME_SIZE,
					 modelQ.poOutputFileName,
					 ModelTextureData::FILE_NAME_SIZE
			);

			success = GLTF_Wrapper::GetModelTextureData(gltfModel, modelTextures, modelQ.pBinary);
			assert(success);

			ProtoData_Wrapper::WriteToFile(modelQ.poOutputFileName, modelTextures);
		}
	}

	return res;
}

bool GLTF_Wrapper::ConvertAndWrite(Model_Query& modelQ, bool getTexture, bool getArmature, int singleMeshIndex)
{
	Model gltfModel;

	bool res = GLTF_Wrapper::Load(gltfModel, modelQ);
	assert(res);

	// Write model mesh(es)
	ModelData model;
	GLTF_Wrapper::GetModelData(gltfModel, model, modelQ, singleMeshIndex);
	ProtoData_Wrapper::WriteToFile(modelQ.poOutputFileName, model);


	// Write armature
	if (getArmature)
	{
		ArmatureData arm;
		GLTF_Wrapper::GetArmatureData(gltfModel, arm);
		ProtoData_Wrapper::WriteToFile(modelQ.poOutputFileName, arm);
	}
	

	// Get texture(s)
	if (getTexture)
	{
		bool success = false;

		if (singleMeshIndex >= 0 || gltfModel.materials.size() <= 1)
		{
			TextureData texture;

			success = GLTF_Wrapper::GetTextureData(gltfModel, texture, model.Meshes->MaterialIndex, modelQ.pBinary);
			assert(success);

			ProtoData_Wrapper::WriteToFile(modelQ.poOutputFileName, texture);
		}
		else
		{
			ModelTextureData modelTextures;

			success = GLTF_Wrapper::GetModelTextureData(gltfModel, modelTextures, modelQ.pBinary);
			assert(success);

			ProtoData_Wrapper::WriteToFile(modelQ.poOutputFileName, modelTextures);
		}
	}

	return res;
}

bool GLTF_Wrapper::ConvertAndWriteAnimations(const char* const pModelFolder, const char* pFileNames[][2], unsigned int animationCount, AnimData::TYPE animType)
{
	bool success = false;
	for (unsigned int i = 0; i < animationCount; i++)
	{
		AnimData animation;
		Model gltfModel;
		Model_Query q(pFileNames[i][0], pModelFolder);
		GLTF_Wrapper::Load(gltfModel, q);
		SetAnimation(gltfModel, 0, animation, q.pBinary, animType);
		success = ProtoData_Wrapper::WriteToFile(pModelFolder, animation, pFileNames[i][1]);

		if (!success)
		{
			return success;
		}
	}

	return success;
}

bool GLTF_Wrapper::ConvertAndWriteAnimationsGLTF(const char* const pModelFolder, const char* pFileNames[][2], unsigned int animationCount, AnimData::TYPE animType)
{
	bool success = false;
	for (unsigned int i = 0; i < animationCount; i++)
	{
		AnimData animation;
		Model gltfModel;
		Model_Query q(pFileNames[i][0], pModelFolder);
		GLTF_Wrapper::LoadGLTF(gltfModel, q);
		SetAnimation(gltfModel, 0, animation, q.pBinary, animType);
		success = ProtoData_Wrapper::WriteToFile(pModelFolder, animation, pFileNames[i][1]);

		if (!success)
		{
			return success;
		}
	}

	return success;
}

bool GLTF_Wrapper::Load(Model& modelOut, Model_Query& modelQ)
{
	TinyGLTF loader;
	std::string err;
	std::string warn;

	bool res = loader.LoadBinaryFromFile(&modelOut, &err, &warn, modelQ.poFileName);

	if (!warn.empty())
	{
		Trace::out("Warning: %s\n", warn.c_str());
		assert(false);
	}

	if (!err.empty())
	{
		Trace::out("Error: %s\n", err.c_str());
		assert(false);
	}

	if (!res)
	{
		Trace::out("Failed to Load File: %s\n", modelQ.poFileName);
		assert(false);
	}

	uint32_t buffSize(0);
	
	res = GLTF_Wrapper::GetRawBuffer(modelQ.poRawBuffer, buffSize, modelQ.poFileName);
	assert(res);

	//Get GLB Header
	GLB_Header glbHeader;
	res = GLTF_Wrapper::GetGLBHeader(glbHeader, modelQ.poRawBuffer, buffSize);
	assert(res);

	//Get JSON Chunk
	uint32_t jsonSize;
	res = GLTF_Wrapper::GetRawJSON(modelQ.poJson, jsonSize, modelQ.poRawBuffer, buffSize);
	assert(res);

	uint32_t binSize;
	res = GLTF_Wrapper::GetBinaryBuffPtr(modelQ.pBinary, binSize, modelQ.poRawBuffer, buffSize);
	assert(res);

	return res;
}

bool GLTF_Wrapper::LoadGLTF(Model& modelOut, Model_Query& modelQ)
{
	TinyGLTF loader;
	std::string err;
	std::string warn;

	bool res = loader.LoadASCIIFromFile(&modelOut, &err, &warn, modelQ.poFileName);

	if (!warn.empty())
	{
		Trace::out("Warning: %s\n", warn.c_str());
		assert(false);
	}

	if (!err.empty())
	{
		Trace::out("Error: %s\n", err.c_str());
		assert(false);
	}

	if (!res)
	{
		Trace::out("Failed to Load File: %s\n", modelQ.poFileName);
		assert(false);
	}

	// Find binary folder and append "buffer.bin" to string
	const char* bufferName = "/buffer.bin";
	unsigned int ending = strlen(modelQ.poFileName);
	while (ending != 0 && modelQ.poFileName[ending] != '/')
	{
		ending--;
	}
	assert(ending != 0);

	size_t bufferFilePathSize =  ending + strlen(bufferName) + 1;
	char* pTmp = new char[bufferFilePathSize]();

	strncpy_s(pTmp, bufferFilePathSize, modelQ.poFileName, ending);
	strcat_s(pTmp, bufferFilePathSize, bufferName);
	pTmp[bufferFilePathSize - 1] = 0x0;

	uint32_t buffSize(0);

	res = GLTF_Wrapper::GetRawBuffer(modelQ.poRawBuffer, buffSize, pTmp);
	assert(res);

	modelQ.pBinary = (unsigned char*)modelQ.poRawBuffer;

	delete[] pTmp;

	return res;
}


bool GLTF_Wrapper::GetRawBuffer(char*& dataOut, uint32_t& sizeOut, const char* const pFile)
{
	File::Handle fh;
	File::Error error;

	error = File::Open(fh, pFile, File::Mode::READ);
	assert(error == File::Error::SUCCESS);

	error = File::Seek(fh, File::Position::END, 0);
	assert(error == File::Error::SUCCESS);

	DWORD fileSize(0);
	error = File::Tell(fh, fileSize);
	assert(error == File::Error::SUCCESS);

	sizeOut = (uint32_t)fileSize;
	dataOut = new char[sizeOut]();
	assert(dataOut);

	error = File::Seek(fh, File::Position::BEGIN, 0);
	assert(error == File::Error::SUCCESS);

	error = File::Read(fh, dataOut, fileSize);
	assert(error == File::Error::SUCCESS);
	
	assert(dataOut);
	assert(sizeOut > 0);

	error = File::Close(fh);
	assert(error == File::Error::SUCCESS);

	return true;
}

bool GLTF_Wrapper::GetGLBHeader(GLB_Header& headerOut, const char* const data, const uint32_t dataSize)
{
	GLB_Header* pHeader = (GLB_Header*)&data[0];
	assert(pHeader);

	assert((char*)(pHeader + 1) < (data + dataSize));

	assert(pHeader->version == 2);
	assert(pHeader->magic == 0x46546C67);

	headerOut = *pHeader;

	return true;
}

bool GLTF_Wrapper::GetRawJSON(char*& poJson, uint32_t& jsonSizeOut, const char* const data, const uint32_t dataSize)
{
	GLB_Header* pHeader = (GLB_Header*)&data[0];
	assert(pHeader);

	assert((char*)(pHeader + 1) < (data + dataSize));
	assert(pHeader->version == 2);
	assert(pHeader->magic == 0x46546C67);

	Chunk_Header* pChunkHeader = (Chunk_Header*)(pHeader + 1);
	assert(pChunkHeader);
	assert(pChunkHeader->chunkType == 0x4E4F534A);
	assert(pChunkHeader->chunkLength > 0);
	assert(((char*)(pChunkHeader + 1) + pChunkHeader->chunkLength) < (data + dataSize));

	jsonSizeOut = pChunkHeader->chunkLength;
	poJson = new char[jsonSizeOut + 1]();
	assert(poJson);
	memset(poJson, 0x0, jsonSizeOut + 1);
	memcpy_s(poJson, jsonSizeOut + 1, pChunkHeader->chunkData, pChunkHeader->chunkLength);

	return true;
}

bool GLTF_Wrapper::GetBinaryBuffPtr(unsigned char*& binPtrOut, uint32_t& binSizeOut, const char* const data, const uint32_t dataSize)
{
	GLB_Header* pHeader = (GLB_Header*)&data[0];
	assert(pHeader);
	assert((char*)(pHeader + 1) < (data + dataSize));
	assert(pHeader->version == 2);
	assert(pHeader->magic == 0x46546C67);

	// JSON Chunk
	Chunk_Header* pChunkHeader = (Chunk_Header*)(pHeader + 1);
	assert(pChunkHeader);
	assert(pChunkHeader->chunkType == 0x4E4F534A);
	assert(pChunkHeader->chunkLength > 0);
	assert(((char*)(pChunkHeader + 1) + pChunkHeader->chunkLength) < (data + dataSize));

	
	// Binary Buffer Chunk
	pChunkHeader = (Chunk_Header*)((uint32_t)(pChunkHeader + 1) + pChunkHeader->chunkLength);
	
	if (pChunkHeader == nullptr)
	{
		Trace::out("Error: Failed accessing binary chunk\n");
		return false;
	}
	assert(pChunkHeader->chunkType == 0x004E4942);
	assert(pChunkHeader->chunkLength > 0);
	assert(pChunkHeader->chunkData != nullptr);

	binPtrOut = pChunkHeader->chunkData;
	binSizeOut = pChunkHeader->chunkLength;

	return true;
}

bool GLTF_Wrapper::GetModelData(Model& gltfModel, ModelData& modelOut, Model_Query& modelQ, int singleMeshIndex)
{
	memcpy_s(modelOut.pModelName,
		ModelData::MODEL_NAME_SIZE,
		modelQ.poOutputFileName,
		ModelData::MODEL_NAME_SIZE
	);

	if (modelOut.Meshes)
	{
		delete [] modelOut.Meshes;
		modelOut.Meshes = nullptr;
	}

	bool success = false;
	if (singleMeshIndex >= 0)
	{
		modelOut.MeshCount = 1;
		modelOut.Meshes = new MeshData[1]();
		
		success = GLTF_Wrapper::GetMeshData(gltfModel, *modelOut.Meshes, modelQ, (unsigned int)singleMeshIndex);
		assert(success);
	}
	else
	{
		modelOut.MeshCount = gltfModel.meshes.size();
		modelOut.Meshes = new MeshData[modelOut.MeshCount]();

		for (unsigned int i = 0; i < modelOut.MeshCount; i++)
		{
			success = GLTF_Wrapper::GetMeshData(gltfModel, modelOut.Meshes[i], modelQ, i);
		}
	}

	return success;
}

bool GLTF_Wrapper::GetMeshData(Model& gltfModel, MeshData& meshOut, Model_Query& modelQ, unsigned int meshIndex, unsigned int primitiveIndex)
{
	memcpy_s(meshOut.pMeshName,
		MeshData::FILE_NAME_SIZE,
		gltfModel.meshes[meshIndex].name.c_str(),
		gltfModel.meshes[meshIndex].name.size()
	);

	auto primitiveMap = gltfModel.meshes[meshIndex].primitives[primitiveIndex];

	GLTF_Wrapper::SetVBO(gltfModel, meshIndex, modelQ.poPositionAttrib, meshOut.VBOVert, modelQ.pBinary);

	GLTF_Wrapper::SetVBO(gltfModel, meshIndex, modelQ.poNormAttrib, meshOut.VBONorm, modelQ.pBinary);

	GLTF_Wrapper::SetVBO(gltfModel, meshIndex, modelQ.poUVAttrib, meshOut.VBOUV, modelQ.pBinary);

	GLTF_Wrapper::SetVBO(gltfModel, meshIndex, modelQ.poIndexAttrib, meshOut.VBOIndex, modelQ.pBinary, VBOData::VBO_TARGET::ELEMENT_ARRAY_BUFFER, VBOData::VBO_COMPONENT::UNSIGNED_INT);

	if (primitiveMap.attributes.find(modelQ.poWeightAttrib) != primitiveMap.attributes.end())
	{
		if (meshOut.poSkinData)
		{
			delete meshOut.poSkinData;
			meshOut.poSkinData = nullptr;
		}

		meshOut.poSkinData = new MeshData::SkinningData();

		GLTF_Wrapper::SetVBO(gltfModel, meshIndex, modelQ.poJointAttrib, meshOut.poSkinData->VBOJoints, modelQ.pBinary, VBOData::VBO_TARGET::ARRAY_BUFFER, VBOData::VBO_COMPONENT::UNSIGNED_INT);

		GLTF_Wrapper::SetVBO(gltfModel, meshIndex, modelQ.poWeightAttrib, meshOut.poSkinData->VBOWeights, modelQ.pBinary);
		
		unsigned int invBindAccessor = 0;
		if (gltfModel.skins[0].inverseBindMatrices != -1)
		{
			invBindAccessor = gltfModel.skins[0].inverseBindMatrices;
		}
		else
		{
			invBindAccessor = gltfModel.skins[0].joints[meshOut.poSkinData->VBOJoints.AttribIndex];
		}
		GLTF_Wrapper::SetVBO(gltfModel, invBindAccessor, meshOut.poSkinData->VBOInvBind, modelQ.pBinary);
	}

	GLTF_Wrapper::SetBounding(meshOut.VBOVert, meshOut.poBounding);

	meshOut.TriCount = meshOut.VBOIndex.Count / 3;
	meshOut.VertCount = meshOut.VBOVert.Count;
	meshOut.RenderMode = Converter_MeshData::GetMode(primitiveMap.mode);
	meshOut.MaterialIndex = primitiveMap.material;

	return true;
}

bool GLTF_Wrapper::GetModelTextureData(Model& gltfModel, ModelTextureData& modelTextOut, unsigned char*& pBinaryBuffer)
{

	modelTextOut.TextureCount = gltfModel.materials.size();
	modelTextOut.Textures = new TextureData[modelTextOut.TextureCount]();

	bool success = false;
	for (unsigned int i = 0; i < modelTextOut.TextureCount; i++)
	{
		success = GLTF_Wrapper::GetTextureData(gltfModel, modelTextOut.Textures[i], i, pBinaryBuffer);
		assert(success);
	}

	return success;
}

bool GLTF_Wrapper::GetTextureData(Model& gltfModel, TextureData& textOut, unsigned int materialIndex, unsigned char*& pBinaryBuffer)
{
	textOut.MaterialIndex = materialIndex;

	tinygltf::PbrMetallicRoughness* pMetalic = &gltfModel.materials[textOut.MaterialIndex].pbrMetallicRoughness;
	
	if (!pMetalic)
	{
		textOut.bBaseColorEnabled = false;
		textOut.bTextEnabled = false;
		return true;
	}

	// Get BaseColorFactor
	assert(pMetalic->baseColorFactor.size() == 4);
	textOut.BaseColor.Set((float)pMetalic->baseColorFactor[0], (float)pMetalic->baseColorFactor[1], (float)pMetalic->baseColorFactor[2], (float)pMetalic->baseColorFactor[3]);
	textOut.bBaseColorEnabled = true;

	textOut.TextureIndex = pMetalic->baseColorTexture.index;
	if (textOut.TextureIndex == -1 || textOut.TextureIndex >= gltfModel.images.size())
	{
		textOut.bTextEnabled = false;
		return true;
	}

	tinygltf::Image* pImage = &gltfModel.images[textOut.TextureIndex];

	unsigned int len = pImage->name.size();
	assert(len < TextureData::FILE_NAME_SIZE - strlen(TextureData::FILE_PREFIX));

	strcpy_s(textOut.pFileName, TextureData::FILE_NAME_SIZE, TextureData::FILE_PREFIX);


	if (len == 0)
	{
		len = pImage->uri.length();
		assert(len < TextureData::FILE_NAME_SIZE - strlen(TextureData::FILE_PREFIX));
		if (len > 0)
		{

			strcat_s(textOut.pFileName, TextureData::FILE_NAME_SIZE, pImage->uri.c_str());
		}
		else
		{
			const char* str = "Proto_Texture";
			strcat_s(textOut.pFileName, TextureData::FILE_NAME_SIZE, str);
		}

	}
	else
	{
		strcat_s(textOut.pFileName, TextureData::FILE_NAME_SIZE, pImage->name.c_str());
	}

	textOut.MagFilter = TextureData::TEXTURE_MAG_FILTER::DEFAULT;
	textOut.MinFilter = TextureData::TEXTURE_MIN_FILTER::DEFAULT;
	textOut.WrapS = TextureData::TEXTURE_WRAP::DEFAULT;
	textOut.WrapT = TextureData::TEXTURE_WRAP::DEFAULT;
	textOut.Width = (unsigned int)pImage->width;
	textOut.Height = (unsigned int)pImage->height;
	textOut.Component = (unsigned int)pImage->component;
	textOut.Bits = (unsigned int)pImage->bits;
	textOut.AsIs = (unsigned int)pImage->as_is;
	textOut.PixelType = Converter_TextureData::GetComponent(pImage->pixel_type);

	switch (textOut.Component)
	{
	case 4:
		textOut.EFormat = TextureData::TEXTURE_EFORMAT::EFORMAT_RGBA;
		break;
	case 3:
		textOut.EFormat = TextureData::TEXTURE_EFORMAT::EFORMAT_RGB;
		break;
	case 1:
		assert(false);
		break;
	default:        
		break;
	}

	if (textOut.poData)
	{
		delete[] textOut.poData;
		textOut.poData = nullptr;
	}

	if (pImage->as_is == false)
	{
		// as_is == already compressed
		textOut.DataSize = pImage->image.size();
		textOut.poData = new unsigned char[textOut.DataSize]();
		assert(textOut.poData);
		auto pStart = &pImage->image[0];
		memcpy_s(textOut.poData, textOut.DataSize, pStart, textOut.DataSize);
		textOut.bTextEnabled = true;
	}
	else
	{
		// export binary
		size_t bufferIndex = (size_t)pImage->bufferView;
		auto bufferView = gltfModel.bufferViews[bufferIndex];

		textOut.DataSize = bufferView.byteLength;

		textOut.poData = new unsigned char[textOut.DataSize]();
		assert(textOut.poData);
		memcpy_s(textOut.poData, textOut.DataSize, pBinaryBuffer + bufferView.byteOffset, bufferView.byteLength);
		textOut.bTextEnabled = true;
	}

	return true;
}

bool GLTF_Wrapper::GetArmatureData(Model& gltfModel, ArmatureData& armOut, unsigned int skinIndex)
{
	Skin* pSkin = &gltfModel.skins[skinIndex];

	armOut.JointCount = pSkin->joints.size();

	int root = pSkin->skeleton;

	if (root == -1)
	{
		root = pSkin->joints[0];
	}

	GetJointDepth(gltfModel, &gltfModel.nodes[root], 0, armOut.JointDepth);

	armOut.poJointTable = new unsigned int[armOut.JointDepth * armOut.JointCount]();

	ReverseHierarchy rootHierarchy(armOut.JointDepth, armOut.JointCount);

	GenerateJointHierarchy(gltfModel, armOut, pSkin, rootHierarchy, root);

	return true;
}


bool GLTF_Wrapper::SetVBO(Model& gltfModel, unsigned int meshIndex, const char* pKey, VBOData& vboOut, unsigned char*& pBinaryBuffer,
	VBOData::VBO_TARGET target, VBOData::VBO_COMPONENT componentReq)
{
	assert(pKey);
	unsigned int index;
	auto map = gltfModel.meshes[meshIndex].primitives[0].attributes;
	auto it = map.find(pKey);
	if (it != map.end())
	{
		index = (unsigned int)it->second;
	}
	else
	{
		Primitive* primitive = &gltfModel.meshes[meshIndex].primitives[0];
		index = (unsigned int)primitive->indices;
	}
	
	auto accessor = gltfModel.accessors[index];
	size_t bufferIndex = (size_t)accessor.bufferView;
	auto bufferView = gltfModel.bufferViews[bufferIndex];

	// if target parameter has been changed, assign it. if not - get target from buffer
	vboOut.TargetType = target != VBOData::VBO_TARGET::UNINITIALIZED ? target : Converter_VBOData::GetTarget(bufferView.target);
	vboOut.Count = accessor.count;
	vboOut.ComponentType = Converter_VBOData::GetComponent(accessor.componentType);

	vboOut.VboType = Converter_VBOData::GetType(accessor.type);
	vboOut.DataSize = bufferView.byteLength;
	vboOut.AttribIndex = index;

	if (vboOut.poData)
	{
		delete[] vboOut.poData;
		vboOut.poData = nullptr;
	}

	if (componentReq != VBOData::VBO_COMPONENT::UNINITIALIZED && vboOut.ComponentType != componentReq)
	{
		vboOut.ConvertData(componentReq, pBinaryBuffer + bufferView.byteOffset, Converter_VBOData::GetStride(vboOut.VboType));
	}
	else
	{
		vboOut.poData = new unsigned char[vboOut.DataSize]();
		assert(vboOut.poData);

		memcpy_s(vboOut.poData, vboOut.DataSize, pBinaryBuffer + bufferView.byteOffset, bufferView.byteLength);
	}

	vboOut.bIsEnabled = true;

	return true;
}

bool GLTF_Wrapper::SetVBO(Model& gltfModel, const char* pKey, VBOData& vboOut, unsigned char* pBinaryBuffer,
	unsigned int byteLength, unsigned int count, VBOData::VBO_TARGET target, VBOData::VBO_COMPONENT componentReq)
{
	assert(pKey);
	unsigned int index;
	auto map = gltfModel.meshes[0].primitives[0].attributes;
	auto it = map.find(pKey);
	if (it != map.end())
	{
		index = (unsigned int)it->second;
	}
	else
	{
		Primitive* primitive = &gltfModel.meshes[0].primitives[0];
		index = (unsigned int)primitive->indices;
	}

	auto accessor = gltfModel.accessors[index];
	size_t bufferIndex = (size_t)accessor.bufferView;
	auto bufferView = gltfModel.bufferViews[bufferIndex];

	// if target parameter has been changed, assign it. if not - get target from buffer
	vboOut.TargetType = target != VBOData::VBO_TARGET::UNINITIALIZED ? target : Converter_VBOData::GetTarget(bufferView.target);
	vboOut.ComponentType = Converter_VBOData::GetComponent(accessor.componentType);
	vboOut.VboType = Converter_VBOData::GetType(accessor.type);
	vboOut.AttribIndex = index;

	vboOut.Count = count;
	vboOut.DataSize = byteLength;

	if (vboOut.poData)
	{
		delete[] vboOut.poData;
		vboOut.poData = nullptr;
	}

	if (componentReq != VBOData::VBO_COMPONENT::UNINITIALIZED && vboOut.ComponentType != componentReq)
	{
		vboOut.ConvertData(componentReq, pBinaryBuffer + bufferView.byteOffset, Converter_VBOData::GetStride(vboOut.VboType));
	}
	else
	{
		vboOut.poData = new unsigned char[vboOut.DataSize]();
		assert(vboOut.poData);

		memcpy_s(vboOut.poData, vboOut.DataSize, pBinaryBuffer + bufferView.byteOffset, bufferView.byteLength);
	}

	vboOut.bIsEnabled = true;

	return true;
}
bool GLTF_Wrapper::SetVBO(Model& gltfModel, unsigned int accessorIndex, VBOData& vboOut, unsigned char*& pBinaryBuffer)
{
	auto accessor = gltfModel.accessors[accessorIndex];
	size_t bufferIndex = (size_t)accessor.bufferView;
	auto bufferView = gltfModel.bufferViews[bufferIndex];

	// if target parameter has been changed, assign it. if not - get target from buffer
	vboOut.TargetType = Converter_VBOData::GetTarget(bufferView.target);
	vboOut.Count = accessor.count;
	vboOut.ComponentType = Converter_VBOData::GetComponent(accessor.componentType);
	vboOut.VboType = Converter_VBOData::GetType(accessor.type);
	vboOut.DataSize = bufferView.byteLength;
	vboOut.AttribIndex = accessorIndex;

	if (vboOut.poData)
	{
		delete[] vboOut.poData;
		vboOut.poData = nullptr;
	}

	if (vboOut.ComponentType == VBOData::VBO_COMPONENT::UNSIGNED_SHORT)
	{
		vboOut.ConvertData_UShorttoUInt(pBinaryBuffer + bufferView.byteOffset, Converter_VBOData::GetStride(vboOut.VboType));
	}
	else
	{
		vboOut.poData = new unsigned char[vboOut.DataSize]();
		assert(vboOut.poData);
		memcpy_s(vboOut.poData, vboOut.DataSize, pBinaryBuffer + bufferView.byteOffset, bufferView.byteLength);
	}

	return true;
}


void GLTF_Wrapper::GenerateJointHierarchy(Model& gltfModel, ArmatureData& armOut, tinygltf::Skin* pSkin, ReverseHierarchy& hierarchy, unsigned int nodeIndex)
{
	unsigned int currentIndex = hierarchy.Add(pSkin, nodeIndex);
	if (currentIndex != UINT_MAX)
	{
		unsigned int* pTableIndexToWrite = armOut.poJointTable + (currentIndex * hierarchy.JointDepth);
		hierarchy.CopyHierarchy(pTableIndexToWrite);

		// Recurse through children
		for (unsigned int i = 0; i < gltfModel.nodes[nodeIndex].children.size(); i++)
		{
			ReverseHierarchy tempRoot = hierarchy;

			GenerateJointHierarchy(gltfModel, armOut, pSkin, tempRoot, gltfModel.nodes[nodeIndex].children[i]);

		}
	}
}


void GLTF_Wrapper::GetJointDepth(Model& gltfModel, Node* root, unsigned int depth, unsigned int& maxDepthOut)
{
	unsigned int newDepth = depth + 1;

	if (root->children.empty())
	{
		if (maxDepthOut < newDepth)
		{
			maxDepthOut = newDepth;
		}

		return;
	}

	for (unsigned int i = 0; i < root->children.size(); i++)
	{
		GetJointDepth(gltfModel, &gltfModel.nodes[root->children[i]], newDepth, maxDepthOut);
	}
}

bool GLTF_Wrapper::SetBounding(const VBOData& vertVBO, BoundingObjectData*& boundingOut)
{

	Vec3 *vertBuff = new Vec3[vertVBO.Count]();
	float* pVert = (float*)vertVBO.poData;

	for (unsigned int i = 0; i < vertVBO.Count; i++)
	{
		vertBuff[i].set(*pVert, *(pVert+1), *(pVert+2));
		pVert += 3;
	}
	
	boundingOut = new BoundingObjectData(vertBuff, vertVBO.Count);

	delete[] vertBuff;

	return true;
}

bool GLTF_Wrapper::SetAnimation(Model& gltfModel, unsigned int animationIndex, AnimData& animOut, unsigned char*& pBinaryBuffer, AnimData::TYPE animType)
{
	tinygltf::Animation anim = gltfModel.animations[animationIndex];

	if (anim.channels.size() <= 0)
	{
		return false;
	}

	char* nameBuffer = new char[AnimData::ANIM_NAME_SIZE]();

	strcat_s(nameBuffer, AnimData::ANIM_NAME_SIZE, gltfModel.skins[0].name.c_str());
	strcat_s(nameBuffer, AnimData::ANIM_NAME_SIZE, "_");
	strcat_s(nameBuffer, AnimData::ANIM_NAME_SIZE, anim.name.c_str());

	memcpy_s(animOut.Name,
		AnimData::ANIM_NAME_SIZE,
		nameBuffer,
		strlen(nameBuffer)
	);

	animOut.AnimType = animType;

	delete[] nameBuffer;

	// Get time data buffer
	auto timeAccessor = gltfModel.accessors[anim.samplers[0].input];
	size_t timeIndex = (size_t)timeAccessor.bufferView;
	auto timeBufferView = gltfModel.bufferViews[timeIndex];
	float* pTime = nullptr;
	bool timeCorrected = false;

	// Correct time if not float values
	if (timeAccessor.componentType != 5126)
	{
		// Unsigned Short
		if (timeAccessor.componentType == 5123)
		{
			pTime = new float[animOut.FrameCount]();
			float* pTemp = pTime;
			unsigned short* pShort = (unsigned short*)pBinaryBuffer + timeBufferView.byteOffset;

			for (unsigned int i = 0; i < animOut.FrameCount; i++)
			{
				*pTemp++ = *pShort++;
			}

			timeCorrected = true;
		}

	}
	else
	{
		pTime = (float*)(pBinaryBuffer + timeBufferView.byteOffset);
	}

	animOut.JointCount = gltfModel.skins[0].joints.size();
	animOut.FrameCount = timeAccessor.count;
	animOut.poFrames = new AnimData::Frame[animOut.FrameCount]();

	std::vector<tinygltf::Node> nodes = gltfModel.nodes;
	std::vector<int> joints = gltfModel.skins[0].joints;
	

	// Frames
	// 
	// Set time and initialize frame values to joint start
	// Just the joints for rigid animations
	for (unsigned int i = 0; i < animOut.FrameCount; i++)
	{
		animOut.poFrames[i].poBones = new AnimData::Bone[animOut.JointCount]();
		animOut.poFrames[i].Time = pTime[i];

		for (unsigned int j = 0; j < animOut.JointCount; j++)
		{
			if (nodes[joints[j]].translation.size() == 0)
			{
				animOut.poFrames[i].poBones[j].poTrans.x = 1.0f;
				animOut.poFrames[i].poBones[j].poTrans.y = 1.0f;
				animOut.poFrames[i].poBones[j].poTrans.z = 1.0f;

			}
			else if (nodes[joints[j]].translation.size() == 3)
			{
				animOut.poFrames[i].poBones[j].poTrans.x = (float)nodes[joints[j]].translation[0];
				animOut.poFrames[i].poBones[j].poTrans.y = (float)nodes[joints[j]].translation[1];
				animOut.poFrames[i].poBones[j].poTrans.z = (float)nodes[joints[j]].translation[2];

			}
			else
			{
				assert(false);
			}

			if (nodes[joints[j]].rotation.size() == 0)
			{
				animOut.poFrames[i].poBones[j].poRot.x = 1.0f;
				animOut.poFrames[i].poBones[j].poRot.y = 1.0f;
				animOut.poFrames[i].poBones[j].poRot.z = 1.0f;

			}
			else if (nodes[joints[j]].rotation.size() == 4)
			{
				animOut.poFrames[i].poBones[j].poRot.x = (float)nodes[joints[j]].rotation[0];
				animOut.poFrames[i].poBones[j].poRot.y = (float)nodes[joints[j]].rotation[1];
				animOut.poFrames[i].poBones[j].poRot.z = (float)nodes[joints[j]].rotation[2];
				animOut.poFrames[i].poBones[j].poRot.w = (float)nodes[joints[j]].rotation[3];

			}
			else
			{
				assert(false);
			}


			if (nodes[joints[j]].scale.size() == 0)
			{
				animOut.poFrames[i].poBones[j].poScale.x = 1.0f;
				animOut.poFrames[i].poBones[j].poScale.y = 1.0f;
				animOut.poFrames[i].poBones[j].poScale.z = 1.0f;

			}
			else if(nodes[joints[j]].scale.size() >= 3)

			{
				animOut.poFrames[i].poBones[j].poScale.x = (float)nodes[joints[j]].scale[0];
				animOut.poFrames[i].poBones[j].poScale.y = (float)nodes[joints[j]].scale[1];
				animOut.poFrames[i].poBones[j].poScale.z = (float)nodes[joints[j]].scale[2];
			}
			else
			{
				assert(false);
			}

		}
	}

	if (timeCorrected)
	{
		delete[] pTime;
	}

	// Update frames with sampler data
	size_t channelCount = anim.channels.size();
	for (unsigned int i = 0; i < channelCount; i++)
	{
		auto it = std::find(joints.begin(), joints.end(), anim.channels[i].target_node);
		if (it == joints.end())
		{
			continue;
		}

		unsigned int boneIndex = it - joints.begin();

		AnimData::PATH path = Converter_AnimData::GetPath(anim.channels[i].target_path);
		auto sampler = anim.samplers[anim.channels[i].sampler];
		
		auto samplerAccessor = gltfModel.accessors[sampler.output];
		assert(samplerAccessor.componentType == 5126);	// <-- ensuring float values

		size_t bufferIndex = (size_t)samplerAccessor.bufferView;
		auto bufferView = gltfModel.bufferViews[bufferIndex];
		float* pData = (float*)(pBinaryBuffer + bufferView.byteOffset);

		switch (path)
		{
		case AnimData::PATH::SCALE:
		{
			for (unsigned int j = 0; j < animOut.FrameCount; j++)
			{
				animOut.poFrames[j].poBones[boneIndex].poScale.x = *pData;
				animOut.poFrames[j].poBones[boneIndex].poScale.y = *(pData + 1);
				animOut.poFrames[j].poBones[boneIndex].poScale.z = *(pData + 2);

				pData += 3;
			}
			break;
		}
		case AnimData::PATH::ROTATION:
		{
			for (unsigned int j = 0; j < animOut.FrameCount; j++)
			{
				animOut.poFrames[j].poBones[boneIndex].poRot.x = *pData;
				animOut.poFrames[j].poBones[boneIndex].poRot.y = *(pData + 1);
				animOut.poFrames[j].poBones[boneIndex].poRot.z = *(pData + 2);
				animOut.poFrames[j].poBones[boneIndex].poRot.w = *(pData + 3);
				
				pData += 4;
			}

			break;
		}
		case AnimData::PATH::TRANSLATION:
		{
			for (unsigned int j = 0; j < animOut.FrameCount; j++)
			{
				animOut.poFrames[j].poBones[boneIndex].poTrans.x = *pData;
				animOut.poFrames[j].poBones[boneIndex].poTrans.y = *(pData + 1);
				animOut.poFrames[j].poBones[boneIndex].poTrans.z = *(pData + 2);

				pData += 3;
			}

			break;
		}

		default:
			break;
		}
	}

	if (animOut.AnimType == AnimData::TYPE::SKINNED)
	{
		animOut.Joints = nullptr;
	}
	else
	{
		// Get Joints
		if (animOut.JointCount > 0)
		{
			animOut.Joints = new AnimData::Node[animOut.JointCount]();

			for (unsigned int i = 0; i < animOut.JointCount; i++)
			{
				animOut.Joints[i].Index = joints[i];

				memcpy_s(animOut.Joints[i].pNodeName,
						 AnimData::NODE_NAME_SIZE,
						 nodes[joints[i]].name.c_str(),
						 AnimData::NODE_NAME_SIZE
				);

				animOut.Joints[i].ChildrenCount = nodes[joints[i]].children.size();
				animOut.Joints[i].ChildIndex = new int[animOut.Joints[i].ChildrenCount]();

				if (nodes[joints[i]].translation.size() == 0)
				{
					animOut.Joints[i].T.x = 1.0f;
					animOut.Joints[i].T.y = 1.0f;
					animOut.Joints[i].T.z = 1.0f;

				}
				else if (nodes[joints[i]].translation.size() == 3)
				{
					animOut.Joints[i].T.x = (float)nodes[joints[i]].translation[0];
					animOut.Joints[i].T.y = (float)nodes[joints[i]].translation[1];
					animOut.Joints[i].T.z = (float)nodes[joints[i]].translation[2];
				}
				else
				{
					assert(false);
				}

				if (nodes[joints[i]].rotation.size() == 0)
				{
					animOut.Joints[i].R.x = 1.0f;
					animOut.Joints[i].R.y = 1.0f;
					animOut.Joints[i].R.z = 1.0f;
					animOut.Joints[i].R.w = 1.0f;

				}
				else if (nodes[joints[i]].rotation.size() == 4)
				{

					animOut.Joints[i].R.x = (float)nodes[joints[i]].rotation[0];
					animOut.Joints[i].R.y = (float)nodes[joints[i]].rotation[1];
					animOut.Joints[i].R.z = (float)nodes[joints[i]].rotation[2];
					animOut.Joints[i].R.w = (float)nodes[joints[i]].rotation[3];
				}
				else
				{
					assert(false);
				}


				if (nodes[joints[i]].scale.size() == 0)
				{
					animOut.Joints[i].S.x = 1.0f;
					animOut.Joints[i].S.y = 1.0f;
					animOut.Joints[i].S.z = 1.0f;

				}
				else if (nodes[joints[i]].scale.size() >= 3)
				{

					animOut.Joints[i].S.x = (float)nodes[joints[i]].scale[0];
					animOut.Joints[i].S.y = (float)nodes[joints[i]].scale[1];
					animOut.Joints[i].S.z = (float)nodes[joints[i]].scale[2];
				}
				else
				{
					assert(false);
				}

				for (int j = 0; j < animOut.Joints[i].ChildrenCount; j++)
				{
					animOut.Joints[i].ChildIndex[j] = nodes[joints[i]].children[j];
				}
			}
		}
	}

	return true;
}

void GLTF_Wrapper::Print(GLB_Header& header)
{
	Trace::out("GLB Header:\n");
	Trace::out("\tmagic: 0x%x\n", header.magic);
	Trace::out("\tversion: 0x%x\n", header.version);
	Trace::out("\tlength: 0x%08x %d\n", header.length);
	Trace::out("\n");
}

/*
// Model Query Methods
*/
Model_Query::Model_Query(const char* _pFileName, const char* outputFileName, const char* positionAttrib, const char* normAttrib, 
	const char* uVAttrib, const char* jointAttrib, const char* weightAttrib, const char* indexAttrib)
	:pBinary(nullptr),  poRawBuffer(nullptr), poJson(nullptr)
{
	size_t fileNameSize = strlen(_pFileName) + 1;
	size_t outputNameSize = strlen(outputFileName) + 1;
	size_t positionSize = strlen(positionAttrib) + 1;
	size_t normSize = strlen(normAttrib) + 1;
	size_t uvSize = strlen(uVAttrib) + 1;
	size_t jointSize = strlen(uVAttrib) + 1;
	size_t weightSize = strlen(uVAttrib) + 1;
	size_t indexSize = strlen(indexAttrib) + 1;

	this->poFileName = new char [fileNameSize]();
	this->poOutputFileName = new char[outputNameSize]();
	this->poPositionAttrib = new char[positionSize]();
	this->poNormAttrib = new char[normSize]();
	this->poUVAttrib = new char[uvSize]();
	this->poJointAttrib = new char[jointSize]();
	this->poWeightAttrib = new char[weightSize]();
	this->poIndexAttrib = new char[indexSize]();

	strcpy_s(this->poFileName, fileNameSize, _pFileName);
	strcpy_s(this->poOutputFileName, outputNameSize, outputFileName);
	strcpy_s(this->poPositionAttrib, positionSize, positionAttrib);
	strcpy_s(this->poNormAttrib, normSize, normAttrib);
	strcpy_s(this->poUVAttrib, uvSize, uVAttrib);
	strcpy_s(this->poJointAttrib, jointSize, jointAttrib);
	strcpy_s(this->poWeightAttrib, weightSize, weightAttrib);
	strcpy_s(this->poIndexAttrib, indexSize, indexAttrib);
}

Model_Query::~Model_Query()
{
	delete[] this->poFileName;
	delete[] this->poOutputFileName;
	delete[] this->poPositionAttrib;
	delete[] this->poNormAttrib;
	delete[] this->poUVAttrib;
	delete[] this->poJointAttrib;
	delete[] this->poWeightAttrib;
	delete[] this->poIndexAttrib;

	if (this->poJson)
	{
		delete[] this->poJson;
	}

	if (this->poRawBuffer)
	{
		delete[] this->poRawBuffer;
	}
}


ReverseHierarchy::ReverseHierarchy()
	: JointDepth(0), poParents(nullptr), NextOpen(0)
{}

ReverseHierarchy::ReverseHierarchy(unsigned int jointDepth, unsigned int jointCount)
	: JointDepth(jointDepth), poParents(nullptr), NextOpen(0)
{
	this->poParents = new unsigned int[jointDepth]();

	for (unsigned int i = 0; i < jointDepth; i++)
	{
		poParents[i] = jointCount;
	}
}


ReverseHierarchy::~ReverseHierarchy()
{
	if (poParents)
	{
		delete[] poParents;
		poParents = nullptr;
	}
}

const ReverseHierarchy& ReverseHierarchy::operator=(const ReverseHierarchy& b)
{
	if (this != &b)
	{
		if (poParents)
		{
			delete[] poParents;
			poParents = nullptr;
		}

		JointDepth = b.JointDepth;
		NextOpen = b.NextOpen;
		poParents = new unsigned int[JointDepth]();

		for (unsigned int i = 0; i < JointDepth; i++)
		{
			poParents[i] = b.poParents[i];
		}
	}

	return *this;
}

ReverseHierarchy::ReverseHierarchy(const ReverseHierarchy& b)
	: JointDepth(0), poParents(nullptr), NextOpen(0)
{

	this->JointDepth = b.JointDepth;
	this->NextOpen = b.NextOpen;
	this->poParents = new unsigned int[JointDepth]();

	for (unsigned int i = 0; i < JointDepth; i++)
	{
		poParents[i] = b.poParents[i];
	}
}


unsigned int ReverseHierarchy::Add(tinygltf::Skin* pSkin, unsigned int nodeIndex)
{
	std::vector<int>::iterator it = find(pSkin->joints.begin(), pSkin->joints.end(), nodeIndex);
	if (it != pSkin->joints.end())
	{
		assert(NextOpen < JointDepth);

		poParents[NextOpen] = (unsigned int)(it - pSkin->joints.begin());

		//poParents[NextOpen] = nodeIndex;

		return poParents[NextOpen++];
	}
	else
	{
		return UINT_MAX;
	}
}

void ReverseHierarchy::CopyHierarchy(unsigned int* pJointTableIndexInOut)
{
	for (unsigned int i = 0; i < JointDepth; i++)
	{
		*pJointTableIndexInOut++ = poParents[i];
	}
}
