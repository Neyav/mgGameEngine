#ifndef MGBINARYTREEH
#define MGBINARYTREEH

#define BINARYTREEDUMP // Build binary tree with capability to dump structure to a file.

#include <stdlib.h>
#ifdef BINARYTREEDUMP 
#include <math.h>
#include <iostream>
#include <fstream>
#endif

/* Template based implementation of a binary tree. It is currently not a balanced tree; I doubt the search performance benefits would pay
	dividends here at the cost of the added code for automatically balancing it. The information that is intended to be stored in this tree is merely too
	small. It serves essentially two purposes at this point in time. The first is replacing mgLinkedList in the mgVisibilityMap code for something that
	can do lookups faster, and the second is to eliminate the need to do a "search" for a value before adding it. This meant iterating over the entire linked list
	each time an object was added. This takes longer, and longer, and longer. This tree will simply NOT add something if it finds it is already in the list,
	which it will do WHILE it is adding it.
		- Chris Laverdure
*/

// TODO: Modify this component so it is a Red Black Tree instead of an unbalanced binary tree. Also remove the sass from the comments.
//		 I no longer find it funny. :p

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
private:
	mgBinaryTreenode<TemplateObject> *Root;
	unsigned int ElementCount;

public:

	void AddElement(TemplateObject Element);
	bool IsElementPresent(TemplateObject Element);
	unsigned int Elements(void);

#ifdef BINARYTREEDUMP
	void DumpTreeStructure(std::string OutputFile);
	int CalculateTreeHeight(int PassDepth, mgBinaryTreenode<TemplateObject> *TraversalNode);
#endif

	mgBinaryTree();
	~mgBinaryTree();
};

template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::AddElement(TemplateObject Element)
{
	if (Root == nullptr)
	{	// This is the first object in the list, just add it to the root.
		Root = new mgBinaryTreenode < TemplateObject > ;
		Root->Element = Element;
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

	TreeStructureFile.open(OutputFile);

	TreeStructureFile << "mgGameEngine -> void mgBinaryTree<TemplateObject>::DumpTreeStructure(std::string OutputFile)" << std::endl;
	TreeStructureFile << ":-- Tree has " << this->Elements() << " elements in it." << std::endl;

	// Calculate the maximum tree depth.
	TreeDepth = this->CalculateTreeHeight(1, Root);

	TreeStructureFile << ":-- Tree has maximum depth of " << TreeDepth << "." << std::endl;

	TreeStructureFile.close();
}
#endif

#endif