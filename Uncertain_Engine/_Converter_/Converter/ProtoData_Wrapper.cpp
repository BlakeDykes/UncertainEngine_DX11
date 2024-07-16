#include "ProtoData_Wrapper.h"

#include "File.h"
#include "MeshData.h"
#include "ModelData.h"
#include "AnimData.h"
#include "SpriteSheetData.h"
#include "ModelTextureData.h"
#include "TextureData.h"
#include "ArmatureData.h"

using namespace Uncertain;

bool ProtoData_Wrapper::WriteToFile(const char* const modelFolder, TextureData& text)
{
	TextureData_proto textProto;

	text.Serialize(textProto);

	const char* dataFolder = "/Texture/";

	size_t nameSize = strlen(DATA_OUTPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + TextureData::FILE_NAME_SIZE + strlen(FILE_EXTENSION) + 1;
	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_OUTPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, text.pFileName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	size_t newDirNameSize = strlen(DATA_OUTPUT_DIR) + strlen(modelFolder) + strlen(dataFolder) + 1;
	char* newDirBuffer = new char[newDirNameSize];
	strcpy_s(newDirBuffer, newDirNameSize, DATA_OUTPUT_DIR);
	strcat_s(newDirBuffer, newDirNameSize, modelFolder);

	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	strcat_s(newDirBuffer, newDirNameSize, dataFolder);
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	err = File::Open(fh, fileNameBuffer, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	textProto.SerializeToString(&strOut);

	err = File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	Trace::out("- Texture - %s converted\n", text.pFileName);
	Trace::out("------ File path - %s\n", newDirBuffer);
	Trace::out("------      Size - %d\n", strOut.length());

	delete[] fileNameBuffer;
	delete[] newDirBuffer;

	return true;
}

bool ProtoData_Wrapper::WriteToFile(const char* const modelFolder, ModelTextureData& modelText)
{
	strcpy_s(modelText.pName, ModelTextureData::FILE_NAME_SIZE, ModelTextureData::FILE_PREFIX);
	strcat_s(modelText.pName, ModelTextureData::FILE_NAME_SIZE, modelFolder);

	ModelTextureData_proto textProto;

	modelText.Serialize(textProto);

	const char* dataFolder = "/Texture/";

	size_t nameSize = strlen(DATA_OUTPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + ModelTextureData::FILE_NAME_SIZE + strlen(FILE_EXTENSION) + 1;
	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_OUTPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, modelText.pName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	size_t newDirNameSize = strlen(DATA_OUTPUT_DIR) + strlen(modelFolder) + strlen(dataFolder) + 1;
	char* newDirBuffer = new char[newDirNameSize];
	strcpy_s(newDirBuffer, newDirNameSize, DATA_OUTPUT_DIR);
	strcat_s(newDirBuffer, newDirNameSize, modelFolder);

	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	strcat_s(newDirBuffer, newDirNameSize, dataFolder);
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	err = File::Open(fh, fileNameBuffer, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	textProto.SerializeToString(&strOut);

	err = File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	Trace::out("- Model Textures - %s converted\n", modelText.pName);
	Trace::out("------ File path - %s\n", newDirBuffer);
	Trace::out("------      Size - %d\n", strOut.length());

	delete[] fileNameBuffer;
	delete[] newDirBuffer;

	return true;
}

bool ProtoData_Wrapper::WriteToFile(const char* const modelFolder, MeshData& mesh)
{
	MeshData_proto meshProto;

	mesh.Serialize(meshProto);

	const char* dataFolder = "/Mesh/";

	size_t nameSize = strlen(DATA_OUTPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + MeshData::FILE_NAME_SIZE + strlen(FILE_EXTENSION) + 1;
	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_OUTPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, "mesh_");
	strcat_s(fileNameBuffer, nameSize, mesh.pMeshName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	size_t newDirNameSize = strlen(DATA_OUTPUT_DIR) + strlen(modelFolder) + strlen(dataFolder) + 1;
	char* newDirBuffer = new char[newDirNameSize];
	strcpy_s(newDirBuffer, newDirNameSize, DATA_OUTPUT_DIR);
	strcat_s(newDirBuffer, newDirNameSize, modelFolder);

	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	strcat_s(newDirBuffer, newDirNameSize, dataFolder);
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	err = File::Open(fh, fileNameBuffer, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	meshProto.SerializeToString(&strOut);

	err = File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	Trace::out("- Mesh - %s converted\n", mesh.pMeshName);
	Trace::out("------ File path - %s\n", newDirBuffer);
	Trace::out("------      Size - %d\n", strOut.length());

	delete[] fileNameBuffer;
	delete[] newDirBuffer;

	return true;
}

bool ProtoData_Wrapper::WriteToFile(const char* const modelFolder, ModelData& model)
{
	ModelData_proto modelProto;

	model.Serialize(modelProto);

	const char* dataFolder = "/Model/";

	size_t nameSize = strlen(DATA_OUTPUT_DIR) + strlen(modelFolder) + strlen(dataFolder) + ModelData::MODEL_NAME_SIZE + strlen(FILE_EXTENSION) + 1;
	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_OUTPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, model.pModelName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	size_t newDirNameSize = strlen(DATA_OUTPUT_DIR) + strlen(modelFolder) + strlen(dataFolder) + 1;
	char* newDirBuffer = new char[newDirNameSize];
	strcpy_s(newDirBuffer, newDirNameSize, DATA_OUTPUT_DIR);
	strcat_s(newDirBuffer, newDirNameSize, modelFolder);

	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	strcat_s(newDirBuffer, newDirNameSize, dataFolder);
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	err = File::Open(fh, fileNameBuffer, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	modelProto.SerializeToString(&strOut);

	err = File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	Trace::out("- Model - %s converted\n", model.pModelName);
	Trace::out("------ File path - %s\n", newDirBuffer);
	Trace::out("------      Size - %d\n", strOut.length());

	delete[] fileNameBuffer;
	delete[] newDirBuffer;

	return true;
}

bool ProtoData_Wrapper::WriteToFile(const char* const modelFolder, AnimData& anim, const char* const pFileName)
{
	AnimData_proto animProto;

	anim.Serialize(animProto);

	const char* dataFolder = "/Animation/";

	size_t nameSize = strlen(DATA_OUTPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + MeshData::FILE_NAME_SIZE + strlen(FILE_EXTENSION) + 1;
	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_OUTPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, pFileName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	size_t newDirNameSize = strlen(DATA_OUTPUT_DIR) + strlen(modelFolder) + strlen(dataFolder) + 1;
	char* newDirBuffer = new char[newDirNameSize];
	strcpy_s(newDirBuffer, newDirNameSize, DATA_OUTPUT_DIR);
	strcat_s(newDirBuffer, newDirNameSize, modelFolder);

	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	strcat_s(newDirBuffer, newDirNameSize, dataFolder);
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	err = File::Open(fh, fileNameBuffer, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	animProto.SerializeToString(&strOut);

	err = File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	Trace::out("- Animation - %s converted\n", pFileName);
	Trace::out("------ File path - %s\n", newDirBuffer);
	Trace::out("------      Size - %d\n", strOut.length());

	delete[] fileNameBuffer;
	delete[] newDirBuffer;

	return true;
}

bool ProtoData_Wrapper::WriteToFile(const char* const modelFolder, SpriteSheetData& ss)
{
	SpriteSheetData_proto ssProto;

	ss.Serialize(ssProto);

	const char* dataFolder = "/SpriteSheet/";

	size_t nameSize = strlen(DATA_OUTPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + SpriteSheetData::IMAGE_NAME_SIZE + strlen(FILE_EXTENSION) + 1;
	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_OUTPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, ss.Name);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	size_t newDirNameSize = strlen(DATA_OUTPUT_DIR) + strlen(modelFolder) + strlen(dataFolder) + 1;
	char* newDirBuffer = new char[newDirNameSize];
	strcpy_s(newDirBuffer, newDirNameSize, DATA_OUTPUT_DIR);
	strcat_s(newDirBuffer, newDirNameSize, modelFolder);

	// Ensure model folder exists
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	// Ensure model/dataType folder exists
	strcat_s(newDirBuffer, newDirNameSize, dataFolder);
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	// Write SpriteSheet
	err = File::Open(fh, fileNameBuffer, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	ssProto.SerializeToString(&strOut);

	err = File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	Trace::out("- SpriteSheet - %s converted\n", ss.Name);
	Trace::out("------ File path - %s\n", newDirBuffer);
	Trace::out("------      Size - %d\n", strOut.length());

	delete[] fileNameBuffer;
	delete[] newDirBuffer;

	return true;
}

bool ProtoData_Wrapper::WriteToFile(const char* const modelFolder, ArmatureData& arm)
{
	assert(strlen(modelFolder) + strlen(ArmatureData::FILE_PREFIX) < ArmatureData::FILE_NAME_SIZE);

	strcpy_s(arm.pArmatureName, ArmatureData::FILE_NAME_SIZE, ArmatureData::FILE_PREFIX);
	strcat_s(arm.pArmatureName, ArmatureData::FILE_NAME_SIZE, modelFolder);

	ArmatureData_proto armProto;
	arm.Serialize(armProto);

	const char* dataFolder = "/Armature/";

	size_t nameSize = strlen(DATA_OUTPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + ArmatureData::FILE_NAME_SIZE + strlen(FILE_EXTENSION) + 1;
	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_OUTPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, arm.pArmatureName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	size_t newDirNameSize = strlen(DATA_OUTPUT_DIR) + strlen(modelFolder) + strlen(dataFolder) + 1;
	char* newDirBuffer = new char[newDirNameSize];
	strcpy_s(newDirBuffer, newDirNameSize, DATA_OUTPUT_DIR);
	strcat_s(newDirBuffer, newDirNameSize, modelFolder);

	// Ensure model folder exists
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	// Ensure model/dataType folder exists
	strcat_s(newDirBuffer, newDirNameSize, dataFolder);
	err = File::MkDir(newDirBuffer);
	assert(err == File::Error::SUCCESS);

	// Write SpriteSheet
	err = File::Open(fh, fileNameBuffer, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	armProto.SerializeToString(&strOut);

	err = File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	Trace::out("- Armature - %s converted\n", arm.pArmatureName);
	Trace::out("------ File path - %s\n", newDirBuffer);
	Trace::out("------      Size - %d\n", strOut.length());

	delete[] fileNameBuffer;
	delete[] newDirBuffer;

	return true;
}


bool ProtoData_Wrapper::ReadFromFile(const char* const modelFolder, const char* const pFileName, TextureData& textOut)
{
	const char* dataFolder = "/Texture/";

	size_t nameSize = strlen(DATA_INPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + strlen(pFileName) + strlen(FILE_EXTENSION) + 1;
	assert(nameSize <= strlen(DATA_INPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + TextureData::FILE_NAME_SIZE + strlen(FILE_EXTENSION) + 1);

	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_INPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, pFileName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	err = File::Open(fh, fileNameBuffer, File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	DWORD size;
	err = File::GetSize(fh, size);

	char* poBuff = new char[size]();
	assert(poBuff);

	err = File::Read(fh, poBuff, size);
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	std::string strBuff(poBuff, size);
	TextureData_proto textProto;

	textProto.ParseFromString(strBuff);

	textOut.Deserialize(textProto);

	delete[] poBuff;
	delete[] fileNameBuffer;

	return true;
}

bool ProtoData_Wrapper::ReadFromFile(const char* const modelFolder, const char* const pFileName, MeshData& meshOut)
{
	const char* dataFolder = "/Mesh/";

	size_t nameSize = strlen(DATA_INPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + strlen(pFileName) + strlen(FILE_EXTENSION) + 1;
	assert(nameSize <= strlen(DATA_INPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + MeshData::FILE_NAME_SIZE +strlen(FILE_EXTENSION) + 1);

	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_INPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, pFileName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	err = File::Open(fh, fileNameBuffer, File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	DWORD size;
	err = File::GetSize(fh, size);

	char* poBuff = new char[size]();
	assert(poBuff);

	err = File::Read(fh, poBuff, size);
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	std::string strBuff(poBuff, size);
	MeshData_proto meshProto;

	meshProto.ParseFromString(strBuff);

	meshOut.Deserialize(meshProto);

	delete[] poBuff;
	delete[] fileNameBuffer;

	return true;
}

bool ProtoData_Wrapper::ReadFromFile(const char* const modelFolder, const char* const pFileName, ModelData& modelOut)
{
	const char* dataFolder = "/Model/";

	size_t nameSize = strlen(DATA_INPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + strlen(pFileName) + strlen(FILE_EXTENSION) + 1;
	assert(nameSize <= strlen(DATA_INPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + ModelData::MODEL_NAME_SIZE + strlen(FILE_EXTENSION) + 1);

	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_INPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, pFileName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	err = File::Open(fh, fileNameBuffer, File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	DWORD size;
	err = File::GetSize(fh, size);

	char* poBuff = new char[size]();
	assert(poBuff);

	err = File::Read(fh, poBuff, size);
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	std::string strBuff(poBuff, size);
	ModelData_proto modelProto;

	modelProto.ParseFromString(strBuff);

	modelOut.Deserialize(modelProto);

	delete[] poBuff;
	delete[] fileNameBuffer;

	return true;
}

bool ProtoData_Wrapper::ReadFromFile(const char* const modelFolder, const char* const pFileName, AnimData& animOut)
{
	const char* dataFolder = "/Animation/";

	size_t nameSize = strlen(DATA_INPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + strlen(pFileName) + strlen(FILE_EXTENSION) + 1;
	assert(nameSize <= strlen(DATA_INPUT_DIR) + strlen(dataFolder) + strlen(modelFolder) + MeshData::FILE_NAME_SIZE + strlen(FILE_EXTENSION) + 1);

	char* fileNameBuffer = new char[nameSize]();

	File::Handle fh;
	File::Error err;

	strcpy_s(fileNameBuffer, nameSize, DATA_INPUT_DIR);
	strcat_s(fileNameBuffer, nameSize, modelFolder);
	strcat_s(fileNameBuffer, nameSize, dataFolder);
	strcat_s(fileNameBuffer, nameSize, pFileName);
	strcat_s(fileNameBuffer, nameSize, FILE_EXTENSION);

	err = File::Open(fh, fileNameBuffer, File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	DWORD size;
	err = File::GetSize(fh, size);

	char* poBuff = new char[size]();
	assert(poBuff);

	err = File::Read(fh, poBuff, size);
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	std::string strBuff(poBuff, size);
	AnimData_proto animProto;

	animProto.ParseFromString(strBuff);

	animOut.Deserialize(animProto);

	delete[] poBuff;
	delete[] fileNameBuffer;

	return true;
}
