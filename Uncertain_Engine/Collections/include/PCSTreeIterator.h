#pragma once

#include "PCSNode.h"

namespace Uncertain
{
	class PCSTreeIterator
	{
	public:
		virtual PCSNode *First() = 0;
		virtual PCSNode *Next() = 0;
		virtual bool IsDone() = 0;
		virtual PCSNode *CurrentItem() = 0;
		virtual PCSTreeIterator* Reset(PCSNode* _pRoot) = 0;
	};
}
