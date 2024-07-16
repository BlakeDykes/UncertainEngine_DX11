#pragma once

#include "NodeBase.h"

namespace Uncertain
{
	class DLink : public NodeBase
	{

	private:
		DLink* pNext;
		DLink* pPrev;

	public:

		enum PriorityCompResult
		{
			THIS_IS_GREATER_THAN,
			THIS_IS_LESS_THAN,

			UNDEFINED
		};

		DLink();
		virtual ~DLink() = default;
		DLink(const DLink& d);
		const DLink& operator =(const DLink& d);

		void SetNext(DLink* d);
		void SetPrev(DLink* d);
		DLink* GetNext() const;
		DLink* GetPrev() const;


		virtual void Wash() override;
		virtual void Print() override;
		virtual bool Compare(NodeBase& pNode) override;
		virtual const char* GetName() override;
		virtual PriorityCompResult ComparePriority(int priority);

	};

}
