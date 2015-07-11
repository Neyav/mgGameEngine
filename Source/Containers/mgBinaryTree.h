#ifndef MGBINARYTREEH
#define MGBINARYTREEH

/*
	Binary Tree Template

	Supports AVL tree through the polymorphism template mgAVLBinaryTree and RBT through mgRBTBinaryTree

	TODO: This tree implementation is currently without delete functionality but that is planned for a future update.

	This is a rather simple but flexible and efficent implementation of a Binary Tree. It is expanded into a self-balancing
	tree through two different methods that inherent its basic properties and expand upon them for their specific purposes.
*/

#define BINARYTREEDUMP // Build binary tree with capability to dump structure to a file.
#define REBALANCETREE // Build binary tree with the capability to rebalance itself.

#include <stdlib.h>
#ifdef BINARYTREEDUMP
#include <iostream>
#include <fstream>
#endif
#ifdef REBALANCETREE
#include "mgSortedList.h"
#endif

template <typename TemplateObject>
class mgBinaryTreenode
{
public:
	TemplateObject Element;

	mgBinaryTreenode<TemplateObject> *Greater;
	mgBinaryTreenode<TemplateObject> *Lesser;

	mgBinaryTreenode();
	~mgBinaryTreenode();
};

template <typename TemplateObject>
mgBinaryTreenode<TemplateObject>::mgBinaryTreenode()
{
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
public:
	mgBinaryTreenode<TemplateObject> *Root;
	unsigned int ElementCount;
	std::string Definition;

#ifdef REBALANCETREE
	mgSortedList<TemplateObject> BinaryTreeList;

	void SplitListSection(int LeftBorder, int RightBorder);
	void ReBalanceTree(void);
#endif

	void NodeToList(mgBinaryTreenode<TemplateObject> *Node, mgLinkedList<TemplateObject> *ListReference);

	virtual void AddElement(TemplateObject Element);
	bool IsElementPresent(TemplateObject Element);
	unsigned int Elements(void);

	void ClearTree(void);

#ifdef BINARYTREEDUMP
	unsigned int DepthValues[1000];

	void DumpTreeStructure(std::string OutputFile);
	int CalculateTreeHeight(int PassDepth, mgBinaryTreenode<TemplateObject> *TraversalNode);
#endif

	mgBinaryTree();
	~mgBinaryTree();
};

template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::NodeToList(mgBinaryTreenode<TemplateObject> *Node, mgLinkedList<TemplateObject> *ListReference)
{
	if (Node == nullptr)
		return; // unwind

	ListReference->AddElement(Node->Element); // Add a copy of the element to our sorted list.

	this->NodeToList(Node->Greater, ListReference);
	this->NodeToList(Node->Lesser, ListReference);
}

#ifdef REBALANCETREE
template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::SplitListSection(int LeftBorder, int RightBorder)
{
	int MiddleEntry;
	
	// Find the middle of these entries.
	MiddleEntry = (RightBorder - LeftBorder) / 2 + LeftBorder;

	this->AddElement(*this->BinaryTreeList[MiddleEntry]); // Add the middle element to the tree.

	if (LeftBorder == RightBorder)
		return; // We've divided it as much as we can

	// Split the remaining
	if (MiddleEntry != LeftBorder)
		this->SplitListSection(LeftBorder, MiddleEntry - 1);
	if (MiddleEntry != RightBorder)
		this->SplitListSection(MiddleEntry + 1, RightBorder);
}
template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::ReBalanceTree(void)
{
	this->BinaryTreeList.ClearList(); // Make sure our list is empty before we begin adding things to it.

	this->NodeToList(Root, &this->BinaryTreeList); // Recursively dump the entire tree to our sorted list.

	if (this->Root != nullptr)
		delete this->Root; // Kill the tree.
	
	this->Root = nullptr;
	this->ElementCount = 0;

	// This is where we rebuild the tree in the optimal order.
	int ListSize = this->BinaryTreeList.NumberOfElements();
	
	if (ListSize == 0)
		return; // There is nothing to balance.

	this->SplitListSection(0, ListSize - 1);

	this->BinaryTreeList.ClearList(); // Destroy the list.
}
#endif

template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::AddElement(TemplateObject Element)
{
	if (Root == nullptr)
	{	// This is the first object in the list, just add it to the root.
		Root = new mgBinaryTreenode < TemplateObject > ;
		Root->Element = Element;

		ElementCount++;
	}
	else
	{	// Find the proper leaf we need to branch off of....
		mgBinaryTreenode<TemplateObject> *ProgressNode = Root; // Start at the root.
		while (1) // 1 is always 1.....
		{
			if (ProgressNode->Element == Element) // We found ourselves in the tree. There is nothing more to do here.
				break;
			else if (ProgressNode->Element > Element) // We are smaller than this branch node.
			{
				if (ProgressNode->Lesser == nullptr) // We belong here.....
				{
					ProgressNode->Lesser = new mgBinaryTreenode < TemplateObject > ;
					ProgressNode->Lesser->Element = Element;
					ElementCount++;

					break;
				}
				else
					ProgressNode = ProgressNode->Lesser; // Proceed down the next branch of the tree...
			}
			else // We are greater than this branch node.
			{
				if (ProgressNode->Greater == nullptr) // We belong here.
				{
					ProgressNode->Greater = new mgBinaryTreenode < TemplateObject >;
					ProgressNode->Greater->Element = Element;
					ElementCount++;

					break;
				}
				else
					ProgressNode = ProgressNode->Greater; // Proceed down the next branch of the tree....
			}
		} // END of while loop
	}
}

template <typename TemplateObject>
bool mgBinaryTree<TemplateObject>::IsElementPresent(TemplateObject Element)
{
	mgBinaryTreenode < TemplateObject > *TravelNode = Root;

	if (TravelNode == nullptr)
		return false; // This is a blank tree. It can't be present.

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
void mgBinaryTree<TemplateObject>::ClearTree(void)
{
	if (Root != nullptr)
		delete Root;

	this->ElementCount = 0;
}

template <typename TemplateObject>
mgBinaryTree<TemplateObject>::mgBinaryTree()
{
	Definition = "Unbalanced Binary Tree";

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

	if (TraversalNode->Lesser != nullptr)
		TotalDepth = this->CalculateTreeHeight(PassDepth + 1, TraversalNode->Lesser);
	if (TraversalNode->Greater != nullptr)
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
	unsigned int ExpectedNodes = 1;
	unsigned int UnbalancedNodes = 0;
	unsigned int CountedElements = 0;

	// Initalize this field.
	for (int Iterator = 0; Iterator < 1000; Iterator++)
		DepthValues[Iterator] = 0;

	TreeStructureFile.open(OutputFile);

	TreeStructureFile << "mgGameEngine -> void mgBinaryTree<TemplateObject>::DumpTreeStructure(std::string OutputFile)" << std::endl;
	TreeStructureFile << ":-- Tree identifies as: " << Definition.c_str() << std::endl;
	TreeStructureFile << ":-- Tree has " << this->Elements() << " elements in it." << std::endl;

	// Calculate the maximum tree depth.
	TreeDepth = this->CalculateTreeHeight(1, Root);

	TreeStructureFile << ":-- Tree has maximum depth of " << TreeDepth << "." << std::endl;
	TreeStructureFile << ":----v" << std::endl;

	for (unsigned int Iterator = 1; Iterator <= TreeDepth; Iterator++)
	{
		TreeStructureFile << "     |-> Depth: " << Iterator << " has " << DepthValues[Iterator] << " nodes. ";
		if (ExpectedNodes > 0 && DepthValues[Iterator] != ExpectedNodes)
		{
			ExpectedNodes = 0;
			TreeStructureFile << "~[Balanced Tree Ends Here]~";
		}
		else if (ExpectedNodes == 0)
			UnbalancedNodes++;
		TreeStructureFile << std::endl;
		ExpectedNodes *= 2;
		CountedElements += DepthValues[Iterator];
	}

	TreeStructureFile << ":----^" << std::endl;
	TreeStructureFile << ":-- Binary Tree has " << UnbalancedNodes << " Unbalanced layers." << std::endl;
	if (CountedElements == ElementCount)
		TreeStructureFile << ":--(Sanity: Pass) Binary Tree is accurate on elements present." << std::endl;
	else
		TreeStructureFile << ":--(Sanity: Fail) Binary Tree count is actually " << CountedElements << ", not " << ElementCount << "!!!!!" << std::endl;

	TreeStructureFile.close();
}
#endif

#endif
