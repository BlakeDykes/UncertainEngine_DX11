#pragma once

namespace Uncertain
{
	class NodeBase;
	class IteratorBase;

	class CollectionBase
	{
	public:
		virtual ~CollectionBase() = default;

		virtual NodeBase* GetFirst() = 0;
		virtual void Add(NodeBase& _node) = 0;
		virtual void AddToBack(NodeBase& _node) = 0;
		virtual void AddByPriority(NodeBase& _node, int priority) = 0;
		virtual NodeBase* RemoveFromFront() = 0;
		virtual NodeBase* Remove(NodeBase& _node) = 0;
		virtual IteratorBase* GetIterator() = 0;
		virtual void Wash() = 0;
	};

}
