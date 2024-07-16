#ifndef ANIM_DATA_H
#define ANIM_DATA_H

#include "AnimData.pb.h"


class AnimData
{
public:
	static const unsigned int NODE_NAME_SIZE = 64;
	static const unsigned int ANIM_NAME_SIZE = 64;

	enum class TYPE
	{
		RIGID	= 0,
		SKINNED = 1
	};

	enum class PATH
	{
		SCALE		= 0,
		ROTATION	= 1,
		TRANSLATION = 2
	};

	enum class OUTPUT_TYPE
	{
		SCALAR	= 0,
		VEC2	= 1,
		VEC3	= 2,
		VEC4	= 3
	};

	struct Translation
	{
		float x;
		float y;
		float z;
	};

	struct Rotation
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct Scale
	{
		float x;
		float y;
		float z;
	};

	struct Bone
	{
		Translation poTrans;
		Rotation poRot;
		Scale poScale;
	};

	struct Frame
	{
		Frame();
		~Frame();
		float Time;
		Bone* poBones;
	};

	struct Node
	{
		char pNodeName[NODE_NAME_SIZE];
		int Index;
		int ChildrenCount;
		int* ChildIndex;
		Translation T;
		Rotation R;
		Scale S;

		~Node()
		{
			if (this->ChildIndex)
			{
				delete[] this->ChildIndex;
				this->ChildIndex = nullptr;
			}
		}
	};

	AnimData();
	AnimData(const AnimData& m) = delete;
	AnimData& operator=(const AnimData& m) = delete;
	~AnimData();

	void Serialize(AnimData_proto& out) const;
	void Deserialize(AnimData_proto& in);

	char Name[ANIM_NAME_SIZE];
	TYPE AnimType;
	unsigned int FrameCount;
	unsigned int JointCount;
	Node* Joints;
	Frame* poFrames;
};

#endif