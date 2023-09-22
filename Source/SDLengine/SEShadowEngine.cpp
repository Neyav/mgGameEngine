#include "SEShadowEngine.h"
#include "SERenderHandler.h"
#include "SEViewDisplay.h"
#include "SETextureHandler.h"

/**********************************************
**			SEShadowEngine					***
**											***
**	Responsible for generating and rendering***
**	shadow volumes from geometry passed to  ***
**	it.										***
**********************************************/


// Renders a right angle triangle on the screen using our texture. scales it, rotates it, and flips it as required.
void SEShadowEngine::renderRightTriangle(mgPoint Spine1, mgPoint Spine2, mgPoint P3)
{
	SDL_Point RotationAxis;
	SDL_RendererFlip Flip = SDL_FLIP_NONE;
	double rotationAngle = 0;
	double facingtestAngle = 0;
	int facingtestDiff = 0;
	mgLineSegment Axis[2];
	mgVector FacingTest;

	// Get our axis lines.
	Axis[0].ImportLine(Spine1, Spine2);
	Axis[0].Facing = LINEFACE_RIGHT; // Used for telling if we need to flip the triangle or not.
	Axis[1].ImportLine(Spine2, P3);

	RotationAxis.y = 0;
	RotationAxis.x = 0;

	rotationAngle = atan2(P3.Y - Spine2.Y, P3.X - Spine2.X) * 180 / mgPI;
	facingtestAngle = atan2(Spine1.Y - Spine2.Y, Spine1.X - Spine2.X) * 180 / mgPI;

	facingtestDiff = abs(rotationAngle - facingtestAngle);

	// Account for rounding errors.
	if (facingtestDiff == 89 || facingtestDiff == 269)
		facingtestDiff++;
	else if (facingtestDiff == 91 || facingtestDiff == 271)
		facingtestDiff--;

	if (facingtestDiff != 90 && facingtestDiff != 270)
	{
		// Not a right angle triangle.
		std::cout << "Not a right angle triangle." << std::endl;
		std::cout << "Spine1 [" << Spine1.Y << ", " << Spine1.X << "]" << std::endl;
		std::cout << "Spine2 [" << Spine2.Y << ", " << Spine2.X << "]" << std::endl;
		std::cout << "P3     [" << P3.Y << ", " << P3.X << "]" << std::endl;

		exit(0);
	}

	// Does this triangle shape match our image? 
	FacingTest.VectorFromPoints(P3, Spine2);

	RenderHandler->Texture[TEXTURE_SHADOWHULL]->setSize(Axis[1].SegmentLength, Axis[0].SegmentLength);

	if (FacingTest * Axis[0].NormalFacingPosition(P3) > 0)
		RenderHandler->Texture[TEXTURE_SHADOWHULL]->renderExt(Spine2.Y, Spine2.X, NULL, rotationAngle, &RotationAxis, SDL_FLIP_VERTICAL);
	else
		RenderHandler->Texture[TEXTURE_SHADOWHULL]->renderExt(Spine1.Y, Spine1.X, NULL, rotationAngle, &RotationAxis);
}

// Render a triangle to the screen by using a image of a 90 degree triangle on a quad
// and rotating/flipping two of them to get the shape of our custom triangle.
void SEShadowEngine::renderTriangle(int Y1, int X1, int Y2, int X2, int Y3, int X3)
{
	mgLineSegment TriangleLines[3];
	mgLineSegment TestLine;
	mgLineCollisionResults TestLineResults;
	mgPoint TrianglePoints[3];
	mgPoint SplitPoint[2]; // Our triangle split points.
	mgVector LineNormal;
	double LongestLength = 0.0;
	int LongestLine = 0;
		
	// Import the triangle into our data structures
	TrianglePoints[0].Y = Y1;
	TrianglePoints[0].X = X1;
	TrianglePoints[1].Y = Y2;
	TrianglePoints[1].X = X2;
	TrianglePoints[2].Y = Y3;
	TrianglePoints[2].X = X3;

	TriangleLines[0].ImportLine(TrianglePoints[0], TrianglePoints[1]);
	TriangleLines[1].ImportLine(TrianglePoints[1], TrianglePoints[2]);
	TriangleLines[2].ImportLine(TrianglePoints[2], TrianglePoints[0]);

	// Determine which line in our triangle is the longest, as we are going to use this line to split.
	for (int iterator = 0; iterator < 3; iterator++)
	{
		if (TriangleLines[iterator].SegmentLength > LongestLength)
		{	// This is the new longest line.
			LongestLength = TriangleLines[iterator].SegmentLength;
			LongestLine = iterator;
		}
	}

	// The first split point is the one not on the plane of the longest line,
	if (LongestLine == 0)
		SplitPoint[0] = TrianglePoints[2];
	else if (LongestLine == 1)
		SplitPoint[0] = TrianglePoints[0];
	else
		SplitPoint[0] = TrianglePoints[1];

	TriangleLines[LongestLine].Facing = LINEFACE_LEFT;

	LineNormal = TriangleLines[LongestLine].NormalFacingPosition(SplitPoint[0]);
	LineNormal.ReverseDirection();

	TestLine.ImportLine(SplitPoint[0], LineNormal, 50000);	// Rediculous length as a hack to ensure it collides. Doesn't effect speed any,
															// no reason to do real calculations.

	TestLineResults = TestLine.CollisionTest(&TriangleLines[LongestLine]);

	if (!TestLineResults.Collision)
	{	// This is a HUGE cheat. If there was no collision, then just reverse the normal direction and go again!
		LineNormal.ReverseDirection();
		TestLine.ImportLine(SplitPoint[0], LineNormal, 50000);
		TestLineResults = TestLine.CollisionTest(&TriangleLines[LongestLine]);
	}

	SplitPoint[1] = TestLineResults.CollisionPoint;

	// Render our two right angle triangles.
	this->renderRightTriangle(SplitPoint[0], SplitPoint[1], TriangleLines[LongestLine].SegmentEnd);
	this->renderRightTriangle(SplitPoint[0], SplitPoint[1], TriangleLines[LongestLine].SegmentStart);
}

void SEShadowEngine::drawShadowHull(mgLineSegment HullLine, mgPoint LightPosition)
{
	mgPoint MiddleofLine = HullLine.Middle(); // Grab the middle of the line.

	mgVector PointToLine; // A vector representing the direction from the point to the center of the line.

	PointToLine.VectorFromPoints(LightPosition, MiddleofLine);

	double DotProduct = PointToLine * HullLine.NormalFacingPosition(LightPosition); // Grab the dot product of the line and the vector to determine if it's facing away or towards us.

	// Facing opposite directions or perpendicular, not a shadow caster.
	if (DotProduct <= 0)
		return;

	// Shadow hulls are a projection of lines from the light source, towards the edges of the occluding line ( which is any line not facing the light, for simplicity )
	// This will give us four points. Two points of the occluding line, and two points that extend off the screen, projected from the light source against both
	// edges of the occluding line. This skewed quad will then be filled in with black triangles to mask the content underneath.

	// TODO: Make this all render to a shadowmap, and find a way to merge them together so we can have multiple light sources.

	// First we need to transfer the line from world space to screen space, we will do this using the render view context.
	mgPoint RenderStart, RenderEnd;
	RenderStart = HullLine.SegmentStart;
	RenderEnd = HullLine.SegmentEnd;

	RenderStart = ViewDisplay->convertToScreen(RenderStart);
	RenderEnd = ViewDisplay->convertToScreen(RenderEnd);
	LightPosition = ViewDisplay->convertToScreen(LightPosition);

	mgVector FirstArc, SecondArc;
	mgLineSegment FirstProjection, SecondProjection, Cross1, Cross2;
	mgLineCollisionResults CrossCollision;
	
	FirstArc.VectorFromPoints(LightPosition, RenderStart);
	SecondArc.VectorFromPoints(LightPosition, RenderEnd);
	FirstProjection.ImportLine(RenderStart, FirstArc, 1000);
	SecondProjection.ImportLine(RenderEnd, SecondArc, 1000);
	Cross1.ImportLine(RenderStart, SecondArc, 1000);
	Cross2.ImportLine(RenderEnd, FirstArc, 1000);

	CrossCollision = Cross1.CollisionTest(&Cross2);

	// The collision test failed ENSURING that the hull is going to be rendered improperly. Better off not to render it at all. Not sure how this is occuring, but it is.
	if (!CrossCollision.Collision)
		return;

	renderTriangle(RenderStart.Y, RenderStart.X, FirstProjection.SegmentEnd.Y, FirstProjection.SegmentEnd.X, SecondProjection.SegmentEnd.Y, SecondProjection.SegmentEnd.X);
	renderTriangle(RenderEnd.Y, RenderEnd.X, FirstProjection.SegmentEnd.Y, FirstProjection.SegmentEnd.X, SecondProjection.SegmentEnd.Y, SecondProjection.SegmentEnd.X);
	renderTriangle(RenderStart.Y, RenderStart.X, RenderEnd.Y, RenderEnd.X, CrossCollision.CollisionPoint.Y, CrossCollision.CollisionPoint.X);
}

SEShadowEngine::SEShadowEngine(SERenderHandler *Renderer, SEViewDisplay *Displayer)
{
	this->RenderHandler = Renderer;
	this->ViewDisplay = Displayer;
}

SEShadowEngine::~SEShadowEngine()
{

}