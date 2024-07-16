#pragma once

#include "PCSTreeReverseIterator.h"
#include "PCSTreeForwardIterator.h"

namespace Uncertain
{
	// forward declare
	class PCSNode;

	// PCSTree class 
	class PCSTree
	{
	public:
		// Simple data structure
		struct Info
		{
			int currNumNodes;
			int maxNumNodes;
			int currNumLevels;
			int maxNumLevels;

			Info()
				: currNumNodes(0), maxNumNodes(0), 
				currNumLevels(0), maxNumLevels(0)
			{
			}

			void Get_Initialize(const Info& info)
			{
				this->currNumNodes = 0; 
				this->maxNumNodes = info.maxNumNodes;
				this->currNumLevels = 0;
				this->maxNumLevels = info.maxNumLevels;
			}
		};

	public:

		PCSTree();
		PCSTree(const PCSTree &in) = delete;
		PCSTree &operator = (const PCSTree &in) = delete;
		~PCSTree();

		void Insert(PCSNode *const pInNode, PCSNode *const pParent);
		void Remove(PCSNode *const pInNode);

		PCSNode* GetRoot() const;
		PCSTreeForwardIterator* GetForwardIT();
		PCSTreeReverseIterator* GetReverseIT();
		void GetInfo(Info &info);

		void Print() const;

	protected:
		void WashChildren(PCSNode& pNode);

		PCSTreeForwardIterator* poForwardIT;
		PCSTreeReverseIterator* poReverseIT;

		// Data
		Info	mInfo;
		PCSNode *pRoot;


	};

}
