#pragma once

#include "CollectionBase.h"

namespace Uncertain
{
	class DLink;
	class DLinkIterator;

	class DLinkManager : public CollectionBase
	{
	public:

		DLinkManager();
		DLinkManager(const DLinkManager&) = default;
		DLinkManager& operator=(const DLinkManager&) = default;
		virtual ~DLinkManager();

		virtual NodeBase* GetFirst() override;
		virtual void Add(NodeBase& _node) override;
		virtual void AddByPriority(NodeBase& _node, int priority) override;
		virtual void AddToBack(NodeBase& _node) override;
		virtual NodeBase* RemoveFromFront() override;
		virtual NodeBase* Remove(NodeBase& _node) override;
		virtual IteratorBase* GetIterator() override;
		virtual void Wash() override;

	private:
		DLink* poHead;
		DLinkIterator* poIterator;
	};
}
