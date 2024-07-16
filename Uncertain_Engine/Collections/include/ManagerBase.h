#ifndef MAN_BASE_H
#define MAN_BASE_H

#include "NodeBase.h"

namespace Uncertain
{
	class CollectionBase;
	class IteratorBase;

	class ManagerBase
	{
	public:

		struct Info
		{
			int DeltaGrow;
			int NumReserved;
			int NumActive;
			int TotalNodes;
			int PeakActive;
			int PeakReserved;
			int PeakNodes;

			Info()
				:DeltaGrow(0), NumReserved(0), NumActive(0), TotalNodes(0),
				PeakActive(0), PeakReserved(0), PeakNodes(0)
			{
			}
		};

		Info mInfo;
		CollectionBase* poActive;
		CollectionBase* poReserved;
		NodeBase::Handle CurrentHandle;

	protected:
		ManagerBase(CollectionBase& pActive, CollectionBase& pReserve);
		virtual ~ManagerBase();

		NodeBase* BaseAdd(int priority = 0);
		NodeBase* BaseAddToBack();
		NodeBase* BaseRemove(NodeBase& pNode);
		NodeBase* BaseFind(NodeBase& pNode);
		NodeBase* BaseFindByHandle(NodeBase::Handle id);
		NodeBase* BaseRemoveByHandle(NodeBase::Handle id);
		void BaseClear();

		void AddToReserve(int count);
		void SetReserve(int initialReserved, int deltaGrow);
		void ManageStats(int reserveChange, int activeChange);

		IteratorBase* BaseGetIterator();

		virtual void BasePrintAll();
		virtual NodeBase* CreateNode() = 0;
	};
}

#endif