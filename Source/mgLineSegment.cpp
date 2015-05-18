#include <math.h>
#include <iostream>

#include "mgLineSegment.h"
#include "mgVectorPoint.h"

void mgLineSegment::ImportLine(mgPoint Origin, mgVector Direction, double Length)
{
	// Let's derive as much information as we can about this line, and fill the corresponding information tables in
	SegmentStart = Origin;
	SegmentLength = Length;

	Direction.NormalizeVector(Length);
	
	SegmentEnd.Y = Origin.Y + Direction.Y;
	SegmentEnd.X = Origin.X + Direction.X;
}

void mgLineSegment::ImportLine(mgPoint LineStart, mgPoint LineEnd)
{
	SegmentStart.Y = LineStart.Y;
	SegmentStart.X = LineStart.X;

	SegmentEnd.Y = LineEnd.Y;
	SegmentEnd.X = LineEnd.X;

	SegmentLength = DistanceBetweenPoints(SegmentStart, SegmentEnd);
}

// This code is derived from the very useful example I found at: http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/1968345#1968345
// Thank you very much sir, you have assisted me in a most troubling time.
// As a reference to the original material, and a guide for me writing this, p0 = SegmentStart, p1 = SegmentEnd, p2 = Against->SegmentStart, p3 = Against->SegmentEnd
mgLineCollisionResults mgLineSegment::CollisionTest(mgLineSegment *Against)
{
	mgLineCollisionResults Results;
	mgPoint s1, s2; // s1 and s2 are hommages to the original source material.
	double s, t; // Same as above. :p

	Results.Collision = false; // It is assumed they don't collide.

	s1 = SegmentEnd - SegmentStart;
	s2 = Against->SegmentEnd - Against->SegmentStart;

	// *Cracks knuckles* Translation time. I don't expect this code to be readable, but then again, neither was the source material ( no offense )
	s = (-s1.Y * (SegmentStart.X - Against->SegmentStart.X) + s1.X * (SegmentStart.Y - Against->SegmentStart.Y)) / (-s2.X * s1.Y + s1.X * s2.Y);
	t = (s2.X * (SegmentStart.Y - Against->SegmentStart.Y) - s2.Y * (SegmentStart.X - Against->SegmentStart.X)) / (-s2.X * s1.Y + s1.X * s2.Y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		Results.CollisionPoint.X = SegmentStart.X + (t * s1.X);
		Results.CollisionPoint.Y = SegmentStart.Y + (t * s1.Y);
		Results.Collision = true;
	}

	return Results;

}

// Calculate the lines normal facing the position unless the line has a predetermined facing, in which case that facing is used.
mgVector mgLineSegment::NormalFacingPosition(mgPoint Position)
{
	mgPoint MiddleofLine;
	mgVector Normal, PositionTowardsCenter;

	// Line facing isn't defined, calculate the normal manually using the point position to determine orientation
	switch (Facing)
	{ 
	case LINEFACE_RIGHT:
		Normal.Y = (SegmentEnd.X - SegmentStart.X) * -1;
		Normal.X = (SegmentEnd.Y - SegmentStart.Y);
		break;
	case LINEFACE_LEFT:
		Normal.Y = (SegmentEnd.X - SegmentStart.X);
		Normal.X = (SegmentEnd.Y - SegmentStart.Y) * -1;
		break;
	default:
		// Calculate the middle of our line
		MiddleofLine.Y = (SegmentEnd.Y - SegmentStart.Y) / 2 + SegmentStart.Y;
		MiddleofLine.X = (SegmentEnd.X - SegmentStart.X) / 2 + SegmentStart.X;

		// Calculate the vector from Position pointing at the center of the line.
		PositionTowardsCenter.VectorFromPoints(Position, MiddleofLine);

		// Calculate the normal
		Normal.Y = (SegmentEnd.X - SegmentStart.X) * -1;
		Normal.X = (SegmentEnd.Y - SegmentStart.Y);

		if (Normal * PositionTowardsCenter > 0) // They are facing the same direction, this isn't our normal.
		{
			Normal.Y *= -1;
			Normal.X *= -1;
		}
	}

	Normal.NormalizeVector();

	return Normal;
}

mgLineSegment::mgLineSegment()
{ // Initialize default behavior for a line segment.
	Description = LINEDES_UNDEFINED;
	Facing = LINEFACE_UNDEFINED;

	LineSegmentBlock = NULL;
	LineSegmentOwner = NULL;

	ObstructsMovement = true;
	ObstructsVision = true;
}

mgLineSegment *mgDefineLine(double StartY, double StartX, double EndY, double EndX, mgPoint Offset, LineDescription Description, LineFacing Facing, mgMapElement *ElementOwner, mgMapObject *ObjectOwner)
{
	mgLineSegment *DefinedLine;
	mgPoint Start, End;

	DefinedLine = new mgLineSegment;
	Start.Y = StartY + Offset.Y;
	Start.X = StartX + Offset.X;
	End.Y = EndY + Offset.Y;
	End.X = EndX + Offset.X;
	DefinedLine->ImportLine(Start,End);
	DefinedLine->LineSegmentBlock = ElementOwner;
	DefinedLine->LineSegmentOwner = ObjectOwner;
	DefinedLine->Facing = Facing;
	DefinedLine->Description = Description;

	return DefinedLine;
}
