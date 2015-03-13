#include <math.h>
#include <iostream>

#include "mgLineSegment.h"
#include "mgVectorPoint.h"

void mgLineSegment::ImportLine(mgPoint Origin, mgVector Direction, double Length)
{
	// Let's derive as much information as we can about this line, and fill the corresponding information tables in
	SegmentStart = Origin;
	SegmentLength = Length;

	Direction.NormalizeVector(1);
	Direction.VectorStepCords(Length);

	SegmentEnd.Y = Origin.Y + Direction.TransformedY;
	SegmentEnd.X = Origin.X + Direction.TransformedX;
}

void mgLineSegment::ImportLine(mgPoint LineStart, mgPoint LineEnd)
{
	SegmentStart.Y = LineStart.Y;
	SegmentStart.X = LineStart.X;

	SegmentEnd.Y = LineEnd.Y;
	SegmentEnd.X = LineEnd.X;

	SegmentLength = DistanceBetweenPoints(SegmentStart, SegmentEnd);
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
char get_line_intersection(double p0_x, double p0_y, double p1_x, double p1_y,
	double p2_x, double p2_y, double p3_x, double p3_y, double *i_x, double *i_y)
{
	double s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	double s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		if (i_x != NULL)
			*i_x = p0_x + (t * s1_x);
		if (i_y != NULL)
			*i_y = p0_y + (t * s1_y);
		return 1;
	}

	return 0; // No collision
}

mgPoint mgLineSegment::InterceptionPoint(mgLineSegment *SecondLine, bool *ValidIntercept)
{
	double x, y;
	
	char Interception = get_line_intersection(SegmentStart.X, SegmentStart.Y, SegmentEnd.X, SegmentEnd.Y, SecondLine->SegmentStart.X, SecondLine->SegmentStart.Y,
		SecondLine->SegmentEnd.X, SecondLine->SegmentEnd.Y, &x, &y);

	mgPoint InterceptionPoint;

	InterceptionPoint.Y = y;
	InterceptionPoint.X = x;

	if (Interception)
		*ValidIntercept = true;
	else
		*ValidIntercept = false;

	return InterceptionPoint;
}

// Calculate the lines normal facing the position.
mgVector mgLineSegment::SurfaceNormalFacingPosition(mgPoint Position)
{
	mgPoint MiddleofLine;
	mgVector Normal, PositionTowardsCenter;

	// Calculate the middle of our line
	MiddleofLine.Y = (SegmentEnd.Y - SegmentStart.Y) / 2 + SegmentStart.Y;
	MiddleofLine.X = (SegmentEnd.X - SegmentStart.X) / 2 + SegmentStart.X;

	// Calculate the vector from Position pointing at the center of the line.
	PositionTowardsCenter.VectorFromPoints(Position, MiddleofLine);
	std::cout << "Middle: " << MiddleofLine.Y << ", " << MiddleofLine.X << std::endl;

	std::cout << "Vector Towards Middle from [" << Position.Y << ", " << Position.X << "] is [" << PositionTowardsCenter.Y << ", " << PositionTowardsCenter.X << "]" << std::endl;

	return Normal;
}