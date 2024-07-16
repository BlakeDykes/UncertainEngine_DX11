#pragma once

#include "PCSTreeIterator.h"

namespace Uncertain
{
	class PCSTreeForwardIterator : public PCSTreeIterator
	{
	public:
		PCSTreeForwardIterator(PCSNode *rootNode);

		virtual PCSNode *First() override;
		virtual PCSNode *Next() override;
		virtual bool IsDone() override;
		virtual PCSNode *CurrentItem() override;
		virtual PCSTreeIterator* Reset(PCSNode* _pRoot) override;


	private:

		PCSNode* pRoot;
		PCSNode* pCurrent;
		bool Done;
	};

}
