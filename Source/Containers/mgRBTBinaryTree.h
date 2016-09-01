#ifndef MGRBTBINARYTREEH
#define MGRBTBINARYTREEH

#include "mgBinaryTree.h"

/*
	Red Black Tree Binary Tree Template

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

// Used to convert references of mgBinaryTree node to mgRBTBinaryTree node. Since the RBT Insert function is the only
// one that creates nodes we can be confident that all nodes in this tree are actually mgRBTBinaryTree node's. Victims of polymorphism.
#define RBTNODE (mgRBTBinaryTreenode<TemplateObject> *)

template <typename TemplateObject>
class mgRBTBinaryTree;

template <typename TemplateObject>
class mgRBTBinaryTreenode : public mgBinaryTreenode < TemplateObject >
{
public:
	bool BlackNode;
	mgRBTBinaryTreenode<TemplateObject> *Parent;

	void fixUp(mgRBTBinaryTree<TemplateObject> *TreeReference);
	void leftRotate(mgRBTBinaryTree<TemplateObject> *TreeReference);
	void rightRotate(mgRBTBinaryTree<TemplateObject> *TreeReference);

	mgRBTBinaryTreenode()
	{
		BlackNode = false; // New nodes are red by default
		Parent = nullptr; 
	}
};

template <typename TemplateObject>
void mgRBTBinaryTreenode<TemplateObject>::leftRotate(mgRBTBinaryTree<TemplateObject> *TreeReference)
{
	mgBinaryTreenode<TemplateObject> *ReferenceNode = this->Greater;

	this->Greater = ReferenceNode->Lesser;
	if (ReferenceNode->Lesser != nullptr)
		(RBTNODE ReferenceNode->Lesser)->Parent = this;
	(RBTNODE ReferenceNode)->Parent = this->Parent;

	if (this->Parent == nullptr)
		TreeReference->Root = ReferenceNode;
	else if (this == this->Parent->Lesser)
		this->Parent->Lesser = ReferenceNode;
	else
		this->Parent->Greater = ReferenceNode;

	ReferenceNode->Lesser = this;
	this->Parent = RBTNODE ReferenceNode;
}

template <typename TemplateObject>
void mgRBTBinaryTreenode<TemplateObject>::rightRotate(mgRBTBinaryTree<TemplateObject> *TreeReference)
{
	mgBinaryTreenode<TemplateObject> *ReferenceNode = this->Lesser;

	this->Lesser = ReferenceNode->Greater;
	if (ReferenceNode->Greater != nullptr)
		(RBTNODE ReferenceNode->Greater)->Parent = this;
	(RBTNODE ReferenceNode)->Parent = this->Parent;

	if (this->Parent == nullptr)
		TreeReference->Root = ReferenceNode;
	else if (this == this->Parent->Greater)
		this->Parent->Greater = ReferenceNode;
	else
		this->Parent->Lesser = ReferenceNode;

	ReferenceNode->Greater = this;
	this->Parent = RBTNODE ReferenceNode;
}

template <typename TemplateObject>
void mgRBTBinaryTreenode<TemplateObject>::fixUp(mgRBTBinaryTree<TemplateObject> *TreeReference)
{
	mgRBTBinaryTreenode<TemplateObject> *FixupNode = this;
	// We just added an element and need to perform the necessary steps to fix the balance of the tree
	while (FixupNode->Parent != nullptr && !FixupNode->Parent->BlackNode)
	{ // Our parent is a Red Node
		if (FixupNode->Parent == FixupNode->Parent->Parent->Lesser)
		{
			mgRBTBinaryTreenode<TemplateObject> *ReferenceNode = RBTNODE FixupNode->Parent->Parent->Greater;

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
			mgRBTBinaryTreenode<TemplateObject> *ReferenceNode = RBTNODE FixupNode->Parent->Parent->Lesser;

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

template <typename TemplateObject>
class mgRBTBinaryTree : public mgBinaryTree < TemplateObject >
{
public:
	void AddElement(TemplateObject Element); // Override AddElement function

	mgRBTBinaryTree()
	{
		this->Definition = "Red Black Tree self balancing algorithm";
	}
};

template <typename TemplateObject>
void mgRBTBinaryTree<TemplateObject>::AddElement(TemplateObject Element)
{
	if (this->Root == nullptr)
	{	// This is the first object in the list, just add it to the root.
		mgRBTBinaryTreenode < TemplateObject > *newNode;
		newNode = new mgRBTBinaryTreenode < TemplateObject >;
		newNode->Element = Element;
		newNode->BlackNode = true; // This is all the fixing we need for a blank tree.
		this->Root = newNode;

		this->ElementCount++;
	}
	else
	{	// Find the proper leaf we need to branch off of....
		mgBinaryTreenode<TemplateObject> *ProgressNode = this->Root; // Start at the root.
		while (1) // 1 is always 1.....
		{
			if (ProgressNode->Element == Element) // We found ourselves in the tree. There is nothing more to do here.
				break;
			else if (ProgressNode->Element > Element) // We are smaller than this branch node.
			{
				if (ProgressNode->Lesser == nullptr) // We belong here.....
				{
					mgRBTBinaryTreenode < TemplateObject > *newNode;
					newNode = new mgRBTBinaryTreenode < TemplateObject >;

					newNode->Element = Element;
					newNode->Parent = RBTNODE ProgressNode;
					ProgressNode->Lesser = newNode;

					// Add Linked List bindings.
					newNode->Next = RBTNODE this->Root;
					newNode->Previous = RBTNODE this->Root->Previous;
					this->Root->Previous = newNode;

					newNode->fixUp(this); // Fix any Red Black Tree rule violations that occured by adding this node.
					(RBTNODE this->Root)->BlackNode = true;

					this->ElementCount++;
					break;
				}
				else
					ProgressNode = ProgressNode->Lesser; // Proceed down the next branch of the tree...
			}
			else // We are greater than this branch node.
			{
				if (ProgressNode->Greater == nullptr) // We belong here.
				{
					mgRBTBinaryTreenode < TemplateObject > *newNode;
					newNode = new mgRBTBinaryTreenode < TemplateObject >;

					newNode->Element = Element;
					newNode->Parent = RBTNODE ProgressNode;
					ProgressNode->Greater = newNode;

					// Add Linked List bindings.
					newNode->Next = RBTNODE this->Root;
					newNode->Previous = RBTNODE this->Root->Previous;
					this->Root->Previous = newNode;

					newNode->fixUp(this); // Fix any Red Black Tree rule violations that occured by adding this node.
					(RBTNODE this->Root)->BlackNode = true;

					this->ElementCount++;
					break;
				}
				else
					ProgressNode = ProgressNode->Greater; // Proceed down the next branch of the tree....
			}
		} // END of while loop
	}
}

#endif
