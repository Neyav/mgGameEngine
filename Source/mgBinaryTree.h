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
struct mgBinaryTreenode
{
	TemplateObject Element;

	mgBinaryTreenode<TemplateObject> *Greater;
	mgBinaryTreenode<TemplateObject> *Lesser;
};

template <typename TemplateObject> 
class mgBinaryTree
{
private:
	mgBinaryTreenode<TemplateObject> *Root;
public:

	void AddElement(TemplateObject Element);
	bool IsElementPresent(TemplateObject Element);

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
				continue;
			else if (ProgressNode->Element > Element) // We are small and humble. :(
			{
				if (ProgressNode->Lesser == NULL) // This is our home.
				{
					ProgressNode->Lesser = new mgBinaryTreenode < TemplateObject > ;
					ProgressNode->Lesser->Greater = NULL;
					ProgressNode->Lesser->Lesser = NULL; // philosophy..... dude....
					ProgressNode->Lesser->Element = Element;
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
					ProgressNode->Greater->Greater = NULL; // Bummer.....
					ProgressNode->Greater->Lesser = NULL; 
					ProgressNode->Greater->Element = Element;
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

}
#endif