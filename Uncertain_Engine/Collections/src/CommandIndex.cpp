#include "CommandIndex.h"

namespace Uncertain
{
	CommandIndex::CommandIndex(int buffSize)
	{
		assert(buffSize > 1);

		this->size = buffSize;
		this->mask = (unsigned int)(buffSize - 1);
		this->index = 0;
	}

	int CommandIndex::operator++(int)
	{
		// plus one then mask for wrapping to front
		this->index = (this->index + 1) & (int)this->mask;

		return this->index;
	}

	bool CommandIndex::operator==(const CommandIndex& c)
	{
		assert(this->size == c.size);

		return this->index == c.index;
	}

	bool CommandIndex::operator!=(const CommandIndex& c)
	{
		assert(this->size == c.size);

		return this->index != c.index;
	}

	int CommandIndex::GetIndex() const
	{
		return this->index;
	}
}