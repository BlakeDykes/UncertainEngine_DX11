#pragma once

namespace Uncertain
{
	class NodeBase
	{
	public:
		typedef unsigned int Handle;

		virtual ~NodeBase() = default;

		virtual bool Compare(NodeBase& pNode) = 0;
		virtual void Wash() { this->ID = 0; }
		virtual const char* GetName() = 0;
		virtual void Print() = 0;

		Handle GetHandle() const { return this->ID; }
		bool IsHandleValid() const { return ID != 0; }

	private:
		friend class ManagerBase;

		Handle ID = 0;
	};
}
