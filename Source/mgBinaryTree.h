#ifndef MGBINARYTREEH
#define MGBINARYTREEH

/*
	Binary Tree Template

	TODO: This tree implementation is currently without delete functionality but that is planned for a future update.

	This is a rather simple but flexible and efficent implementation of a Binary Tree. It was converted into a Red Black Tree
	but for diagnostic purposes all the Red Black Tree code was seperated with the preprocessing flag "REDBLACKTREE". Since there
	is no runtime penalty to keep this distinction, and the trees are so similar that the code suffers very little added complexity
	for it, I have decided to keep it. If anything it serves as an easy way to distinguish which parts are used for the Red Black Tree
	and which parts are common to both.

	I personally found very few guides on the Internet that were actually useful in doing this. Most people seem to not get the
	common sense practice of naming variables after more than just letters. Plus I think left and right as node branches in this
	sense are a little nonsensical. z = y->p->p->left is significantly less readable than 
	FixupNode = ReferenceNode->Parent->Parent->Lesser in my opinion. You are free to disagree.

	Thing I like that I did: Made the rotates and fixup functions of the node class. It just made sense.
	Thing I didn't like that I did: The fact that the Root node's parent is nullptr, rather than the tree being based around
			a "placeholder" Root node forced me to carry the reference of the tree itself into the fixup and rotate functions
			into the node and then call on a function to rotate around the Root node. Feels messy. I'm sure there is a better
			way.

	- Chris Laverdure
*/

#define BINARYTREEDUMP // Build binary tree with capability to dump structure to a file.
#define REDBLACKTREE // Tree is a red black tree.

#include <stdlib.h>
#ifdef BINARYTREEDUMP 
#include <iostream>
#include <fstream>
#endif

#ifdef REDBLACKTREE
template <typename TemplateObject>
class mgBinaryTree;
#endif

template <typename TemplateObject>
class mgBinaryTreenode
{
private:
#ifdef REDBLACKTREE
	void leftRotate(mgBinaryTree<TemplateObject> *TreeReference);
	void rightRotate(mgBinaryTree<TemplateObject> *TreeReference);
#endif
public:
	TemplateObject Element;

#ifdef REDBLACKTREE
	bool BlackNode;

	void fixUp(mgBinaryTree<TemplateObject> *TreeReference);

	mgBinaryTreenode<TemplateObject> *Parent;
#endif
	mgBinaryTreenode<TemplateObject> *Greater;
	mgBinaryTreenode<TemplateObject> *Lesser;

	mgBinaryTreenode();
	~mgBinaryTreenode();
};

#ifdef REDBLACKTREE
template <typename TemplateObject>
void mgBinaryTreenode<TemplateObject>::leftRotate(mgBinaryTree<TemplateObject> *TreeReference)
{
	mgBinaryTreenode<TemplateObject> *ReferenceNode = this->Greater;

	this->Greater = ReferenceNode->Lesser;
	if (ReferenceNode->Lesser != nullptr)
		ReferenceNode->Lesser->Parent = this;
	ReferenceNode->Parent = this->Parent;

	if (this->Parent == nullptr)
		TreeReference->SwapRoot(ReferenceNode);
	else if (this == this->Parent->Lesser)
		this->Parent->Lesser = ReferenceNode;
	else
		this->Parent->Greater = ReferenceNode;

	ReferenceNode->Lesser = this;
	this->Parent = ReferenceNode;
}

template <typename TemplateObject>
void mgBinaryTreenode<TemplateObject>::rightRotate(mgBinaryTree<TemplateObject> *TreeReference)
{
	mgBinaryTreenode<TemplateObject> *ReferenceNode = this->Lesser;

	this->Lesser = ReferenceNode->Greater;
	if (ReferenceNode->Greater != nullptr)
		ReferenceNode->Greater->Parent = this;
	ReferenceNode->Parent = this->Parent;

	if (this->Parent == nullptr)
		TreeReference->SwapRoot(ReferenceNode);
	else if (this == this->Parent->Greater)
		this->Parent->Greater = ReferenceNode;
	else
		this->Parent->Lesser = ReferenceNode;

	ReferenceNode->Greater = this;
	this->Parent = ReferenceNode;
}

template <typename TemplateObject>
void mgBinaryTreenode<TemplateObject>::fixUp(mgBinaryTree<TemplateObject> *TreeReference)
{
	mgBinaryTreenode<TemplateObject> *FixupNode = this;
	// We just added an element and need to perform the necessary steps to fix the balance of the tree
	while (FixupNode->Parent != nullptr && !FixupNode->Parent->BlackNode)
	{ // Our parent is a Red Node
		if (FixupNode->Parent == FixupNode->Parent->Parent->Lesser)
		{
			mgBinaryTreenode<TemplateObject> *ReferenceNode = FixupNode->Parent->Parent->Greater;

			if (ReferenceNode != nullptr && !ReferenceNode->BlackNode)
			{
				FixupNode->Parent->BlackNode = true;
				ReferenceNode->BlackNode = true;
				FixupNode->Parent->Parent->BlackNode = false;
				FixupNode = FixupNode->Parent->Parent;
			}
			else
			{
				if (FixupNode == FixupNode->Parent->Greater)
				{
					FixupNode = FixupNode->Parent;
					FixupNode->leftRotate(TreeReference);
				}

				FixupNode->Parent->BlackNode = true;
				FixupNode->Parent->Parent->BlackNode = false;

				FixupNode->Parent->Parent->rightRotate(TreeReference);
			}
		}
		else
		{
			mgBinaryTreenode<TemplateObject> *ReferenceNode = FixupNode->Parent->Parent->Lesser;

			if (ReferenceNode != nullptr && !ReferenceNode->BlackNode)
			{
				FixupNode->Parent->BlackNode = true;
				ReferenceNode->BlackNode = true;
				FixupNode->Parent->Parent->BlackNode = false;
				FixupNode = FixupNode->Parent->Parent;
			}
			else
			{
				if (FixupNode == FixupNode->Parent->Lesser)
				{
					FixupNode = FixupNode->Parent;
					FixupNode->rightRotate(TreeReference);
				}
				FixupNode->Parent->BlackNode = true;
				FixupNode->Parent->Parent->BlackNode = false;
				FixupNode->Parent->Parent->leftRotate(TreeReference);
			}
		}
	}
}
#endif

template <typename TemplateObject>
mgBinaryTreenode<TemplateObject>::mgBinaryTreenode()
{
#ifdef REDBLACKTREE
	BlackNode = false; // Nodes default to Red

	Parent = nullptr;
#endif
	// New nodes don't have children, so as default behavior this makes a lot of sense.
	Lesser = nullptr;
	Greater = nullptr;
}

template <typename TemplateObject>
mgBinaryTreenode<TemplateObject>::~mgBinaryTreenode()
{
	/* Deleting a node results in its children being deleted. Supporting individual node deletion is planned
	   and that means this is going to be replaced with code that delinks this node from the tree safely rather than the
	   destruction of its children */
	if (Lesser != nullptr)
		delete Lesser;
	if (Greater != nullptr)
		delete Greater;
}

template <typename TemplateObject> 
class mgBinaryTree
{
private:
	mgBinaryTreenode<TemplateObject> *Root;
	unsigned int ElementCount;

public:

#ifdef REDBLACKTREE
	void SwapRoot(mgBinaryTreenode<TemplateObject> *NewRoot);
#endif

	void AddElement(TemplateObject Element);
	bool IsElementPresent(TemplateObject Element);
	unsigned int Elements(void);

#ifdef BINARYTREEDUMP
	unsigned int DepthValues[1000];

	void DumpTreeStructure(std::string OutputFile);
	int CalculateTreeHeight(int PassDepth, mgBinaryTreenode<TemplateObject> *TraversalNode);
#endif

	mgBinaryTree();
	~mgBinaryTree();
};

#ifdef REDBLACKTREE
template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::SwapRoot(mgBinaryTreenode<TemplateObject> *NewRoot)
{
	Root = NewRoot;
}
#endif

template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::AddElement(TemplateObject Element)
{
	if (Root == nullptr)
	{	// This is the first object in the list, just add it to the root.
		Root = new mgBinaryTreenode < TemplateObject > ;
		Root->Element = Element;
#ifdef REDBLACKTREE
		Root->BlackNode = true; // This is all the fixing we need for a blank tree.
#endif
	}
	else
	{	// Find a node we belong in, bailing if we find a DOPPLEGANGER!@!!!@@@##!
		mgBinaryTreenode<TemplateObject> *ProgressNode = Root; // Start at the root.
		while (1) // 1 is always 1.....
		{
			if (ProgressNode->Element == Element) // DOPPLEGANGER2#$@#@#
				break;
			else if (ProgressNode->Element > Element) // We are small and humble. :(
			{
				if (ProgressNode->Lesser == nullptr) // This is our home.
				{
					ProgressNode->Lesser = new mgBinaryTreenode < TemplateObject > ;
					ProgressNode->Lesser->Element = Element;
					ElementCount++;

#ifdef REDBLACKTREE
					ProgressNode->Lesser->Parent = ProgressNode;
					ProgressNode->Lesser->fixUp(this);
					Root->BlackNode = true;
#endif

					break;
				}
				else
					ProgressNode = ProgressNode->Lesser; // Go down the tree....
			}
			else // We must BE GREATER THAN WE THOUGHT!!!!!!
			{
				if (ProgressNode->Greater == nullptr) // This is our home.
				{
					ProgressNode->Greater = new mgBinaryTreenode < TemplateObject >;
					ProgressNode->Greater->Element = Element;
					ElementCount++;

#ifdef REDBLACKTREE
					ProgressNode->Greater->Parent = ProgressNode;
					ProgressNode->Greater->fixUp(this);
					Root->BlackNode = true;
#endif
					break;
				}
				else
					ProgressNode = ProgressNode->Greater; // Go down the tree....
			}
		} // END of while loop
	}
}

template <typename TemplateObject>
bool mgBinaryTree<TemplateObject>::IsElementPresent(TemplateObject Element)
{
	mgBinaryTreenode < TemplateObject > *TravelNode = Root;

	if (TravelNode == nullptr)
		return false; // Obviously, duuuh...

	while (1)
	{
		if (TravelNode->Element == Element)
			return true; // Found it!
		else if (TravelNode->Element > Element)
		{	// We're smaller than it, travel the lesser road.
			if (TravelNode->Lesser == nullptr)
				return false; // It isn't here.
			else
				TravelNode = TravelNode->Lesser;
		}
		else
		{	// We must travel the greater road.
			if (TravelNode->Greater == nullptr)
				return false;
			else
				TravelNode = TravelNode->Greater;
		}
	}
}

template <typename TemplateObject>
unsigned int mgBinaryTree<TemplateObject>::Elements(void)
{
	return ElementCount;
}

template <typename TemplateObject>
mgBinaryTree<TemplateObject>::mgBinaryTree()
{
	Root = nullptr;
	ElementCount = 0;
}

template <typename TemplateObject>
mgBinaryTree<TemplateObject>::~mgBinaryTree()
{
	if (Root != nullptr)
		delete Root; // See the binarytreenode class and appreciate its self destructive behavior.
}

#ifdef BINARYTREEDUMP

template <typename TemplateObject>
int mgBinaryTree<TemplateObject>::CalculateTreeHeight(int PassDepth, mgBinaryTreenode<TemplateObject> *TraversalNode)
{
	int TotalDepth = PassDepth;
	int TempDepth = 0;

	if (TraversalNode == Root)
		TotalDepth = PassDepth = 1;

	DepthValues[PassDepth]++; // Keep track of the number of nodes per depth;

	if (TraversalNode->Lesser != NULL)
		TotalDepth = this->CalculateTreeHeight(PassDepth + 1, TraversalNode->Lesser);
	if (TraversalNode->Greater != NULL)
		TempDepth = this->CalculateTreeHeight(PassDepth + 1, TraversalNode->Greater);

	if (TempDepth > TotalDepth)
		return TempDepth;
	else
		return TotalDepth;
}

template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::DumpTreeStructure(std::string OutputFile)
{
	std::ofstream TreeStructureFile;
	unsigned int TreeDepth = 0;

	// Initalize this field.
	for (int Iterator = 0; Iterator < 1000; Iterator++)
		DepthValues[Iterator] = 0;

	TreeStructureFile.open(OutputFile);

	TreeStructureFile << "mgGameEngine -> void mgBinaryTree<TemplateObject>::DumpTreeStructure(std::string OutputFile)" << std::endl;
#ifdef REDBLACKTREE
	TreeStructureFile << ":-- mgBinaryTree was compiled as a Red Black Tree." << std::endl;
#endif
	TreeStructureFile << ":-- Tree has " << this->Elements() << " elements in it." << std::endl;

	// Calculate the maximum tree depth.
	TreeDepth = this->CalculateTreeHeight(1, Root);

	TreeStructureFile << ":-- Tree has maximum depth of " << TreeDepth << "." << std::endl;
	TreeStructureFile << ":----v" << std::endl;

	for (int Iterator = 1; Iterator < TreeDepth; Iterator++)
	{
		TreeStructureFile << "     |-> Depth: " << Iterator << " has " << DepthValues[Iterator] << " nodes." << std::endl;
	}

	TreeStructureFile.close();
}
#endif

#endif