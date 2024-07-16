#include "PCSTree.h"
#include "PCSNode.h"

namespace Uncertain
{
	PCSTree::PCSTree()
		: pRoot(nullptr)
	{
		this->poForwardIT = new PCSTreeForwardIterator(nullptr);
		this->poReverseIT = new PCSTreeReverseIterator(nullptr);
	}

	PCSTree::~PCSTree()
	{
		this->poReverseIT->Reset(this->pRoot);
		
		PCSNode* pCur = this->poReverseIT->CurrentItem();
		PCSNode* pTemp = nullptr;

		while (!this->poReverseIT->IsDone())
		{
			pTemp = this->poReverseIT->Next();
			delete pCur;
			pCur = pTemp;
		}

		this->pRoot = nullptr;

		delete this->poForwardIT;
		delete this->poReverseIT;
	}

	PCSNode * PCSTree::GetRoot() const
	{
		return this->pRoot;
	}

	PCSTreeForwardIterator* PCSTree::GetForwardIT()
	{
		return (PCSTreeForwardIterator*)this->poForwardIT->Reset(this->pRoot);
	}

	PCSTreeReverseIterator* PCSTree::GetReverseIT()
	{
		return (PCSTreeReverseIterator*)this->poReverseIT->Reset(this->pRoot);
	}

	void PCSTree::Insert(PCSNode * const pInNode, PCSNode * const pParent)
	{
		if (pInNode != nullptr)
		{
			pInNode->SetParent(pParent);

			if (pParent != nullptr)
			{
				PCSNode* pChild = pParent->GetChild();
				pParent->SetChild(pInNode);
				
				pInNode->SetNextSibling(pChild);
				pInNode->SetPrevSibling(nullptr);

				if (pChild != nullptr)
				{
					pChild->SetPrevSibling(pInNode);
				}
				
				pParent->SetForward(pInNode);
				pInNode->SetReverse(pParent);
				pInNode->SetForward();

			}
			// first node
			else
			{
				this->pRoot = pInNode;
			}
		}
	}

	void PCSTree::Remove(PCSNode * const pInNode)
	{
		if (pInNode != nullptr)
		{
			PCSNode* pReverse = nullptr;

			if (this->pRoot == pInNode)
			{
				this->pRoot = nullptr;
			}
			else
			{
				pReverse = pInNode->GetReverse();
				
				if (pInNode->GetNextSibling() != nullptr)
				{
					pInNode->GetNextSibling()->SetPrevSibling(pInNode->GetPrevSibling());
				}
				if (pInNode->GetPrevSibling() != nullptr)
				{
					pInNode->GetPrevSibling()->SetNextSibling(pInNode->GetNextSibling());
				}
				else
				{
					pInNode->GetParent()->SetChild(pInNode->GetNextSibling());
				}

				pReverse->SetForward();
				if (pReverse->GetForward() != nullptr)
				{
					pReverse->GetForward()->SetReverse(pReverse);
				}
			}

			this->WashChildren(*pInNode);
		}
	}

	void PCSTree::WashChildren(PCSNode& pNode)
	{
		if (pNode.GetChild() != nullptr)
		{
			PCSNode* pCur = pNode.GetChild();
			PCSNode* pTemp = nullptr;

			while (pCur != nullptr)
			{
				pTemp = pCur->GetNextSibling();

				if (pCur->GetChild() != nullptr)
				{
					WashChildren(*pCur);
				}

				pCur->Wash();

				pCur = pTemp;
			}
		}

		pNode.Wash();
	}

	void PCSTree::GetInfo(Info & info)
	{
		info.Get_Initialize(this->mInfo);
		
		PCSNode* pCur = this->pRoot;
		PCSNode* pParent = nullptr;

		// empty tree
		if (pCur == nullptr)
		{
			return;
		}

		// root node exists
		info.currNumLevels = 1;
		int curLevel = info.currNumLevels;

		// flag for moving level up in walk
		bool upALevel = false;

		while (pCur != nullptr)
		{
			upALevel = false;
			++info.currNumNodes;

			pParent = pCur;
			pCur = pCur->GetChild();

			// No child, get sibling
			if (pCur == nullptr)
			{
				// keep going up until next sibling has been found
				while (pParent != nullptr && pCur == nullptr)
				{
					pCur = pParent->GetNextSibling();
					pParent = pParent->GetParent();

					// if we've gone up twice
					if (upALevel == true)
					{
						// check for new max-current level before going up
						if (curLevel > info.currNumLevels)
						{
							info.currNumLevels = curLevel;
						}
						--curLevel;
					}
					upALevel = true;
				}
			}
			else
			{
				++curLevel;
			}
		}

		// set new maxes
		if (info.currNumNodes > info.maxNumNodes)
		{
			info.maxNumNodes = info.currNumNodes;
		}
		if (info.currNumLevels > info.maxNumLevels)
		{
			info.maxNumLevels = info.currNumLevels;
		}

		this->mInfo = info;
	}

	void PCSTree::Print() const
	{
		PCSNode* pCur = this->pRoot;

		if (pCur == nullptr)
		{
			Trace::out("----- Empty -------\n\n");
		}
		else
		{
			char nameBuffer[PCSNode::NAME_SIZE];
			pCur->GetName(nameBuffer, PCSNode::NAME_SIZE);

			Trace::out("- Root -- %s (0x%p)\n", nameBuffer, pCur);

			pCur = pCur->GetChild();

			int curLevel = 1;
			bool upALevel = false;
			char parentNameBuf[PCSNode::NAME_SIZE];

			PCSNode* pParent = pCur->GetParent();
			PCSNode* pLastParent = nullptr;

			while (pCur != nullptr)
			{
				// Clear buffers
				memset(nameBuffer, 0x0, PCSNode::NAME_SIZE);
				
				pCur->GetName(nameBuffer, PCSNode::NAME_SIZE);
				if (pLastParent != pParent)
				{
					pParent->GetName(parentNameBuf, PCSNode::NAME_SIZE);
					pLastParent = pParent;
				}

				// Print pCur
				for (int i = 0; i < curLevel + 1; i++)
				{
					Trace::out("-");
				}
				Trace::out("%s (0x%p) --- Parent: %s\n", nameBuffer, pCur, parentNameBuf);

				pParent = pCur;
				pCur = pCur->GetChild();

				upALevel = false;

				if (pCur == nullptr)
				{
					while (pParent != nullptr && pCur == nullptr)
					{
						pCur = pParent->GetNextSibling();
						pParent = pParent->GetParent();

						if (upALevel == true)
						{
							--curLevel;
						}
						upALevel = true;
					}
				}
				else
				{
					++curLevel;
				}

			}

			Trace::out("------------------------------\n");
		}
	}

}

// ---  End of File ---
