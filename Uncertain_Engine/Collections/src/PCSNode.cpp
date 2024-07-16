#include "PCSTree.h"
#include "PCSNode.h"

#include <cstring>

namespace Uncertain
{
	// constructor
	PCSNode::PCSNode()
		: pParent(nullptr), pChild(nullptr), pNextSibling(nullptr),
		pPrevSibling(nullptr), pForward(nullptr), pReverse(nullptr)
	{
		memset(this->pName, 0x0, PCSNode::NAME_SIZE);
	}

	// copy constructor
	PCSNode::PCSNode(const PCSNode & in)
	{
		this->pParent = in.pParent;
		this->pChild = in.pChild;
		this->pNextSibling = in.pNextSibling;
		this->pPrevSibling = in.pPrevSibling;
		this->pForward = in.pForward;
		this->pReverse = in.pReverse;
		memcpy(this->pName, in.pName, PCSNode::NAME_SIZE);
	}

	// Specialize Constructor
	PCSNode::PCSNode(PCSNode * const pInParent, PCSNode * const pInChild, PCSNode * const pInNextSibling, PCSNode * const pInPrevSibling, const char * const pInName)
		: pParent(pInParent), pChild(pInChild), pNextSibling(pInNextSibling),
		pPrevSibling(pInPrevSibling), pForward(nullptr), pReverse(nullptr)
	{
		memcpy(this->pName, pInName, PCSNode::NAME_SIZE);
	}

	PCSNode::PCSNode(const char * const pInName)
		: pParent(nullptr), pChild(nullptr), pNextSibling(nullptr),
		pPrevSibling(nullptr), pForward(nullptr), pReverse(nullptr)
	{
		memcpy(this->pName, pInName, PCSNode::NAME_SIZE);
	}

	// destructor
	PCSNode::~PCSNode()
	{
	}

	// assignment operator
	PCSNode & PCSNode::operator = (const PCSNode & in)
	{
		this->pParent = in.pParent;
		this->pChild = in.pChild;
		this->pNextSibling = in.pNextSibling;
		this->pPrevSibling = in.pPrevSibling;
		this->pForward = in.pForward;
		this->pReverse = in.pReverse;
		memcpy(this->pName, in.pName, PCSNode::NAME_SIZE);

		return *this;
	}

	void PCSNode::SetParent(PCSNode * const pIn)
	{
		this->pParent = pIn;
	}

	void PCSNode::SetChild(PCSNode * const pIn)
	{
		this->pChild = pIn;
	}

	void PCSNode::SetNextSibling(PCSNode * const pIn)
	{
		this->pNextSibling = pIn;
	}

	void PCSNode::SetPrevSibling(PCSNode * const pIn)
	{
		this->pPrevSibling = pIn;
	}

	void PCSNode::SetForward(PCSNode* const pIn)
	{
		this->pForward = pIn;
	}

	void PCSNode::SetForward()
	{
		if (this->pChild != nullptr)
		{
			this->pForward = this->pChild;
			this->pChild->pReverse = this;
		}
		else if (this->pNextSibling != nullptr)
		{
			this->pForward = this->pNextSibling;
			this->pNextSibling->pReverse = this;
		}
		else
		{
			PCSNode* pCurParent = this->pParent;

			if (pCurParent != nullptr)
			{
				PCSNode* pNext = pCurParent->pNextSibling;
				while (pCurParent != nullptr && pNext == nullptr)
				{
					pCurParent = pCurParent->pParent;
					if (pCurParent != nullptr)
					{
						pNext = pCurParent->pNextSibling;
					}
				}

				this->pForward = pNext;
				
				if (pNext != nullptr)
				{
					pNext->pReverse = this;
				}
			}
		}
	}

	void PCSNode::SetReverse(PCSNode* const pIn)
	{
		this->pReverse = pIn;
	}

	PCSNode * PCSNode::GetParent(void) const
	{
		return this->pParent;
	}

	PCSNode * PCSNode::GetChild(void) const
	{
		return this->pChild;
	}

	PCSNode * PCSNode::GetNextSibling(void) const
	{
		return this->pNextSibling;
	}

	PCSNode * PCSNode::GetPrevSibling(void) const
	{
		return this->pPrevSibling;
	}

	PCSNode* PCSNode::GetForward(void) const
	{
		return this->pForward;
	}

	PCSNode* PCSNode::GetReverse(void) const
	{
		return this->pReverse;
	}

	PCSNode::Code PCSNode::SetName(const char * const pInName)
	{
		PCSNode::Code retStatus = PCSNode::Code::FAIL_NULL_PTR;

		if (pInName != nullptr)
		{
			size_t count = strlen(pInName) > PCSNode::NAME_SIZE ? PCSNode::NAME_SIZE - 1 : strlen(pInName);
			memcpy(this->pName, pInName, count);
			retStatus = PCSNode::Code::SUCCESS;
		}

		return retStatus;
	}

	PCSNode::Code PCSNode::GetName(char * const pOutBuffer, unsigned int sizeOutBuffer) const
	{
		PCSNode::Code retStatus = PCSNode::Code::FAIL_NULL_PTR;

		if (pOutBuffer != nullptr)
		{
			size_t count = sizeOutBuffer > PCSNode::NAME_SIZE ? PCSNode::NAME_SIZE - 1 : sizeOutBuffer - 1;
			memcpy(pOutBuffer, &this->pName[0], count);
			memset(&pOutBuffer[sizeOutBuffer - 1], 0x0, 1);

			retStatus = PCSNode::Code::SUCCESS;
		}

		return retStatus;
	}

	void PCSNode::PrintNode() const
	{
		const size_t bufferSize = PCSNode::NAME_SIZE + 13;
		char childString[bufferSize];
		char parentString[bufferSize];
		char nextSibString[bufferSize];
		char prevSibString[bufferSize];

		// Parent
		if (this->pParent == nullptr)
		{
			sprintf_s(parentString, bufferSize, "nullptr");
		}
		else
		{
			sprintf_s(parentString, bufferSize, "%s (0x%p)", this->pParent->pName, this->pParent);
		}

		// Child
		if (this->pChild == nullptr)
		{
			sprintf_s(childString, bufferSize, "nullptr");
		}
		else
		{
			sprintf_s(childString, bufferSize, "%s (0x%p)", this->pChild->pName, this->pChild);
		}
		// Next sibling
		if (this->pNextSibling == nullptr)
		{
			sprintf_s(nextSibString, bufferSize, "nullptr");
		}
		else
		{
			sprintf_s(nextSibString, bufferSize, "%s (0x%p)", this->pNextSibling->pName, this->pNextSibling);
		}
		// Prev sibling
		if (this->pPrevSibling == nullptr)
		{
			sprintf_s(prevSibString, bufferSize, "nullptr");
		}
		else
		{
			sprintf_s(prevSibString, bufferSize, "%s (0x%p)", this->pPrevSibling->pName, this->pPrevSibling);
		}
	
		Trace::out("\n- %s -----------------------------------\n", this->pName);
		Trace::out("------------- Parent -- %s\n", parentString);
		Trace::out("-------------- Child -- %s\n", childString);
		Trace::out("------- Next Sibling -- %s\n", nextSibString);
		Trace::out("--- Previous Sibling -- %s\n\n", prevSibString);

	}

	void PCSNode::PrintChildren() const
	{
		Trace::out("\n- %s: Children ------------------------\n", this->pName);

		PCSNode* pCur = this->pChild;

		if (pCur == nullptr)
		{
			Trace::out("--- None");
		}
		else
		{
			while (pCur != nullptr)
			{
				Trace::out("--- %s (0x%p)\n", pCur->pName, pCur);
				pCur = pCur->pNextSibling;
			}
			Trace::out("\n");
		}
	}

	void PCSNode::PrintSiblings() const
	{
		Trace::out("\n- %s: Siblings ------------------------\n", this->pName);
		
		PCSNode* pCur = this->pParent;

		if (pCur == nullptr)
		{
			Trace::out("--- None -- Root Node\n");
		}
		else
		{
			pCur = pCur->pChild;

			while (pCur != nullptr)
			{
				if (pCur == this)
				{
					Trace::out("---");
				}
				Trace::out("--- %s (0x%p)\n", pCur->pName, pCur);
				pCur = pCur->pNextSibling;
			}
			Trace::out("\n");
		}
	}

	int PCSNode::GetNumSiblings() const
	{
		int retVal = 1;

		PCSNode* pSib = this->pParent;

		if (pSib != nullptr)
		{
			pSib = pSib->pChild;

			while (pSib->GetNextSibling() != nullptr)
			{
				++retVal;
				pSib = pSib->GetNextSibling();
			}
		}

		return retVal;
	}

	int PCSNode::GetNumChildren() const
	{
		int retVal = 0;

		PCSNode* pCur = this->pChild;

		while (pCur != nullptr)
		{
			++retVal;
			pCur = pCur->pNextSibling;
		}

		return retVal;
	}

	void PCSNode::Wash()
	{
		this->pParent = nullptr;
		this->pChild = nullptr;
		this->pNextSibling = nullptr;
		this->pPrevSibling = nullptr;
		this->pForward = nullptr;
		this->pReverse = nullptr;
		memset(this->pName, 0x0, PCSNode::NAME_SIZE);
	}
}

// ---  End of File ---
