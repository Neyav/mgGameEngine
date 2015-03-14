#ifndef MGLINESEGMENTH
#define MGLINESEGMENTH

#include <stdlib.h>

#include "mgVectorPoint.h"

// Includes were causing serious issues with C++ for some stupid reason.
// Seems like circular dependencies give it a headache.
// Fuck you Microsoft, FUCK YOU Visual Studio.
class mgMapElement;
class mgMapObject;

class mgLineSegment
{
public:

	// Definitions of the line segment
	mgPoint SegmentStart;
	mgPoint SegmentEnd;
	double SegmentLength;

	// Otherworldly references to the line segment
	mgMapElement *LineSegmentBlock = NULL;
	mgMapObject *LineSegmentOwner = NULL;

	void ImportLine(mgPoint Origin, mgVector Direction, double Length);
	void ImportLine(mgPoint LineStart, mgPoint LineEnd);
	mgPoint InterceptionPoint(mgLineSegment *SecondLine, bool *ValidIntercept);
	mgVector NormalFacingPosition(mgPoint Position);
};

#endif