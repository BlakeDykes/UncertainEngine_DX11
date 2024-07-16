#include "DLinkManager.h"
#include "NodeBase.h"
#include "DLink.h"
#include "DLinkIterator.h"

namespace Uncertain
{
	DLinkManager::DLinkManager()
		:poHead(nullptr)
	{
		this->poIterator = new DLinkIterator();
	}

	DLinkManager::~DLinkManager()
	{
		DLink* pCur = this->poHead;
		DLink* pTemp = nullptr;

		while (pCur != nullptr)
		{
			pTemp = pCur->GetNext();
			delete pCur;
			pCur = pTemp;
		}

		this->poHead = nullptr;

		delete this->poIterator;
		this->poIterator = nullptr;
	}

	NodeBase* DLinkManager::GetFirst()
	{
		return this->poHead;
	}

	void DLinkManager::Add(NodeBase& _node)
	{
		DLink* pNode = (DLink*)&_node;

		if (poHead != nullptr)
		{
			poHead->SetPrev(pNode);
		}

		pNode->SetNext(poHead);
		this->poHead = pNode;
	}

	void DLinkManager::AddToBack(NodeBase& _node)
	{
		DLink* pNode = (DLink*)&_node;

		if (this->poHead == nullptr)
		{
			this->poHead = pNode;
		}
		else
		{
			DLink* pCur = this->poHead;
			while (pCur->GetNext() != nullptr)
			{
				pCur = pCur->GetNext();
			}

			pCur->SetNext(pNode);
			pNode->SetPrev(pCur);
		}
	}

	void DLinkManager::AddByPriority(NodeBase& _node, int priority)
	{
		DLink* pNode = (DLink*)&_node;

		if (this->poHead == nullptr)
		{
			this->poHead = pNode;
		}
		else
		{
			DLink* pCur = this->poHead;
			DLink* pTemp = nullptr;

			while (pCur != nullptr)
			{
				// Insert before pCur if it has a higher priority than pNode
				if (pCur->ComparePriority(priority) == DLink::PriorityCompResult::THIS_IS_GREATER_THAN)
				{
					pNode->SetNext(pCur);
					pNode->SetPrev(pTemp);
					pCur->SetPrev(pNode);

					if (pTemp)
					{
						pTemp->SetNext(pNode);
					}
					else
					{
						this->poHead = pNode;
					}

					return;
				}

				pTemp = pCur;
				pCur = pCur->GetNext();
			}

			pTemp->SetNext(pNode);
			pNode->SetPrev(pTemp);
		}
	}

	NodeBase* DLinkManager::RemoveFromFront()
	{
		DLink* pNode = this->poHead;

		if (pNode != nullptr)
		{
			if (pNode->GetNext() != nullptr)
			{
				pNode->GetNext()->SetPrev(nullptr);
			}

			this->poHead = pNode->GetNext();
		}

		pNode->SetNext(nullptr);
		pNode->SetPrev(nullptr);

		return pNode;
	}

	NodeBase* DLinkManager::Remove(NodeBase& _node)
	{
		DLink* pNode = (DLink*)&_node;

		if (this->poHead != nullptr)
		{
			if (pNode->GetNext() != nullptr)
			{
				pNode->GetNext()->SetPrev(pNode->GetPrev());
			}
			if (pNode->GetPrev() != nullptr)
			{
				pNode->GetPrev()->SetNext(pNode->GetNext());
			}
			else
			{
				this->poHead = pNode->GetNext();
			}

			pNode->SetNext(nullptr);
			pNode->SetPrev(nullptr);
		}

		return pNode;
	}

	IteratorBase* DLinkManager::GetIterator()
	{
		return this->poIterator->Reset(this->poHead);
	}

	void DLinkManager::Wash()
	{
		if (this->poHead != nullptr)
		{
			DLink* pCur = this->poHead;
			DLink* pTemp = nullptr;

			while (pCur != nullptr)
			{
				pTemp = pCur->GetNext();
				Remove(*pCur);
				pCur = pTemp;
			}

			this->poHead = nullptr;
		}
	}
}