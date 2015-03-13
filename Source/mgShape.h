#ifndef MGSHAPEH
#define MGSHAPEH

#include "mgLineSegment.h"
#include "mgLinkedList.h"
//#include "mgMapElement.h"
//#include "mgMapObject.h"
#include "mgVectorPoint.h"

extern class mgMapElement;
extern class mgMapObject;

class mgShape
{
private:
	mgLinkedList<mgLineSegment> ShapeLines;

public:
	void AddLineToShape(mgLineSegment LineToAdd, mgMapElement *MapBlock, mgMapObject *LineOwner);
	void AddLineToShape(mgPoint PointFrom, mgPoint PointTo, mgMapElement *MapBlock, mgMapObject *LineOwner);

	mgLinkedList<mgLineSegment> *ShapeForm(void);
};
#endif