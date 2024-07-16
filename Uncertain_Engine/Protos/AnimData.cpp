#include "AnimData.h"


AnimData::AnimData()
	: Name{ 0 }, FrameCount(0), JointCount(0), Joints(nullptr), poFrames(nullptr)
{
}

AnimData::~AnimData()
{
	if (this->Joints)
	{
		delete[] this->Joints;
		this->Joints = nullptr;
	}
	if (this->poFrames)
	{
		delete[] this->poFrames;
	}
}

void AnimData::Serialize(AnimData_proto& out) const
{
	std::string animName((const char*)this->Name, ANIM_NAME_SIZE);
	out.set_animtype((AnimData_proto_TYPE)this->AnimType);
	out.set_framecount(this->FrameCount);
	out.set_name(animName);
	out.set_jointcount(this->JointCount);

	AnimData_proto::Frame* pFrame = nullptr;
	AnimData_proto::Bone* pBone = nullptr;
	AnimData_proto::Translation* pTrans = nullptr;
	AnimData_proto::Rotation* pRot = nullptr;
	AnimData_proto::Scale* pScale = nullptr;


	for (unsigned int i = 0; i < this->FrameCount; i++)
	{
		pFrame = out.add_frames();
		pFrame->set_time(this->poFrames[i].Time);

		for (unsigned int j = 0; j < JointCount; j++)
		{
			pBone = pFrame->add_bones();

			pTrans = pBone->mutable_trans();
			pTrans->set_x(this->poFrames[i].poBones[j].poTrans.x);
			pTrans->set_y(this->poFrames[i].poBones[j].poTrans.y);
			pTrans->set_z(this->poFrames[i].poBones[j].poTrans.z);

			pRot = pBone->mutable_rot();
			pRot->set_x(this->poFrames[i].poBones[j].poRot.x);
			pRot->set_y(this->poFrames[i].poBones[j].poRot.y);
			pRot->set_z(this->poFrames[i].poBones[j].poRot.z);
			pRot->set_w(this->poFrames[i].poBones[j].poRot.w);

			pScale = pBone->mutable_scale();
			pScale->set_x(this->poFrames[i].poBones[j].poScale.x);
			pScale->set_y(this->poFrames[i].poBones[j].poScale.y);
			pScale->set_z(this->poFrames[i].poBones[j].poScale.z);
		}
	}

	if (this->AnimType != AnimData::TYPE::SKINNED)
	{
		for (unsigned int i = 0; i < this->JointCount; i++)
		{
			AnimData_proto::Node* joint = out.add_joints();
			std::string jointName((const char*)this->Joints[i].pNodeName, NODE_NAME_SIZE);

			AnimData_proto::Translation* t = new AnimData_proto::Translation();
			t->set_x(this->Joints[i].T.x);
			t->set_y(this->Joints[i].T.y);
			t->set_z(this->Joints[i].T.z);
			joint->set_allocated_t(t);

			AnimData_proto::Rotation* r = new AnimData_proto::Rotation();
			r->set_x(this->Joints[i].R.x);
			r->set_y(this->Joints[i].R.y);
			r->set_z(this->Joints[i].R.z);
			r->set_w(this->Joints[i].R.w);
			joint->set_allocated_r(r);

			AnimData_proto::Scale* s = new AnimData_proto::Scale();
			s->set_x(this->Joints[i].S.x);
			s->set_y(this->Joints[i].S.y);
			s->set_z(this->Joints[i].S.z);
			joint->set_allocated_s(s);

			joint->set_nodename(jointName);
			joint->set_index(this->Joints[i].Index);
			joint->set_childrencount(this->Joints[i].ChildrenCount);

			for (int j = 0; j < this->Joints[i].ChildrenCount; j++)
			{
				joint->add_childindex(this->Joints[i].ChildIndex[j]);
			}
		}
	}
}

void AnimData::Deserialize(AnimData_proto& in)
{
	memcpy_s(this->Name, ANIM_NAME_SIZE, in.name().data(), ANIM_NAME_SIZE);
	this->AnimType = (AnimData::TYPE)in.animtype();
	this->FrameCount = in.framecount();
	this->JointCount = in.jointcount();

	if (this->AnimType != AnimData::TYPE::SKINNED && this->JointCount > 0)
	{
		if (this->Joints)
		{
			delete[] this->Joints;
		}

		this->Joints = new Node[this->JointCount]();

		for (unsigned int i = 0; i < this->JointCount; i++)
		{
			AnimData_proto::Node* nodeProto = in.mutable_joints((int)i);
			assert(nodeProto);

			this->Joints[i].Index = nodeProto->index();
			memcpy_s(this->Joints[i].pNodeName, NODE_NAME_SIZE, nodeProto->nodename().data(), NODE_NAME_SIZE);
			this->Joints[i].ChildrenCount = nodeProto->childrencount();

			this->Joints[i].T.x = nodeProto->t().x();
			this->Joints[i].T.y = nodeProto->t().y();
			this->Joints[i].T.z = nodeProto->t().z();

			this->Joints[i].R.x = nodeProto->r().x();
			this->Joints[i].R.y = nodeProto->r().y();
			this->Joints[i].R.z = nodeProto->r().z();
			this->Joints[i].R.w = nodeProto->r().w();

			this->Joints[i].S.x = nodeProto->s().x();
			this->Joints[i].S.y = nodeProto->s().y();
			this->Joints[i].S.z = nodeProto->s().z();

			if (this->Joints[i].ChildrenCount > 0)
			{
				this->Joints[i].ChildIndex = new int[(unsigned int)this->Joints[i].ChildrenCount]();

				for (int j = 0; j < this->Joints[i].ChildrenCount; j++)
				{
					this->Joints[i].ChildIndex[j] = nodeProto->childindex(j);
				}
			}
		}
	}
	else
	{
		this->Joints = nullptr;
	}

	// Frames
	google::protobuf::RepeatedPtrField<AnimData_proto_Frame>* pFrame = in.mutable_frames();
	google::protobuf::RepeatedPtrField<AnimData_proto_Bone> bones;

	this->poFrames = new AnimData::Frame[this->FrameCount]();

	for (unsigned int i = 0; i < this->FrameCount; i++)
	{
		this->poFrames[i].Time = pFrame->Get(i).time();
		this->poFrames[i].poBones = new AnimData::Bone[JointCount]();
		bones = pFrame->Get(i).bones();

		for (unsigned int j = 0; j < JointCount; j++)
		{
			this->poFrames[i].poBones[j].poTrans.x = bones[j].trans().x();
			this->poFrames[i].poBones[j].poTrans.y = bones[j].trans().y();
			this->poFrames[i].poBones[j].poTrans.z = bones[j].trans().z();

			this->poFrames[i].poBones[j].poRot.x = bones[j].rot().x();
			this->poFrames[i].poBones[j].poRot.y = bones[j].rot().y();
			this->poFrames[i].poBones[j].poRot.z = bones[j].rot().z();
			this->poFrames[i].poBones[j].poRot.w = bones[j].rot().w();

			this->poFrames[i].poBones[j].poScale.x = bones[j].scale().x();
			this->poFrames[i].poBones[j].poScale.y = bones[j].scale().y();
			this->poFrames[i].poBones[j].poScale.z = bones[j].scale().z();
		}
	}
}

AnimData::Frame::Frame()
	: Time(0.0f), poBones(nullptr)
{
}

AnimData::Frame::~Frame()
{
	if (poBones)
	{
		delete[] poBones;
	}
}