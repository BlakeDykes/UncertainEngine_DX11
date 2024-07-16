#include "PCSTreeReverseIterator.h"

namespace Uncertain
{
	PCSTreeReverseIterator::PCSTreeReverseIterator(PCSNode *rootNode)
		:pRoot(rootNode), Done(true)
	{
		PCSNode* pCur = rootNode;
		if (!pCur)
		{
			this->pCurrent = pCur;
		}
		else
		{
			while (pCur->GetForward() != nullptr)
			{
				pCur = pCur->GetForward();
			}

			this->pTail = pCur;
			this->pCurrent = this->pTail;
			this->Done = false;
		}
	}

	PCSNode *PCSTreeReverseIterator::First()
	{
		if (this->pRoot != nullptr)
		{
			if (this->pTail == nullptr)
			{
				this->pTail = this->pRoot;
			}

			while (this->pTail->GetForward() != nullptr)
			{
				this->pTail = this->pTail->GetForward();
			}
		}

		if (this->pTail == nullptr)
		{
			this->pCurrent = nullptr;
			this->Done = true;
		}

		return  this->pTail;
	}

	PCSNode *PCSTreeReverseIterator::Next()
	{
		if (this->pCurrent != nullptr)
		{
			this->pCurrent = this->pCurrent->GetReverse();
		}
		
		if (this->pCurrent == nullptr)
		{
			this->Done = true;
		}

		return  this->pCurrent;
	}

	bool PCSTreeReverseIterator::IsDone()
	{
		if (this->pCurrent == nullptr)
		{
			this->Done = true;
		}

		return this->Done;
	}

	PCSNode *PCSTreeReverseIterator::CurrentItem()
	{
		if (this->pCurrent == nullptr)
		{
			this->Done = true;
		}

		return  this->pCurrent;
	}

	PCSTreeIterator* PCSTreeReverseIterator::Reset(PCSNode* _pRoot)
	{
		this->pRoot = _pRoot;

		PCSNode* pCur = this->pRoot;
		if (!pCur)
		{
			this->pCurrent = pCur;
			this->Done = true;
		}
		else
		{
			while (pCur->GetForward() != nullptr)
			{
				pCur = pCur->GetForward();
			}

			this->pTail = pCur;
			this->pCurrent = this->pTail;
			this->Done = false;
		}

		return this;
	}
}

// ---  End of File ---------------
