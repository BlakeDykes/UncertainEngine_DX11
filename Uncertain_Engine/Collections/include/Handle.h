#pragma once

#include "DLink.h"

namespace Uncertain
{
	class Handle : public DLink
	{
	public:

		typedef unsigned int ID;
		typedef unsigned int Index;

		enum class Status
		{
			SUCCESS,
			INSUFFICIENT_SPACE,
			INVALID_HANDLE,

			HANDLE_ERROR
		};

		class Lock
		{
		public:
			Lock(const Handle& h);

			Lock() = delete;
			Lock(const Lock&) = delete;
			Lock& operator =(const Lock&) = delete;
			~Lock();

			operator bool() const;
			operator Handle::Status() const;

		private:
			const Handle& handle;
			Handle::Status status;
		};

		class HandleData
		{
		public:
			ID id;
			Index index;

			HandleData();
			void Clear();
			Handle::Status ValidateData();
		};

		Handle();
		~Handle();

		Handle(const Handle&) = delete;
		const Handle& operator =(const Handle&) = delete;

		bool operator==(const Handle& h) const;
		bool operator!=(const Handle& h) const;

		ID GetID() const { return this->id; }
		void SetID(const ID _id) { this->id = _id; }
		Index GetIndex() const { return this->index; }
		void SetIndex(const Index _index) { this->index = _index; }


		Status ValidateHandle() const;
		Status InvalidateHandle();
		Status AcquireResource() const;
		Status ReleaseResource() const;
		Status ActivateHandle();


		virtual void Wash() override;
		virtual bool Compare(NodeBase& pNode) override;

	private:
		ID id;
		Index index;

	};
}
