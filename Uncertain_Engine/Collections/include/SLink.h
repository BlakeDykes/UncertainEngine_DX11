#pragma once

#include "NodeBase.h"

namespace Uncertain
{
	class SLink : public NodeBase
	{
	public:
		SLink();
		SLink(const SLink& sLink);
		SLink& operator = (const SLink& sLink);
		virtual ~SLink() = default;

		SLink* GetNext() const;
		void SetNext(SLink* sLink);

		virtual void Wash() override;

	private:
		SLink* pNext;

	};
}

