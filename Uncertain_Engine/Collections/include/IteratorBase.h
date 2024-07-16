#pragma once

namespace Uncertain
{
	class NodeBase;

	class IteratorBase
	{
	public:
		virtual ~IteratorBase() = default;

		virtual NodeBase* Next() = 0;
		virtual bool IsDone() = 0;
		virtual NodeBase* First() = 0;
		virtual NodeBase* Current() = 0;

		virtual IteratorBase* Reset(NodeBase* _pHead) = 0;
	};
}
