#pragma once

#include "IteratorBase.h"

namespace Uncertain
{
	class DLink;

	class DLinkIterator : public IteratorBase
	{
	public:
		DLink* pHead;
		DLink* pCur;
		bool Done;

		DLinkIterator();

		NodeBase* Next() override;
		bool IsDone() override;
		NodeBase* First() override;
		NodeBase* Current() override;

		IteratorBase* Reset(NodeBase* _pHead) override;

	private:
		void Clear();
	};
}
