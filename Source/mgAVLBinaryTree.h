#ifndef MGAVLBINARYTREEH
#define MGAVLBINARYTREEH

#include "mgBinaryTree.h"

/*
	AVL Binary Tree Template

	This code expands upon the mgBinaryTree class adding AVL auto balancing to it.

	Special thanks to http://www.geeksforgeeks.org/avl-tree-set-1-insertion/ for the AVL implementation.
	I adapted it to this code base, which is why it looks so different from	the rest of the code. There are parts I like
	about the way it's done, and parts I do not like. Overall, this has been a fun and educational exercise.

	- Chris Laverdure
*/

// Used to convert references of mgBinaryTree node to mgAVLBinaryTree node. Since the AVL Insert function is the only
// one that creates nodes we can be confident that all nodes in this tree are actually mgAVLBinaryTree node's. Victims of polymorphism.
#define AVLNODE (mgAVLBinaryTreenode<TemplateObject> *)

template <typename TemplateObject>
class mgAVLBinaryTreenode : public mgBinaryTreenode<TemplateObject>
{
public:
	int Height;

	mgAVLBinaryTreenode()
	{
		this->Height = 1; // Default height for new objects is 1.
	}
};

template <typename TemplateObject>
class mgAVLBinaryTree : public mgBinaryTree < TemplateObject >
{
public:
	inline int AVLheight(mgAVLBinaryTreenode<TemplateObject> *CurrentNode)
	{
		if (CurrentNode == nullptr)
			return 0;
		return CurrentNode->Height;
	}
	inline int maxvalue(int a, int b)
	{
		return (a > b) ? a : b;
	}
	inline int getBalance(mgAVLBinaryTreenode<TemplateObject> *CurrentNode)
	{
		if (CurrentNode == nullptr)
			return 0;
		return AVLheight(AVLNODE  CurrentNode->Greater) - AVLheight(AVLNODE  CurrentNode->Lesser);
	}
	inline mgAVLBinaryTreenode<TemplateObject> *AVLrightRotate(mgAVLBinaryTreenode<TemplateObject> *CurrentNode)
	{
		mgAVLBinaryTreenode<TemplateObject> *LesserChild = AVLNODE  CurrentNode->Lesser;
		mgAVLBinaryTreenode<TemplateObject> *GreaterGrandChild = AVLNODE  LesserChild->Greater;

		// Right Rotation
		LesserChild->Greater = CurrentNode;
		CurrentNode->Lesser = GreaterGrandChild;

		// Update heights
		CurrentNode->Height = maxvalue(AVLheight(AVLNODE  CurrentNode->Lesser), AVLheight(AVLNODE  CurrentNode->Greater)) + 1;
		LesserChild->Height = maxvalue(AVLheight(AVLNODE  LesserChild->Lesser), AVLheight(AVLNODE  LesserChild->Greater)) + 1;

		return LesserChild;
	}
	inline mgAVLBinaryTreenode<TemplateObject> *AVLleftRotate(mgAVLBinaryTreenode<TemplateObject> *CurrentNode)
	{
		mgAVLBinaryTreenode<TemplateObject> *GreaterChild = AVLNODE  CurrentNode->Greater;
		mgAVLBinaryTreenode<TemplateObject> *LesserGrandChild = AVLNODE  GreaterChild->Lesser;

		// Left Rotation
		GreaterChild->Lesser = CurrentNode;
		CurrentNode->Greater = LesserGrandChild;

		// Update heights
		CurrentNode->Height = maxvalue(AVLheight(AVLNODE  CurrentNode->Lesser), AVLheight(AVLNODE  CurrentNode->Greater)) + 1;
		GreaterChild->Height = maxvalue(AVLheight(AVLNODE  GreaterChild->Lesser), AVLheight(AVLNODE  GreaterChild->Greater)) + 1;

		return GreaterChild;
	}

	mgAVLBinaryTreenode<TemplateObject> *AVLInsert(mgAVLBinaryTreenode<TemplateObject> *CurrentNode, TemplateObject Element);

	void AddElement(TemplateObject Element); // Override the AddElement function.

	mgAVLBinaryTree()
	{
		this->Definition = "AVL self balancing algorithm";
	}
};

template <typename TemplateObject>
mgAVLBinaryTreenode<TemplateObject> *mgAVLBinaryTree<TemplateObject>::AVLInsert(mgAVLBinaryTreenode<TemplateObject> *CurrentNode, TemplateObject Element)
{
	mgAVLBinaryTreenode<TemplateObject> *WorkerNode;
	if (CurrentNode == nullptr) // We landed on a empty leaf, this is where we want to add our node
	{
		mgAVLBinaryTreenode<TemplateObject> *newNode;

		newNode = new mgAVLBinaryTreenode < TemplateObject >;
		newNode->Element = Element;

		this->ElementCount++;

		return newNode;
	}

	// Only traverse if it is greater or lesser. If it is equal we unwind right now.
	if (Element < CurrentNode->Element)
	{
		WorkerNode = AVLInsert(AVLNODE  CurrentNode->Lesser, Element);

		if (WorkerNode != nullptr)
			CurrentNode->Lesser = WorkerNode;
	}
	else if (Element > CurrentNode->Element)
	{
		WorkerNode = AVLInsert(AVLNODE  CurrentNode->Greater, Element);

		if (WorkerNode != nullptr)
			CurrentNode->Greater = WorkerNode;
	}
	else
		return nullptr; // We found an equal, just unwind the loop now.

	CurrentNode->Height = maxvalue(AVLheight(AVLNODE  CurrentNode->Lesser), AVLheight(AVLNODE  CurrentNode->Greater)) + 1;

	int balance = getBalance(AVLNODE  CurrentNode);

	if (balance == 2)
	{
		if (getBalance(AVLNODE  CurrentNode->Greater) < 0)
			CurrentNode->Greater = AVLrightRotate(AVLNODE  CurrentNode->Greater);
		return AVLleftRotate(AVLNODE  CurrentNode);
	}
	else if (balance == -2)
	{
		if (getBalance(AVLNODE  CurrentNode->Lesser) > 0)
			CurrentNode->Lesser = AVLleftRotate(AVLNODE  CurrentNode->Lesser);
		return AVLrightRotate(AVLNODE  CurrentNode);
	}

	return CurrentNode; // No change
}

template <typename TemplateObject>
void mgAVLBinaryTree<TemplateObject>::AddElement(TemplateObject Element)
{
	mgAVLBinaryTreenode<TemplateObject> *newNode = AVLInsert(AVLNODE  this->Root, Element);
	if (newNode != nullptr) // This occurs when the root of the tree is a duplicate to a new Insert query. It returns nullptr
		this->Root = newNode;	    // to escape.
}

#endif
