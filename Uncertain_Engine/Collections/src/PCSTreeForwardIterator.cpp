#include "PCSTreeForwardIterator.h"

namespace Uncertain
{
	PCSTreeForwardIterator::PCSTreeForwardIterator(PCSNode* rootNode)
		:pRoot(rootNode), pCurrent(rootNode)
	{
		if (this->pRoot != nullptr)
		{
			this->Done = false;
		}
		else
		{
			this->Done = true;
		}
	}

	PCSNode *PCSTreeForwardIterator::First()
	{
		if (this->pRoot == nullptr)
		{
			this->Done = true;
		}

		return this->pRoot;
	}

	PCSNode *PCSTreeForwardIterator::Next()
	{
		this->pCurrent = this->pCurrent->GetForward();

		if (this->pCurrent == nullptr)
		{
			this->Done = true;
		}

		return this->pCurrent;
	}

	bool PCSTreeForwardIterator::IsDone()
	{
		if (this->pCurrent == nullptr)
		{
			this->Done = true;
		}

		return this->Done;
	}

	PCSNode *PCSTreeForwardIterator::CurrentItem()
	{
		if (this->pCurrent == nullptr)
		{
			this->Done = true;
		}

		return this->pCurrent;
	}

	PCSTreeIterator* PCSTreeForwardIterator::Reset(PCSNode* _pRoot)
	{
		this->pRoot = _pRoot;
		this->pCurrent = this->pRoot;

		if (this->pRoot != nullptr)
		{
			this->Done = false;
		}
		else
		{
			this->Done = true;
		}

		return this;
	}
}

// ---  End of File ---------------
