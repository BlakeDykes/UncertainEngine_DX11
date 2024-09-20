#include "TextureData.pb.h"
#include "MeshData.pb.h"
#include "ModelData.pb.h"
#include "ModelTextureData.pb.h"
#include "AnimData.pb.h"
#include "SpriteSheetData.pb.h"
#include "ArmatureData.pb.h"

class MeshData;
class ModelData;
class TextureData;
class ModelTextureData;
class AnimData;
class SpriteSheetData;
class ArmatureData;

class ProtoData_Wrapper
{
public:

	static const char* DATA_OUTPUT_DIR;
	static const char* DATA_INPUT_DIR;
	static const char* FILE_EXTENSION;

	static bool WriteToFile(const char* const modelFolder, TextureData& text);
	static bool WriteToFile(const char* const modelFolder, ModelTextureData& text);
	static bool WriteToFile(const char* const modelFolder, MeshData& mesh);
	static bool WriteToFile(const char* const modelFolder, ModelData& model);
	static bool WriteToFile(const char* const modelFolder, AnimData& anim, const char* const pFileName);
	static bool WriteToFile(const char* const modelFolder, SpriteSheetData& ss);
	static bool WriteToFile(const char* const modelFolder, ArmatureData& arm);
	static bool ReadFromFile(const char* const modelFolder, const char* const pFileName, TextureData& textOut);
	static bool ReadFromFile(const char* const modelFolder, const char* const pFileName, MeshData& meshOut);
	static bool ReadFromFile(const char* const modelFolder, const char* const pFileName, ModelData& modelOut);
	static bool ReadFromFile(const char* const modelFolder, const char* const pFileName, AnimData& animOut);

};