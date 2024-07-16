#include "HandleTable.h"

namespace Uncertain
{

	using namespace ThreadFramework;

	unsigned int HandleTable::INVALID_STATE = 0x0;
	unsigned int HandleTable::PENDING_STATE = 0xFFFFFFFF;

	HandleTable::HandleTable()
	{
		this->SourceID = this->STARTING_ID;

		for (int i = 0; i < this->MAX_HANDLES; i++)
		{
			this->table[i].id.store(this->INVALID_STATE);
		}
	}

	Handle::Status HandleTable::ActivateHandle(Handle::ID& id, Handle::Index& index)
	{
		HandleTable* inst = HandleTable::GetInstance();

		Handle::Status retStatus = Handle::Status::HANDLE_ERROR;

		//inst->mtx.lock();

		if (inst->FindNextAvailable(index))
		{
			bool lockStatus = inst->table[index].mtx.try_lock();

			// Somethings gone wrong, return HANDLE_ERROR
			if (inst->table[index].id.load() != PENDING_STATE)
			{
				inst->table[index].id.store(INVALID_STATE);

				if (lockStatus)
				{
					inst->table[index].mtx.unlock();
					lockStatus = false;
				}
			}
			// Store new ID and unlock
			if (lockStatus)
			{
				retStatus = Handle::Status::SUCCESS;

				id = inst->GetNewID();
				inst->table[index].id.store(id);

				inst->table[index].mtx.unlock();

				//Trace::out("Handle activated: 0x%X\n", id);

			}
			else
			{
				retStatus = Handle::Status::HANDLE_ERROR;
			}

		}
		else
		{
			retStatus = Handle::Status::INSUFFICIENT_SPACE;

			// Handle overflow
			assert(false);
		}

		//inst->mtx.unlock();

		return retStatus;
	}

	Handle::Status HandleTable::InvalidateHandle(Handle& handle)
	{
		HandleTable* inst = HandleTable::GetInstance();

		Handle::Status retStatus = Handle::Status::HANDLE_ERROR;

		if (inst->ValidateHandle(handle) == Handle::Status::SUCCESS)
		{
			Handle::Index hIndex = handle.GetIndex();

			bool lockStatus = inst->table[hIndex].mtx.try_lock();

			if (lockStatus)
			{
				// Double check to avoid race then invalidate
				if (inst->ValidateHandle(handle) == Handle::Status::SUCCESS)
				{
					//Trace::out("Handle Invalidated: 0x%X\n", handle.GetID());

					inst->table[hIndex].id.store(INVALID_STATE);

					handle.SetIndex(GetInvalidIndex());

					retStatus = Handle::Status::SUCCESS;
				}

				inst->table[hIndex].mtx.unlock();
			}
		}

		return retStatus;

	}

	Handle::Status HandleTable::ValidateHandle(const Handle& handle)
	{
		HandleTable* inst = HandleTable::GetInstance();

		Handle::Status retStatus = Handle::Status::HANDLE_ERROR;

		Handle::Index hIndex = handle.GetIndex();

		if (hIndex < MAX_HANDLES)
		{
			if (inst->table[hIndex].id.load() == handle.GetID())
			{
				retStatus = Handle::Status::SUCCESS;
			}
			else
			{
				retStatus = Handle::Status::INVALID_HANDLE;
			}
		}
		else
		{
			retStatus = Handle::Status::INVALID_HANDLE;
		}

		return retStatus;
	}

	Handle::Status HandleTable::ValidateHandleData(const Handle::HandleData& data)
	{
		HandleTable* inst = HandleTable::GetInstance();

		Handle::Status retStatus = Handle::Status::HANDLE_ERROR;

		if (data.index < MAX_HANDLES)
		{
			if (inst->table[data.index].id.load() == data.id)
			{
				retStatus = Handle::Status::SUCCESS;
			}
			else
			{
				retStatus = Handle::Status::INVALID_HANDLE;
			}
		}
		else
		{
			retStatus = Handle::Status::INVALID_HANDLE;
		}

		return retStatus;
	}

	Handle::Status HandleTable::AcquireResource(const Handle& handle)
	{
		HandleTable* inst = HandleTable::GetInstance();

		Handle::Status retStatus = inst->ValidateHandle(handle);

		if (retStatus == Handle::Status::SUCCESS)
		{
			bool lockStatus = inst->table[handle.GetIndex()].mtx.try_lock();

			if (lockStatus)
			{
				// Double check for race condition
				retStatus = inst->ValidateHandle(handle);

				// handle has invalidated since we validated the first time
				if (retStatus != Handle::Status::SUCCESS)
				{
					inst->table[handle.GetIndex()].mtx.unlock();
				}
			}
			else
			{
				retStatus = Handle::Status::HANDLE_ERROR;
			}
		}

		return retStatus;
	}

	Handle::Status HandleTable::ReleaseResource(const Handle& handle)
	{
		HandleTable* inst = HandleTable::GetInstance();

		Handle::Status retStatus = inst->ValidateHandle(handle);

		if (retStatus == Handle::Status::SUCCESS)
		{
			inst->table[handle.GetIndex()].mtx.unlock();
		}

		return retStatus;
	}

	bool HandleTable::FindNextAvailable(Handle::Index& index)
	{

		bool retStatus = false;

		for (Handle::Index i = 0; i < MAX_HANDLES; i++)
		{
			if (this->table[i].id.compare_exchange_strong(INVALID_STATE, PENDING_STATE))
			{
				index = i;
				retStatus = true;
				break;
			}
		}

		return retStatus;
	}

	Handle::ID HandleTable::GetNewID()
	{
		HandleTable* inst = HandleTable::GetInstance();

		return inst->SourceID++;
	}

	Handle::ID HandleTable::GetInvalidID()
	{
		HandleTable* inst = HandleTable::GetInstance();

		return inst->INVALID_STATE;
	}

	Handle::Index HandleTable::GetInvalidIndex()
	{
		HandleTable* inst = HandleTable::GetInstance();

		return inst->INVALID_INDEX;
	}

	void HandleTable::PrintTable()
	{
		HandleTable* inst = HandleTable::GetInstance();
		assert(inst);

		Trace::out("\n");

		// No protection.... Just printing
		for (int i = 0; i < 100; i++)
		{
			if (inst->table[i].id == INVALID_STATE)
			{
				Trace::out("[%d]: %s \n", i, STRING_ME(INVALID_STATE));
			}
			else
			{
				Trace::out("[%d]: %x \n", i, inst->table[i].id.load());
			}
		}

		Trace::out("\n");
	}

	HandleTable* HandleTable::GetInstance()
	{
		static HandleTable instance;

		return &instance;
	}
}
