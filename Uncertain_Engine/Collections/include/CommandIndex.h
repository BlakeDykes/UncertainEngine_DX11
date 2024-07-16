#pragma once

namespace Uncertain
{
	class CommandIndex
	{
	public:
		CommandIndex(int buffSize);

		CommandIndex() = delete;
		CommandIndex(const CommandIndex&) = delete;
		const CommandIndex& operator = (const CommandIndex&) = delete;
		~CommandIndex() = default;

		// postfix
		int operator++(int);
		bool operator==(const CommandIndex& c);
		bool operator!=(const CommandIndex& c);

		int GetIndex() const;
		void SetIndex(int _index) { this->index = _index; }

	private:
		int index;
		int size;
		unsigned int mask;

	};
}


