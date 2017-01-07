#ifndef MGLINESEGMENTH
#define MGLINESEGMENTH

#include <stdlib.h>

#include "mgVectorPoint.h"

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
// UPDATE: Properly identifying the facing of your geometry lines is necessary for proper operation of
//			shadow hull generation. So if you plan on having dynamic shadows, this is necessary.
#define LineFacing unsigned char
#define LINEFACE_UNDEFINED 0
#define LINEFACE_RIGHT 1
#define LINEFACE_LEFT 2

// Includes were causing serious issues with C++ for some stupid reason.
// Seems like circular dependencies give it a headache.
// Fuck you Microsoft, FUCK YOU Visual Studio.
class mgMapElement;
class mgMapObject;

struct mgLineCollisionResults
{
	mgPoint CollisionPoint;
	bool Collision;
};

class mgLineSegment
{
public:

	// Definitions of the line segment
	mgPoint SegmentStart;
	mgPoint SegmentEnd;
	double SegmentLength;

	bool ObstructsMovement;
	bool ObstructsVision;

	LineFacing Facing;

	// Otherworldly references to the line segment
	mgMapElement *LineSegmentBlock;
	mgMapObject *LineSegmentOwner;

	void ImportLine(mgPoint Origin, mgVector Direction, double Length);
	void ImportLine(mgPoint Origin, mgVector Direction);
	void ImportLine(mgPoint LineStart, mgPoint LineEnd);
	mgVector NormalFacingPosition(mgPoint Position);
	mgPoint Middle(void);
	mgLineCollisionResults CollisionTest(mgLineSegment *Against);

	mgLineSegment();
};

mgLineSegment *mgDefineLine(double StartY, double StartX, double EndY, double EndX, mgPoint Offset, LineFacing Facing, mgMapElement *ElementOwner, mgMapObject *ObjectOwner);

#endif
