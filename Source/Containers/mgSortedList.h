#ifndef MGSORTEDLISTH
#define MGSORTEDLISTH

#include "mgLinkedList.h"

/*
		Sorted Linked List
			Inherets from mgLinkedList

		This is a implementation of a sorted linked list that does insertion sorting to keep the list sorted at all times.
		Because of its implementation there is no need to change any other functions of mgLinkedList. Deletion (when it is implemented)
		will not break sorting.
*/

template <typename TemplateClass>
class mgSortedList : public mgLinkedList < TemplateClass >
{
public:
	bool BackwardSort;

	// Override these functions
	void AddElement(TemplateClass ElementToAdd);
	void AddElementReference(TemplateClass *ElementToAdd, bool Ownership);

	mgSortedList()
	{
		BackwardSort = false;
	};
};

template <typename TemplateClass>
void mgSortedList<TemplateClass>::AddElementReference(TemplateClass *ElementToAdd, bool Ownership)
{
	mgLinkedListElement<TemplateClass> *LinkedListEntry;
	mgLinkedListElement<TemplateClass> *LocalIterator = this->LinkedList;

	LinkedListEntry = new mgLinkedListElement < TemplateClass >;

	LinkedListEntry->Element = ElementToAdd;
	LinkedListEntry->Ownership = Ownership;

	if (this->LinkedList == nullptr)
	{
		this->LinkedList = LinkedListEntry; // There is no list, add it here

		this->Elements++;
		return;
	}

	// Now before we link this element into the list we need to find out where it belongs.
	// Backwards listing puts biggest elements at the start, smallest at the front.
	while (1)
	{ // WHILE
		if (this->BackwardSort)
		{	// Biggst items first
			if (*LinkedListEntry->Element > *LocalIterator->Element)
			{
				if (LocalIterator->Previous == nullptr)
					this->LinkedList = LinkedListEntry; // We are the first item.
				else
					LocalIterator->Previous->Next = LinkedListEntry;

				// Slip ourselves in before this object.
				LinkedListEntry->Previous = LocalIterator->Previous;
				LinkedListEntry->Next = LocalIterator;
				LocalIterator->Previous = LinkedListEntry;

				this->Elements++;
				break;
			}
		}
		else
		{ // Smallest items first.
			if (*LinkedListEntry->Element < *LocalIterator->Element)
			{
				if (LocalIterator->Previous == nullptr)
					this->LinkedList = LinkedListEntry; // We are the first item.
				else
					LocalIterator->Previous->Next = LinkedListEntry;

				// Slip ourselves in before this object.
				LinkedListEntry->Previous = LocalIterator->Previous;
				LocalIterator->Previous = LinkedListEntry;
				LinkedListEntry->Next = LocalIterator;

				this->Elements++;
				break;
			}

		}

		if (LocalIterator->Next == nullptr)
		{ // We've reached the end of the list without the object meeting our critera, tack it on the end
			LocalIterator->Next = LinkedListEntry;
			LinkedListEntry->Previous = LocalIterator;

			this->Elements++;
			break;
		}

		LocalIterator = LocalIterator->Next; // Move forward
	} // END while(1)

}

template <typename TemplateClass>
void mgSortedList<TemplateClass>::AddElement(TemplateClass ElementToAdd)
{
	TemplateClass *CopyofElement;

	CopyofElement = new TemplateClass;
	*CopyofElement = ElementToAdd;

	this->AddElementReference(CopyofElement, true);
}
#endif
