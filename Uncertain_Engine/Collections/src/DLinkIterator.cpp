#include "DLinkIterator.h"
#include "NodeBase.h"
#include "DLink.h"

namespace Uncertain
{
	DLinkIterator::DLinkIterator()
		: pHead(nullptr), pCur(nullptr), Done(true)
	{
	}

	NodeBase* DLinkIterator::Next()
	{
		assert(pCur != nullptr);

		this->pCur = this->pCur->GetNext();

		this->Done = this->pCur == nullptr ? true : false;

		return this->pCur;
	}

	bool DLinkIterator::IsDone()
	{
		return this->Done;
	}

	NodeBase* DLinkIterator::First()
	{
		return this->pHead;
	}

	NodeBase* DLinkIterator::Current()
	{
		return this->pCur;
	}

	IteratorBase* DLinkIterator::Reset(NodeBase* _pHead)
	{
		if (_pHead != nullptr)
		{
			this->pHead = (DLink*)_pHead;
			this->pCur = this->pHead;
			Done = false;
		}
		else
		{
			this->Clear();
		}

		return this;
	}

	void DLinkIterator::Clear()
	{
		this->pHead = nullptr;
		this->pCur = nullptr;
		this->Done = true;
	}
}
