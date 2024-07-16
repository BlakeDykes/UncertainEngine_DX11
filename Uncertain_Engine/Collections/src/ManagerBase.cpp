#include "ManagerBase.h"
#include "CollectionBase.h"
#include "IteratorBase.h"

namespace Uncertain
{
	ManagerBase::ManagerBase(CollectionBase& pActive, CollectionBase& pReserve)
		: mInfo(), poActive(&pActive), poReserved(&pReserve), CurrentHandle(0)
	{
	}

	ManagerBase::~ManagerBase()
	{
		delete this->poActive;
		delete this->poReserved;

		this->poActive = nullptr;
		this->poReserved = nullptr;
	}

	NodeBase* ManagerBase::BaseAdd(int priority)
	{
		IteratorBase* it = this->poReserved->GetIterator();

		while (it->First() == nullptr)
		{
			AddToReserve(this->mInfo.DeltaGrow);
			it = this->poReserved->GetIterator();
		}

		NodeBase* pNode = poReserved->RemoveFromFront();
		assert(pNode != nullptr);

		if (priority == 0)
		{
			this->poActive->Add(*pNode);
		}
		else
		{
			this->poActive->AddByPriority(*pNode, priority);
		}

		pNode->ID = ++this->CurrentHandle;

		ManageStats(-1, 1);

		return pNode;
	}

	NodeBase* ManagerBase::BaseAddToBack()
	{
		IteratorBase* it = this->poReserved->GetIterator();

		while (it->First() == nullptr)
		{
			AddToReserve(this->mInfo.DeltaGrow);
			it = this->poReserved->GetIterator();
		}

		NodeBase* pNode = poReserved->RemoveFromFront();
		assert(pNode != nullptr);

		pNode->Wash();

		this->poActive->AddToBack(*pNode);

		pNode->ID = ++this->CurrentHandle;

		ManageStats(-1, 1);

		return pNode;
	}

	NodeBase* ManagerBase::BaseRemove(NodeBase& pNode)
	{
		NodeBase* pRem = this->poActive->Remove(pNode);

		pRem->Wash();

		this->poReserved->Add(pNode);

		ManageStats(1, -1);

		return pRem;
	}

	NodeBase* ManagerBase::BaseFind(NodeBase& pNode)
	{
		IteratorBase* it = this->poActive->GetIterator();
		assert(it != nullptr);

		NodeBase* pCur = it->First();

		while (it->IsDone() != true)
		{
			if (pCur->Compare(pNode) == true)
			{
				break;
			}
			pCur = it->Next();
		}

		return pCur;
	}

	NodeBase* ManagerBase::BaseFindByHandle(NodeBase::Handle id)
	{
		IteratorBase* it = this->poActive->GetIterator();
		assert(it != nullptr);

		NodeBase* pCur = it->First();

		while (it->IsDone() != true)
		{
			if (pCur->ID == id)
			{
				break;
			}
			pCur = it->Next();
		}

		return pCur;
	}

	NodeBase* ManagerBase::BaseRemoveByHandle(NodeBase::Handle id)
	{
		IteratorBase* it = this->poActive->GetIterator();
		assert(it != nullptr);

		NodeBase* pCur = it->First();

		while (it->IsDone() != true)
		{
			if (pCur->ID == id)
			{
				this->poActive->Remove(*pCur);
				pCur->Wash();
				this->poReserved->Add(*pCur);

				ManageStats(1, -1);

			}
			pCur = it->Next();
		}

		return pCur;
	}

	void ManagerBase::BaseClear()
	{
		IteratorBase* it = this->poActive->GetIterator();
		assert(it != nullptr);

		NodeBase* pCur = it->First();
		NodeBase* pTemp = nullptr;

		while (it->IsDone() != true)
		{
			pTemp = it->Next();
			BaseRemove(*pCur);
			pCur = pTemp;
		}
	}

	void ManagerBase::AddToReserve(int count)
	{
		assert(count > 0);

		for (int i = 0; i < count; i++)
		{
			NodeBase* pNode = this->CreateNode();
			poReserved->Add(*pNode);
		}

		ManageStats(count, 0);
	}

	void ManagerBase::SetReserve(int initialReserved, int deltaGrow)
	{
		assert(deltaGrow > 0);
		this->mInfo.DeltaGrow = deltaGrow;

		if (initialReserved > this->mInfo.NumReserved)
		{
			this->AddToReserve(initialReserved - this->mInfo.NumReserved);
		}
	}

	void ManagerBase::ManageStats(int reserveChange, int activeChange)
	{
		this->mInfo.NumReserved += reserveChange;
		this->mInfo.PeakReserved = this->mInfo.PeakReserved < this->mInfo.NumReserved ? this->mInfo.NumReserved : this->mInfo.PeakReserved;

		this->mInfo.NumActive += activeChange;
		this->mInfo.PeakActive = this->mInfo.PeakActive < this->mInfo.NumActive ? this->mInfo.NumActive : this->mInfo.PeakActive;

		this->mInfo.TotalNodes = this->mInfo.NumActive + this->mInfo.NumReserved;
		this->mInfo.PeakNodes = this->mInfo.PeakNodes < this->mInfo.TotalNodes ? this->mInfo.TotalNodes : this->mInfo.PeakNodes;
	}

	IteratorBase* ManagerBase::BaseGetIterator()
	{
		return this->poActive->GetIterator();
	}

	void ManagerBase::BasePrintAll()
	{
		IteratorBase* pIt = this->poActive->GetIterator();

		NodeBase* pNode = pIt->First();

		while (!pIt->IsDone())
		{
			pNode->Print();
			pNode = pIt->Next();
		}

		Trace::out("--------------------------\n");
	}
}