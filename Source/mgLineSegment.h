#ifndef MGLINESEGMENTH
#define MGLINESEGMENTH

#include <stdlib.h>

#include "mgVectorPoint.h"

#define LineDescription unsigned char

// Feel free to add more as they are needed
#define LINEDES_UNDEFINED 0
#define LINEDES_TOP 1
#define LINEDES_LEFTSIDE 2
#define LINEDES_RIGHTSIDE 3
#define LINEDES_BOTTOM 4

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

	bool ObstructsMovement = true;
	bool ObstructsVision = true;

	LineDescription Description = LINEDES_UNDEFINED;

	// Otherworldly references to the line segment
	mgMapElement *LineSegmentBlock = NULL;
	mgMapObject *LineSegmentOwner = NULL;

	void ImportLine(mgPoint Origin, mgVector Direction, double Length);
	void ImportLine(mgPoint LineStart, mgPoint LineEnd);
	mgPoint InterceptionPoint(mgLineSegment *SecondLine, bool *ValidIntercept);
	mgVector NormalFacingPosition(mgPoint Position);
};

#endif