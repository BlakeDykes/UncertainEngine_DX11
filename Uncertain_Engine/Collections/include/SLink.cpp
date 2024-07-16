#include "SLink.h"

namespace Uncertain
{
	SLink::SLink()
	{
		this->pNext = nullptr;
	}

	SLink::SLink(const SLink& sLink)
	{
		this->pNext = sLink.pNext;
	}

	SLink& SLink::operator = (const SLink& sLink)
	{
		this->pNext = sLink.pNext;
		return *this;
	}

	SLink* SLink::GetNext() const
	{
		return this->pNext;
	}

	void SLink::SetNext(SLink* sLink)
	{
		this->pNext = sLink;
	}

	void SLink::Wash()
	{
		this->pNext = nullptr;
	}

}