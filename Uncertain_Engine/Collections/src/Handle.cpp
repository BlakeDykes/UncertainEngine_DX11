#include "Handle.h"
#include "HandleTable.h"


using namespace ThreadFramework;

namespace Uncertain
{
	Handle::Handle()
	{
		//Status status = HandleManager::ActivateHandle(this->id, this->index);
		//assert(status == Status::SUCCESS);

		//Trace::out("New handle created: 0x%X\n", this->id);
	}

	Handle::~Handle()
	{
		this->InvalidateHandle();
	}

	void Handle::Wash()
	{
		DLink::Wash();
		this->id = HandleTable::GetInvalidID();
	}

	bool Handle::Compare(NodeBase& pNode)
	{
		Handle* pComp = (Handle*)&pNode;

		return this->id == pComp->id;
	}

	bool Handle::operator==(const Handle& h) const
	{
		return this->id == h.id;
	}

	bool Handle::operator!=(const Handle& h) const
	{
		return this->id != h.id;
	}

	Handle::Status Handle::ActivateHandle()
	{
		return HandleTable::ActivateHandle(this->id, this->index);
	}

	Handle::Status Handle::ValidateHandle() const
	{
		return HandleTable::ValidateHandle(*this);
	}

	Handle::Status Handle::InvalidateHandle()
	{
		return HandleTable::InvalidateHandle(*this);
	}

	Handle::Status Handle::AcquireResource() const
	{
		return HandleTable::AcquireResource(*this);
	}

	Handle::Status Handle::ReleaseResource() const
	{
		return HandleTable::ReleaseResource(*this);
	}

	Handle::Status Handle::HandleData::ValidateData()
	{
		return HandleTable::ValidateHandleData(*this);
	}


	Handle::HandleData::HandleData()
		:id(HandleTable::GetNewID()), index(HandleTable::GetInvalidIndex())
	{
	}

	void Handle::HandleData::Clear()
	{
		this->id = HandleTable::GetInvalidID();
		this->index = HandleTable::GetInvalidIndex();
	}


	// ----------------------------------------------------------------------
	// Handle::Lock method implementations ----------------------------------
	// ----------------------------------------------------------------------

	Handle::Lock::Lock(const Handle& h)
		:handle(h)
	{
		this->status = handle.AcquireResource();
	}

	Handle::Lock::~Lock()
	{
		if (this->status == Handle::Status::SUCCESS)
		{
			this->status = this->handle.ReleaseResource();
		}
	}

	Handle::Lock::operator bool() const
	{
		return (this->status == Handle::Status::SUCCESS);
	}

	Handle::Lock::operator Handle::Status() const
	{
		return this->status;
	}
}