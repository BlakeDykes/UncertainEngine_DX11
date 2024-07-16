#include "DLink.h"

namespace Uncertain
{
	DLink::DLink()
	{
		pNext = nullptr;
		pPrev = nullptr;
	}

	DLink::DLink(const DLink& d)
	{
		this->pNext = d.pNext;
		this->pPrev->pNext = d.pPrev;
	}

	const DLink& DLink::operator =(const DLink& d)
	{
		this->pNext = d.pNext;
		this->pPrev->pNext = d.pPrev;

		return *this;
	}

	void DLink::SetNext(DLink* d)
	{
		this->pNext = d;
	}

	void DLink::SetPrev(DLink* d)
	{
		this->pPrev = d;
	}

	DLink* DLink::GetNext() const
	{
		return this->pNext;
	}

	DLink* DLink::GetPrev() const
	{
		return this->pPrev;
	}

	void DLink::Wash()
	{
		NodeBase::Wash();
		pNext = nullptr;
		pPrev = nullptr;
	}

	void DLink::Print()
	{
		Trace::out("---Prev: 0x%p\n", this->pPrev);
		Trace::out("---Next: 0x%p\n", this->pNext);
	}

	bool DLink::Compare(NodeBase& pNode)
	{
		// Compare not implemented in derived class
		AZUL_UNUSED_VAR(pNode);
		assert(false);
		return false;
	}

	DLink::PriorityCompResult DLink::ComparePriority(int priority)
	{
		// ComparePriority not implemented in derived class
		AZUL_UNUSED_VAR(priority);
		assert(false);
		return DLink::PriorityCompResult::UNDEFINED;
	}

	const char* DLink::GetName()
	{
		// GetName not implemented in derived class
		assert(false);
		return nullptr;
	}
}

