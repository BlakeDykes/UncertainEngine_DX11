#include "ModelNode.h"
#include "Model_NULL.h"

ModelNode::ModelNode()
	: poModel(nullptr)
{
}

ModelNode::~ModelNode()
{
	delete poModel;
}

void ModelNode::SetNull()
{
	delete poModel;
	poModel = new Model_NULL();
}

bool ModelNode::Compare(NodeBase& pNode)
{
	return this->GetHandle() == pNode.GetHandle() && this->GetType() == static_cast<ModelNode*>(&pNode)->GetType();
}
