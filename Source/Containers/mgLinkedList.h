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

// ------------------- mgLinkedListElement
// - This is the container for each element in a linked list.
// -

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

// ------------------- mgLinkedList
// - This is the linked list itself.
// -

template <typename TemplateClass> class mgListIterator;

template <typename TemplateClass> class mgLinkedList
{
public:
	mgLinkedListElement<TemplateClass> *LinkedList;

	int Elements;

	// Operator Overloads
	TemplateClass *operator[] (const int IndexNumber);

	// Functions
	virtual void AddElement(TemplateClass ElementToAdd);
	virtual void AddElementReference(TemplateClass *ElementToAdd, bool Ownership);
	void ClearList(void);
	int NumberOfElements(void);

	mgListIterator<TemplateClass> ListIterator(void);

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

	LinkedList = LinkedListEntry;

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

	LinkedList = LinkedListEntry;

	Elements++;
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

	Elements = 0; 
}

template <typename TemplateClass>
mgListIterator<TemplateClass> mgLinkedList<TemplateClass>::ListIterator(void)
{
	mgListIterator<TemplateClass> NewListIterator(this);

	return NewListIterator;
}

template <typename TemplateClass> 
int mgLinkedList<TemplateClass>::NumberOfElements(void)
{
	return Elements;
}

template <typename TemplateClass> mgLinkedList<TemplateClass>::mgLinkedList()
{
	LinkedList = nullptr;

	Elements = 0;
}

template <typename TemplateClass> mgLinkedList<TemplateClass>::~mgLinkedList()
{
	// Empty the list
	this->ClearList();
}

// ------------------- mgListIterator
// - This is the iterator for reading from a linked list.
// -

template <typename TemplateClass> class mgListIterator
{
private:
	mgLinkedListElement<TemplateClass> *Iterator;
	mgLinkedList<TemplateClass> *MasterList;
public:
	void LinktoList(mgLinkedList<TemplateClass> *List);

	TemplateClass ReturnElement(void);
	TemplateClass *ReturnElementReference(void);
	int NumberOfElements(void);
	bool IteratorAtEnd(void);
	void JumptoStart(void);

	mgListIterator(mgLinkedList<TemplateClass> *MasterList)
	{
		this->MasterList = MasterList;
		this->Iterator = MasterList->LinkedList;
	}
	mgListIterator()
	{
		this->MasterList = nullptr;
		this->Iterator = nullptr;
	}
};

template <typename TemplateClass>
void mgListIterator<TemplateClass>::LinktoList(mgLinkedList<TemplateClass> *List)
{
	this->MasterList = List;
	this->Iterator = MasterList->LinkedList;
}

template <typename TemplateClass>
TemplateClass mgListIterator<TemplateClass>::ReturnElement(void)
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

		std::cout << "template <class TemplateClass> TemplateClass mgListIterator<TemplateClass>::ReturnElement(void) -- Attempt to read past end of list." << std::endl;

		exit(-1);
	}
}

template <typename TemplateClass>
TemplateClass *mgListIterator<TemplateClass>::ReturnElementReference(void)
{
	if (Iterator != nullptr)
	{
		mgLinkedListElement<TemplateClass> *ReturnObject;
		ReturnObject = Iterator;
		Iterator = Iterator->Next;
		return ReturnObject->Element;
	}
	else
	{
		// You should be checking for the end of the list if you are using this function. Exit hard and painfully because this error could crop up somewhere else
		// in a more ambigious manner if we don't fix the problem now.

		std::cout << "template <class TemplateClass> TemplateClass mgListIterator<TemplateClass>::ReturnElement(void) -- Attempt to read past end of list." << std::endl;

		exit(-1);
	}
}

template <typename TemplateClass> 
int mgListIterator<TemplateClass>::NumberOfElements(void)
{
	if (MasterList != nullptr)
		return MasterList->Elements;
	else
		return 0;
}

template <typename TemplateClass>
bool mgListIterator<TemplateClass>::IteratorAtEnd(void)
{
	if (Iterator == nullptr)
		return true;
	else
		return false;
}

template <typename TemplateClass> 
void mgListIterator<TemplateClass>::JumptoStart(void)
{
	if (MasterList != nullptr)
		Iterator = MasterList->LinkedList;
	else
		Iterator = nullptr;
}
#endif
