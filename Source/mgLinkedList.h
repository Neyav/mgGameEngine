#ifndef MGLINKEDLISTH
#define MGLINKEDLISTH

#include <stdlib.h>

/*
	Now, why in the name of sweet little baby Jesus would I go ahead and use C++ features like objects and templates
	to create a linked list template class when SUCH a thing already exists as part of the C++ STD Library?

	The answer is two-fold. Primarily because it was faster for me to write my own class that did exactly what I needed it to
	do and was expandable in EXACTLY the ways I need to expand it than it was for me to relearn how to use STD::list. Secondarily
	I am exactly that arrogant and pompous and, for fucks sake, I LIKE MY OWN Linked List implementation, so BUGGER OFF!!!

	-Chris Laverdure, arrogant engineer of re-inventing the wheel.
*/

template <typename TemplateStruct>
struct mgLinkedListElement
{
	TemplateStruct *Element;
	bool ElementPassedAsPointer;

	mgLinkedListElement<TemplateStruct> *Next;
	mgLinkedListElement<TemplateStruct> *Previous;
};

template <class TemplateClass> class mgLinkedList
{
private:
	mgLinkedListElement<TemplateClass> *LinkedList;
	mgLinkedListElement<TemplateClass> *Iterator;

	int Elements;

public:

	bool ExplicitPointerCleanup; // If true the linked list cleans its pointers, regardless of any built in behavoir.

	void AddElement(TemplateClass ElementToAdd);
	void AddElementReference(TemplateClass *ElementToAdd);
	void ResetIterator(void);
	void ClearList(void);
	TemplateClass ReturnElement(void);
	TemplateClass *ReturnElementReference(void);
	int NumberOfElements(void);
	bool IteratorAtEnd(void);

	mgLinkedList();
	~mgLinkedList();
};

template <class TemplateClass> void mgLinkedList<TemplateClass>::AddElement(TemplateClass ElementToAdd)
{
	// This is the head of the linked list, let's create our first entry!
	mgLinkedListElement<TemplateClass> *LinkedListEntry;
	TemplateClass *CopyofElement;

	CopyofElement = new TemplateClass;

	*CopyofElement = ElementToAdd;

	LinkedListEntry = new mgLinkedListElement < TemplateClass >;

	LinkedListEntry->Element = CopyofElement;
	LinkedListEntry->Next = LinkedList;
	LinkedListEntry->ElementPassedAsPointer = false; // This element was not passed as a pointer.
	if (LinkedList == NULL)
	{
		LinkedList = Iterator = LinkedListEntry;
		LinkedList->Previous = NULL;
	}
	else
	{
		LinkedList->Previous = LinkedListEntry;
		LinkedList = LinkedListEntry;
		Iterator = LinkedListEntry;
	}

	Elements++;
}

template <class TemplateClass> void mgLinkedList<TemplateClass>::AddElementReference(TemplateClass *ElementToAdd)
{
	// This is the head of the linked list, let's create our first entry!
	mgLinkedListElement<TemplateClass> *LinkedListEntry;

	LinkedListEntry = new mgLinkedListElement < TemplateClass >;

	LinkedListEntry->Element = ElementToAdd;
	LinkedListEntry->Next = LinkedList;
	LinkedListEntry->ElementPassedAsPointer = true; // It was passed as a pointer;
	if (LinkedList == NULL)
	{
		LinkedList = Iterator = LinkedListEntry;
		LinkedList->Previous = NULL;
	}
	else
	{
		LinkedList->Previous = LinkedListEntry;
		LinkedList = LinkedListEntry;
		Iterator = LinkedListEntry;
	}

	Elements++;
}

template <class TemplateClass> void mgLinkedList<TemplateClass>::ResetIterator(void)
{
	Iterator = LinkedList;
}

template <class TemplateClass> TemplateClass mgLinkedList<TemplateClass>::ReturnElement(void)
{
	if (Iterator != NULL)
	{
		TemplateClass CopyOfObject;

		CopyOfObject = *Iterator->Element;
		Iterator = Iterator->Next;
		return CopyOfObject;
	}
	else
	{
		// You should be checking for the end of the list if you are using this function. Exit hard and painfully because this error could crop up somewhere else
		// in a more ambigious manner if we don't fix the problem now.

		std::cout << "template <class TemplateClass> TemplateClass mgLinkedList<TemplateClass>::ReturnElement(void) -- Attempt to read past end of list." << std::endl;

		exit(-1);
	}
}

template <class TemplateClass> TemplateClass *mgLinkedList<TemplateClass>::ReturnElementReference(void)
{
	if (Iterator == NULL)
		return NULL;
	else
	{
		mgLinkedListElement<TemplateClass> *ReturnObject;
		ReturnObject = Iterator;
		Iterator = Iterator->Next;
		ReturnObject->ElementPassedAsPointer = true; // This object has been sent to its parent as a pointer reference
													// This being true prevents the list clean up from deleting it.
		return ReturnObject->Element;
	}
}


template <class TemplateClass> int mgLinkedList<TemplateClass>::NumberOfElements(void)
{
	return Elements;
}

template <class TemplateClass> bool mgLinkedList<TemplateClass>::IteratorAtEnd(void)
{
	if (Iterator == NULL)
		return true;

	return false;
}

template <class TemplateClass> void mgLinkedList<TemplateClass>::ClearList(void)
{
	while (LinkedList != NULL)
	{
		mgLinkedListElement<TemplateClass> *LastEntry;

		LastEntry = LinkedList;
		LinkedList = LinkedList->Next;

		delete LastEntry;
	}
}

template <class TemplateClass> mgLinkedList<TemplateClass>::mgLinkedList()
{
	LinkedList = NULL;
	Iterator = NULL;

	ExplicitPointerCleanup = false;

	Elements = 0;
}

template <class TemplateClass> mgLinkedList<TemplateClass>::~mgLinkedList()
{
	while (LinkedList != NULL)
	{
		mgLinkedListElement<TemplateClass> *LastEntry;

		LastEntry = LinkedList;
		LinkedList = LinkedList->Next;

		if (!LastEntry->ElementPassedAsPointer || ExplicitPointerCleanup)
			delete LastEntry->Element; // This element was never passed on as a pointer, nor was it recieved as one, we can delete it here.
										// If it was passed on, or recieved as a pointer it is imagined that the coder will hand delete
										// their own references.
		delete LastEntry;
	}
}
#endif