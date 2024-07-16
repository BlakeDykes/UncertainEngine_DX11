#pragma once

#include "PCSTreeIterator.h"

namespace Uncertain
{
	class PCSTreeReverseIterator : public PCSTreeIterator
	{
	public:

		PCSTreeReverseIterator(PCSNode *rootNode);

		virtual PCSNode *First() override;
		virtual PCSNode *Next() override;
		virtual bool IsDone() override;
		virtual PCSNode *CurrentItem() override;
		virtual PCSTreeIterator* Reset(PCSNode* _pRoot) override;

	private:
		PCSNode * pRoot;
		PCSNode * pCurrent;
		PCSNode* pTail;

		bool Done;
	};

}
