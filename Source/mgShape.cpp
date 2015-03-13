#include <stdlib.h>

#include "mgLineSegment.h"
#include "mgLinkedList.h"
#include "mgShape.h"

void mgShape::AddLineToShape(mgLineSegment LineToAdd, mgMapElement *MapBlock, mgMapObject *LineOwner)
{
	LineToAdd.LineSegmentBlock = MapBlock;
	LineToAdd.LineSegmentOwner = LineOwner;

	ShapeLines.AddElement(LineToAdd);
}

void mgShape::AddLineToShape(mgPoint PointFrom, mgPoint PointTo, mgMapElement *MapBlock, mgMapObject *LineOwner)
{
	mgLineSegment LineToAdd;

	LineToAdd.ImportLine(PointFrom, PointTo);
	LineToAdd.LineSegmentBlock = MapBlock;
	LineToAdd.LineSegmentOwner = LineOwner;

	ShapeLines.AddElement(LineToAdd);
}

mgLinkedList<mgLineSegment> *mgShape::ShapeForm(void)
{
	mgLinkedList<mgLineSegment> *LinesToSend;

	LinesToSend = new mgLinkedList < mgLineSegment > ;

	ShapeLines.ResetIterator();

	for (int Iterator = 0; Iterator < ShapeLines.NumberOfElements(); Iterator++)
	{
		mgLineSegment *CopiedLine;

		CopiedLine = ShapeLines.ReturnElement();

		LinesToSend->AddElementReference(CopiedLine);
	}

	LinesToSend->ExplicitPointerCleanup = true; // When the list is deleted, so to are all its copied lines.
	return LinesToSend;
}