#ifndef MGBINARYTREEH
#define MGBINARYTREEH

#include <stdlib.h>

/* Template based implementation of a binary tree. It is currently not a balanced tree; I doubt the search performance benefits would pay
	dividends here at the cost of the added code for automatically balancing it. The information that is intended to be stored in this tree is merely too
	small. It serves essentially two purposes at this point in time. The first is replacing mgLinkedList in the mgVisibilityMap code for something that
	can do lookups faster, and the second is to eliminate the need to do a "search" for a value before adding it. This meant iterating over the entire linked list
	each time an object was added. This takes longer, and longer, and longer. This tree will simply NOT add something if it finds it is already in the list,
	which it will do WHILE it is adding it.
		- Chris Laverdure
*/

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
	// We are the greatest and lessest that ever existed.... *ROCK GUITAR CHORD*
	Lesser = NULL;
	Greater = NULL;
}

template <typename TemplateObject>
mgBinaryTreenode<TemplateObject>::~mgBinaryTreenode()
{
	// We are being deleted, our lineage line cannot continue without us... DELETE OUR CHILDREN, BOTH THE BETTER, AND THE WORSE!
	if (Lesser != NULL)
		delete Lesser;
	if (Greater != NULL)
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

	mgBinaryTree();
	~mgBinaryTree();
};

template <typename TemplateObject>
void mgBinaryTree<TemplateObject>::AddElement(TemplateObject Element)
{
	if (Root == NULL)
	{	// This is the first object in the list, just add it to the root.
		Root = new mgBinaryTreenode < TemplateObject > ;
		Root->Greater = NULL;
		Root->Lesser = NULL;
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
				if (ProgressNode->Lesser == NULL) // This is our home.
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
				if (ProgressNode->Greater == NULL) // This is our home.
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

	if (TravelNode == NULL)
		return false; // Obviously, duuuh...

	while (1)
	{
		if (TravelNode->Element == Element)
			return true; // Found it!
		else if (TravelNode->Element > Element)
		{	// We're smaller than it, travel the lesser road.
			if (TravelNode->Lesser == NULL)
				return false; // It isn't here.
			else
				TravelNode = TravelNode->Lesser;
		}
		else
		{	// We must travel the greater road.
			if (TravelNode->Greater == NULL)
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
	Root = NULL;
	ElementCount = 0;
}

template <typename TemplateObject>
mgBinaryTree<TemplateObject>::~mgBinaryTree()
{
	if (Root != NULL)
		delete Root; // See the binarytreenode class and appreciate its self destructive behavior.
					// It cuts itselfs into deletion to the tune of Marilyn Manson.
}

#endif