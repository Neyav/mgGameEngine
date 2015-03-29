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

// Used when generating a shape so that a lines normal doesn't
// need to be calculated by position. This saves a lot of unnecessary math where
// the outword facing of a line is definite and know upon generation
// Definitions for the facings may be a bit weird, 0,0 is the upper left corner of the map grid in this case
// To have the normal face away from -y, -x ( off the grid ) the following rules are observed:
// A line going from 0, 0 to 2, 0 is considered to be facing right
// A line going to 2, 0 to 0, 0 would be facing left
// A line going from 0, 0 to 0, 2 is considered to be facing left
// A line going from 0, 2 to 0, 0 is considered to be facing right
// If you are uncertain which to use when generating a shape out of lines, either test it out or just
// leave it as its default undefined. This isn't a make or break decision, just a little optimization.
#define LineFacing unsigned char
#define LINEFACE_UNDEFINED 0
#define LINEFACE_RIGHT 1
#define LINEFACE_LEFT 2

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

	LineDescription Description;
	LineFacing Facing;

	// Otherworldly references to the line segment
	mgMapElement *LineSegmentBlock;
	mgMapObject *LineSegmentOwner;

	void ImportLine(mgPoint Origin, mgVector Direction, double Length);
	void ImportLine(mgPoint LineStart, mgPoint LineEnd);
	mgPoint InterceptionPoint(mgLineSegment *SecondLine, bool *ValidIntercept);
	mgVector NormalFacingPosition(mgPoint Position);

	mgLineSegment();
};

#endif