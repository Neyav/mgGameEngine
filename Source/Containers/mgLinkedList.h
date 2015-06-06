#ifndef MGLINKEDLISTH
#define MGLINKEDLISTH

#include <stdlib.h>
#include <iostream>

/*
	Now, why in the name of sweet little baby Jesus would I go ahead and use C++ features like objects and templates
	to create a linked list template class when SUCH a thing already exists as part of the C++ STD Library?

	The answer is two-fold. Primarily because it was faster for me to write my own class that did exactly what I needed it to
	do and was expandable in EXACTLY the ways I need to expand it than it was for me to relearn how to use STD::list. Secondarily
	I am exactly that arrogant and pompous and, for fucks sake, I LIKE MY OWN Linked List implementation, so BUGGER OFF!!!

	-Chris Laverdure, arrogant engineer of re-inventing the wheel.
*/

template <typename TemplateClass>
class mgLinkedListElement
{
public:
	TemplateClass *Element;
	bool Ownership;

	mgLinkedListElement<TemplateClass> *Next;
	mgLinkedListElement<TemplateClass> *Previous;

	mgLinkedListElement<TemplateClass>();
	~mgLinkedListElement<TemplateClass>();
};

template <typename TemplateClass> mgLinkedListElement<TemplateClass>::mgLinkedListElement()
{
	Ownership = true; // Default ownership

	Element = nullptr;
	Next = Previous = nullptr;
}

template <typename TemplateClass> mgLinkedListElement<TemplateClass>::~mgLinkedListElement()
{
	// Keep the chain.
	if (Next != nullptr)
		Next->Previous = Previous;
	if (Previous != nullptr)
		Previous->Next = Next;

	if (Ownership)
		delete Element; // We own it, we clear it.
}


template <typename TemplateClass> class mgLinkedList
{
public:
	mgLinkedListElement<TemplateClass> *LinkedList;
	mgLinkedListElement<TemplateClass> *Iterator;

	int Elements;

	// Operator Overloads
	TemplateClass *operator[] (const int IndexNumber);

	// Functions
	virtual void AddElement(TemplateClass ElementToAdd);
	virtual void AddElementReference(TemplateClass *ElementToAdd, bool Ownership);
	void ResetIterator(void);
	void ClearList(void);
	TemplateClass ReturnElement(void);
	TemplateClass *ReturnElementReference(void);
	int NumberOfElements(void);
	bool IteratorAtEnd(void);
	mgLinkedListElement<TemplateClass> *ReturnIterator(void);
	mgLinkedListElement<TemplateClass> *ReturnHeadIterator(void);

	mgLinkedList();
	~mgLinkedList();
};

template <typename TemplateClass>
TemplateClass *mgLinkedList<TemplateClass>::operator[] (const int IndexNumber)
{
	mgLinkedListElement<TemplateClass> *LocalIterator = LinkedList;
	int IndexIterator = IndexNumber;

	// These are invalid requests
	if (IndexNumber > (Elements - 1))
		return nullptr;
	else if (IndexNumber < 0)
		return nullptr;

	while (IndexIterator > 0)
	{
		LocalIterator = LocalIterator->Next;
		IndexIterator--;
	}

	return LocalIterator->Element;
}

template <typename TemplateClass> void mgLinkedList<TemplateClass>::AddElement(TemplateClass ElementToAdd)
{
	mgLinkedListElement<TemplateClass> *LinkedListEntry;
	TemplateClass *CopyofElement;

	CopyofElement = new TemplateClass;

	*CopyofElement = ElementToAdd;

	LinkedListEntry = new mgLinkedListElement < TemplateClass >;

	LinkedListEntry->Element = CopyofElement;
	LinkedListEntry->Ownership = true;
	LinkedListEntry->Next = LinkedList;
	if (LinkedList != nullptr) 
		LinkedList->Previous = LinkedListEntry;

	LinkedList = Iterator = LinkedListEntry;

	Elements++;
}

template <typename TemplateClass> void mgLinkedList<TemplateClass>::AddElementReference(TemplateClass *ElementToAdd, bool Ownership)
{
	mgLinkedListElement<TemplateClass> *LinkedListEntry;

	LinkedListEntry = new mgLinkedListElement < TemplateClass >;

	LinkedListEntry->Element = ElementToAdd;
	LinkedListEntry->Next = LinkedList;
	LinkedListEntry->Ownership = Ownership;
	if (LinkedList != nullptr) 	
		LinkedList->Previous = LinkedListEntry;

	LinkedList = Iterator = LinkedListEntry;

	Elements++;
}

template <typename TemplateClass> void mgLinkedList<TemplateClass>::ResetIterator(void)
{
	Iterator = LinkedList;
}

template <typename TemplateClass> TemplateClass mgLinkedList<TemplateClass>::ReturnElement(void)
{
	if (Iterator != nullptr)
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

template <typename TemplateClass> TemplateClass *mgLinkedList<TemplateClass>::ReturnElementReference(void)
{
	if (Iterator == nullptr)
		return nullptr;
	else
	{
		mgLinkedListElement<TemplateClass> *ReturnObject;
		ReturnObject = Iterator;
		Iterator = Iterator->Next;
		return ReturnObject->Element;
	}
}


template <typename TemplateClass> int mgLinkedList<TemplateClass>::NumberOfElements(void)
{
	return Elements;
}

template <typename TemplateClass> bool mgLinkedList<TemplateClass>::IteratorAtEnd(void)
{
	if (Iterator == nullptr)
		return true;

	return false;
}

template <typename TemplateClass> mgLinkedListElement<TemplateClass> *mgLinkedList<TemplateClass>::ReturnIterator(void)
{
	return Iterator;
}

template <typename TemplateClass> mgLinkedListElement<TemplateClass> *mgLinkedList<TemplateClass>::ReturnHeadIterator(void)
{
	return LinkedList;
}

template <typename TemplateClass> void mgLinkedList<TemplateClass>::ClearList(void)
{
	if (LinkedList != nullptr)
	{
		while (LinkedList->Next != nullptr)
			delete LinkedList->Next;
		delete LinkedList;
	}

	LinkedList = nullptr;
}

template <typename TemplateClass> mgLinkedList<TemplateClass>::mgLinkedList()
{
	LinkedList = nullptr;
	Iterator = nullptr;

	Elements = 0;
}

template <typename TemplateClass> mgLinkedList<TemplateClass>::~mgLinkedList()
{
	// Empty the list
	this->ClearList();
}
#endif