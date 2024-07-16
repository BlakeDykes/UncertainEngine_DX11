#include "MeshData.h"


MeshData::MeshData()
	:pMeshName{ 0 }, RenderMode{ MeshData::RENDER_MODE::DEFAULT },
	TriCount(0), VertCount(0), VBOVert(), VBONorm(), VBOUV(),
	VBOIndex(), poSkinData(nullptr), poBounding(nullptr), MaterialIndex(0), NodeIndex(0)
{
}

MeshData::~MeshData()
{
	if (this->poBounding)
	{
		delete this->poBounding;
		this->poBounding = nullptr;
	}
	if (this->poSkinData)
	{
		delete this->poSkinData;
		this->poSkinData = nullptr;
	}
}


void MeshData::Serialize(MeshData_proto& out) const
{
	std::string meshName((const char*)this->pMeshName, FILE_NAME_SIZE);
	out.set_pmeshname(meshName);

	out.set_mode((MeshData_proto_RENDER_MODE)this->RenderMode);
	out.set_tricount(this->TriCount);
	out.set_vertcount(this->VertCount);
	out.set_materialindex(this->MaterialIndex);
	out.set_nodeindex(this->NodeIndex);

	assert(this->VertCount >= 3);
	assert(this->TriCount > 0);

	VBOData_proto* pVBO;

	pVBO = new VBOData_proto();
	this->VBOVert.Serialize(*pVBO);
	out.set_allocated_vbovert(pVBO);

	pVBO = new VBOData_proto();
	this->VBONorm.Serialize(*pVBO);
	out.set_allocated_vbonorm(pVBO);

	pVBO = new VBOData_proto();
	this->VBOUV.Serialize(*pVBO);
	out.set_allocated_vbouv(pVBO);

	pVBO = new VBOData_proto();
	this->VBOIndex.Serialize(*pVBO);
	out.set_allocated_vboindex(pVBO);

	if (this->poSkinData)
	{
		MeshData_proto_SkinningData* pSkinData =  out.mutable_skindata();

		pVBO = new VBOData_proto();
		this->poSkinData->VBOWeights.Serialize(*pVBO);
		pSkinData->set_allocated_vboweights(pVBO);

		pVBO = new VBOData_proto();
		this->poSkinData->VBOJoints.Serialize(*pVBO);
		pSkinData->set_allocated_vbojoints(pVBO);

		pVBO = new VBOData_proto();
		this->poSkinData->VBOInvBind.Serialize(*pVBO);
		pSkinData->set_allocated_vboinvbind(pVBO);
	}

	BoundingObjectData_proto* pBounding = new BoundingObjectData_proto();
	this->poBounding->Serialize(*pBounding);
	out.set_allocated_bounding_sphere(pBounding);
}

void MeshData::Deserialize(MeshData_proto& in)
{
	this->RenderMode = (RENDER_MODE)in.mode();
	this->TriCount = in.tricount();
	this->VertCount = in.vertcount();
	this->VBOVert.Deserialize(in.vbovert());
	this->VBONorm.Deserialize(in.vbonorm());
	this->VBOUV.Deserialize(in.vbouv());
	this->VBOIndex.Deserialize(in.vboindex());
	this->MaterialIndex = in.materialindex();
	this->NodeIndex = in.nodeindex();

	// Set skindata if it exists
	if (in.has_skindata())
	{
		if (this->poSkinData)
		{
			delete this->poSkinData;
		}

		MeshData_proto::SkinningData skinData = in.skindata();
		this->poSkinData = new SkinningData();

		this->poSkinData->VBOWeights.Deserialize(skinData.vboweights());
		this->poSkinData->VBOJoints.Deserialize(skinData.vbojoints());
		this->poSkinData->VBOInvBind.Deserialize(skinData.vboinvbind());
	}

	// Set bounding
	Vec3* vertBuff = new Vec3[this->VBOVert.Count]();
	float* pVert = (float*)VBOVert.poData;

	for (unsigned int i = 0; i < VBOVert.Count; i++)
	{
		vertBuff[i].set(*pVert, *(pVert + 1), *(pVert + 2));
		pVert += 3;
	}

	this->poBounding = new BoundingObjectData(vertBuff, VBOVert.Count);

	delete[] vertBuff;

	memcpy_s(this->pMeshName, FILE_NAME_SIZE, in.pmeshname().data(), FILE_NAME_SIZE);
}

void MeshData::Print() const
{
	Trace::out("----------------------------\n");
	Trace::out("%s: \n", this->pMeshName);
	Trace::out("      Render Mode: %d\n", this->RenderMode);
	Trace::out("      Tri Count: %d\n", this->TriCount);
	Trace::out("      Vert Count: %d\n", this->VertCount);
	//Trace::out("	  Texture Count: %u\n", this->TextureCount);

	Trace::out("----------------------------\n");

	Trace::out("\n");
	this->VBOVert.Print("VBO - Vert");
	Trace::out("\n");
	this->VBONorm.Print("VBO - Norm");
	Trace::out("\n");
	this->VBOUV.Print("VBO - UV");
	Trace::out("\n");
	this->VBOIndex.Print("VBO - Index");
	Trace::out("\n");
	//TextureData* pText = this->Textures;
	//for (unsigned int i = 0; i < this->TextureCount; i++)
	//{
	//	pText->Print();
	//	pText++;
	//}
	//this->Textures->Print();
	Trace::out("----------------------------\n");
	Trace::out("\n");
}