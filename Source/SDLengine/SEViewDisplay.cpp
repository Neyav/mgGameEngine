#include "SERenderHandler.h"
#include "SETextureHandler.h"
#include "SEViewDisplay.h"
#include "GameGlobals.h"
#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../Containers/mgLinkedList.h"

/************************************************
 *		SEViewDisplay                   *
 *	Responsible for setting up the screen   *
 *	and rendering the gameworld upon it.    *
 ************************************************/

mgPoint convertToScreen(mgPoint ConversionPoint, SEViewDisplayContext DisplayContext)
{
	double precisiony, precisionx;
	
	// Pixel location of StartY/StartX
	int PixelY = (DisplayContext.ScreenHeight / 2) - ((DisplayContext.CenterY - DisplayContext.StartY) * DisplayContext.TileSizeY) - DisplayContext.PositionOffsetY;
	int PixelX = (DisplayContext.ScreenWidth / 2) - ((DisplayContext.CenterX - DisplayContext.StartX) * DisplayContext.TileSizeX) - DisplayContext.PositionOffsetX;

	ConversionPoint.Y = PixelY + (ConversionPoint.Y - DisplayContext.StartY) * DisplayContext.TileSizeY;
	ConversionPoint.X = PixelX + (ConversionPoint.X - DisplayContext.StartX) * DisplayContext.TileSizeX;

	return ConversionPoint;
}

// Renders a right angle triangle on the screen using our texture. scales it, rotates it, and flips it as required.
void renderRightTriangle(mgPoint Spine1, mgPoint Spine2, mgPoint P3, SERenderHandler *RenderHandler)
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

	std::cout << rotationAngle << " " << facingtestAngle << "(" << rotationAngle - facingtestAngle << " - " << facingtestAngle - rotationAngle << ")" << std::endl;

	facingtestDiff = abs(rotationAngle - facingtestAngle);

	// Account for rounding errors.
	if ( facingtestDiff == 89 || facingtestDiff == 269 )
		facingtestDiff++;
	else if ( facingtestDiff == 91 || facingtestDiff == 271 )
		facingtestDiff--;

	std::cout << facingtestDiff << std::endl;

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

	if (FacingTest * Axis[0].NormalFacingPosition(P3) > 0)
		Flip = SDL_FLIP_VERTICAL;

	RenderHandler->Texture[TEXTURE_SHADOWHULL]->setSize(5, 5);
	RenderHandler->Texture[TEXTURE_SHADOWHULL]->render(Spine1.Y, Spine1.X);
	RenderHandler->Texture[TEXTURE_SHADOWHULL]->render(Spine2.Y, Spine2.X);
	RenderHandler->Texture[TEXTURE_SHADOWHULL]->render(P3.Y, P3.X);

	RenderHandler->Texture[TEXTURE_SHADOWHULL]->setSize(Axis[1].SegmentLength, Axis[0].SegmentLength);
	if (Flip == SDL_FLIP_VERTICAL)
		RenderHandler->Texture[TEXTURE_SHADOWHULL]->renderExt(Spine2.Y, Spine2.X, NULL, rotationAngle, &RotationAxis, Flip);
	else
		RenderHandler->Texture[TEXTURE_SHADOWHULL]->renderExt(Spine1.Y, Spine1.X, NULL, rotationAngle, &RotationAxis, Flip);
}

// Render a triangle to the screen by using a image of a 90 degree triangle on a quad
// and rotating/flipping two of them to get the shape of our custom triangle.
void renderTriangle(int Y1, int X1, int Y2, int X2, int Y3, int X3, SERenderHandler *RenderHandler)
{
	mgLineSegment TriangleLines[3];
	mgLineSegment TestLine;
	mgLineCollisionResults TestLineResults;
	mgPoint TrianglePoints[3];
	mgPoint SplitPoint[2]; // Our triangle split points.
	mgVector LineNormal;
	double LongestLength = 0.0;
	int LongestLine = 0;

	std::cout << "Triangle IN: [" << Y1 << ", " << X1 << "] [" << Y2 << ", " << X2 << "] [" << Y3 << ", " << X3 << "]" << std::endl;

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

	std::cout << "Triangle Lengths: 0:[" << TriangleLines[0].SegmentLength << "] 1:[" << TriangleLines[1].SegmentLength << "] 2:[" << TriangleLines[2].SegmentLength << "]" << std::endl;

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

	std::cout << "Longest Line: " << LongestLine << " Normal: " << LineNormal.Y << ", " << LineNormal.X << std::endl;

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
	renderRightTriangle(SplitPoint[0], SplitPoint[1], TriangleLines[LongestLine].SegmentEnd, RenderHandler);
	renderRightTriangle(SplitPoint[0], SplitPoint[1], TriangleLines[LongestLine].SegmentStart, RenderHandler);

	// Now we are going to draw the first triangle. The spine of it will be between both split points, with the top being SplitPoint[0].
	// So the height of our sprite will be the distance between the split points.
	// The width will be the length from SplitPoint[1] towards the longest line's segment end.
	// The rotation is going to be the angle from SplitPoint[1] to the longst line's segment end.

	// Our first question is going to be, do we need to flip the right angle triangle sprite to be able to represent this triangle?
	// To get the answer we are going to make a line out of the spine from top to bottom ( bottom = longestline ) with the facing set
	// to right, and then test the dot product of it against the extended point to see if it faces the interior of the triangle line, or the exterior.
	// If it is the exterior then we need to flip it. flip it good. Whatever answer we get here, the opposite will be true for the second triangle.
}

// TODO: Break into it's own class, just here as a proof of concept.
void drawShadowHull(SERenderHandler *RenderHandler, SEViewDisplayContext DisplayContext, mgLineSegment HullLine, mgPoint LightPosition)
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

	RenderStart = convertToScreen(RenderStart, DisplayContext);
	RenderEnd = convertToScreen(RenderEnd, DisplayContext);
	LightPosition = convertToScreen(LightPosition, DisplayContext);

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

	renderTriangle(RenderStart.Y, RenderStart.X, FirstProjection.SegmentEnd.Y, FirstProjection.SegmentEnd.X, SecondProjection.SegmentEnd.Y, SecondProjection.SegmentEnd.X,RenderHandler);
	renderTriangle(RenderEnd.Y, RenderEnd.X, FirstProjection.SegmentEnd.Y, FirstProjection.SegmentEnd.X, SecondProjection.SegmentEnd.Y, SecondProjection.SegmentEnd.X, RenderHandler);
	renderTriangle(RenderStart.Y, RenderStart.X, RenderEnd.Y, RenderEnd.X, CrossCollision.CollisionPoint.Y, CrossCollision.CollisionPoint.X, RenderHandler);
}

void SEViewDisplay::Initialize(SERenderHandler *RenderHandler, mgMapDataHandler *MapDataHandler, mgLinkedList<mgMapObject> *MOBJList)
{
	Renderer = RenderHandler;
	GameWorld = MapDataHandler;
	this->MOBJList = MOBJList;
}

// Setup the context for the current render pass. This information only needs to be changed when either the position changes, or the zoom changes.
void SEViewDisplay::SetupViewContext(mgPoint Position, double zoom)
{
	bool forcechange = false; // Triggered if everything needs to be recalculated.

	// If the screen resolution changes, recalculate everything.
	if (this->ViewContext.ScreenWidth != Renderer->getWidth() ||
			this->ViewContext.ScreenHeight != Renderer->getHeight())
	{
		forcechange = true;
		this->ViewContext.ScreenWidth = Renderer->getWidth();
		this->ViewContext.ScreenHeight = Renderer->getHeight();
	}

	// Zoom has changed, recalculate all zoom related context information.
	if (zoom != this->ViewContext.zoom || forcechange)
	{
		// Calculate the size of a tile on the screen in pixels.
		this->ViewContext.TileSizeY = mgFloor(TILEY * zoom);
		this->ViewContext.TileSizeX = mgFloor(TILEX * zoom);

		// Calculate the pixel offset of a wall tile on the screen in pixels.
		this->ViewContext.WalloffsetY = mgFloor(TILEYOFFSET * zoom);
		this->ViewContext.WalloffsetX = mgFloor(TILEXOFFSET * zoom);

		// Calculate how many tiles are going to be rendered.
		this->ViewContext.TilesY = (int)(this->ViewContext.ScreenHeight / this->ViewContext.TileSizeY) + 2;
		this->ViewContext.TilesX = (int)(this->ViewContext.ScreenWidth / this->ViewContext.TileSizeX) + 2;

		// Clip based on our maximum boundries.
		if (this->ViewContext.TilesY > MAXTILESY)
			this->ViewContext.TilesY = MAXTILESY;
		if (this->ViewContext.TilesX > MAXTILESX)
			this->ViewContext.TilesX = MAXTILESX;
	}

	// Position has changed, recalculate all position related context information.
	if (Position != this->ViewContext.Position || forcechange)
	{
		// Calculate which tile is the center of the screen.
		this->ViewContext.CenterY = mgFloor(Position.Y);
		this->ViewContext.CenterX = mgFloor(Position.X);
	}

	// Zoom or position has changed, recalculate information reliant on both.
	if (Position != this->ViewContext.Position || zoom != this->ViewContext.zoom || forcechange)
	{
		// The offset is the size of a tile * the fractional portion of the position.
		this->ViewContext.PositionOffsetY = (int)((double)this->ViewContext.TileSizeY * (double)(Position.Y - this->ViewContext.CenterY));
		this->ViewContext.PositionOffsetX = (int)((double)this->ViewContext.TileSizeX * (double)(Position.X - this->ViewContext.CenterX));

		this->ViewContext.StartY = this->ViewContext.CenterY - (int)(this->ViewContext.TilesY / 2);
		this->ViewContext.StopY = this->ViewContext.CenterY + (int)(this->ViewContext.TilesY / 2);
		this->ViewContext.StartX = this->ViewContext.CenterX - (int)(this->ViewContext.TilesX / 2);
		this->ViewContext.StopX = this->ViewContext.CenterX + (int)(this->ViewContext.TilesX / 2);
	}

	// Save these so we can compare changes to determine what needs recalculating.
	this->ViewContext.Position = Position;
	this->ViewContext.zoom = zoom;
	
}

void SEViewDisplay::RenderWorld(mgPoint Position, double zoom)
{
	// Determine any changes that require recalculating the context and do it.
	this->SetupViewContext(Position, zoom);

	mgLinkedList<mgMapObject> VisibleElements[MAXTILESY][MAXTILESX];

	{
		// Go through the global map object list and fill our visible array with map objects in screen space.
		mgListIterator<mgMapObject> MOBJ_Iterator(MOBJList);
		while (!MOBJ_Iterator.IteratorAtEnd()) 
		{
			mgMapObject *WorkingObject = MOBJ_Iterator.ReturnElementReference();
			int PositionY = mgFloor(WorkingObject->Position.Y);
			int PositionX = mgFloor(WorkingObject->Position.X);

			if (PositionY >= this->ViewContext.StartY && PositionY <= this->ViewContext.StopY)
			{
				if (PositionX >= this->ViewContext.StartX && PositionX <= this->ViewContext.StopX)
				{
					// Object is in visible scope, add it to our array.
					VisibleElements[PositionY - this->ViewContext.StartY][PositionX - this->ViewContext.StartX].AddElementReference(WorkingObject, false);
				}
			}
		}
	}

	// Scale textures	
	Renderer->Texture[TEXTURE_GAME_DEFAULTFLOOR]->setSize(this->ViewContext.TileSizeY, this->ViewContext.TileSizeX);
	Renderer->Texture[TEXTURE_GAME_DEFAULTWALL]->setSize(this->ViewContext.TileSizeY + this->ViewContext.WalloffsetY, this->ViewContext.TileSizeX + this->ViewContext.WalloffsetX);

	int PixelY = (this->ViewContext.ScreenHeight / 2) - ((this->ViewContext.CenterY - this->ViewContext.StartY) * this->ViewContext.TileSizeY) - this->ViewContext.PositionOffsetY;
	int PixelX = (this->ViewContext.ScreenWidth / 2) - ((this->ViewContext.CenterX - this->ViewContext.StartX) * this->ViewContext.TileSizeX) - this->ViewContext.PositionOffsetX;

	// Render the floor tiles	
	for (int RenderY = this->ViewContext.StartY; RenderY <= this->ViewContext.StopY; RenderY++)
	{
		for (int RenderX = this->ViewContext.StartX; RenderX <= this->ViewContext.StopX; RenderX++)
		{
			Renderer->Texture[TEXTURE_GAME_DEFAULTFLOOR]->render(PixelY, PixelX);
			PixelX += this->ViewContext.TileSizeX;
		}
		PixelX = (this->ViewContext.ScreenWidth / 2) - ((this->ViewContext.CenterX - this->ViewContext.StartX) * this->ViewContext.TileSizeX) - this->ViewContext.PositionOffsetX;
		PixelY += this->ViewContext.TileSizeY;
	}

	PixelY = (this->ViewContext.ScreenHeight / 2) - ((this->ViewContext.CenterY - this->ViewContext.StartY) * this->ViewContext.TileSizeY) - this->ViewContext.PositionOffsetY;
	PixelX = (this->ViewContext.ScreenWidth / 2) - ((this->ViewContext.CenterX - this->ViewContext.StartX) * this->ViewContext.TileSizeX) - this->ViewContext.PositionOffsetX;

	// Render the wall tiles and any objects
	for (int RenderY = this->ViewContext.StartY; RenderY <= this->ViewContext.StopY; RenderY++)
	{
		for (int RenderX = this->ViewContext.StartX; RenderX <= this->ViewContext.StopX; RenderX++)
		{
			mgListIterator<mgMapObject> MOBJ_Iterator;
			MOBJ_Iterator.LinktoList(&VisibleElements[RenderY - this->ViewContext.StartY][RenderX - this->ViewContext.StartX]);

			if (GameWorld->IsBlockClippable(RenderY, RenderX)) // Simple check to see if it's a wall, we should really be doing a check for map element type.
				Renderer->Texture[TEXTURE_GAME_DEFAULTWALL]->render(PixelY - this->ViewContext.WalloffsetY, PixelX - this->ViewContext.WalloffsetX);

			// Check our map object list to see if any map objects reside here.
			while (!MOBJ_Iterator.IteratorAtEnd())
			{
				mgMapObject *WorkingObject = MOBJ_Iterator.ReturnElementReference();
				int offsetY = PixelY;
				int offsetX = PixelX;

				// Determine our render position.
				offsetY += (int)((double)(WorkingObject->Position.Y - (double)RenderY - WorkingObject->ObjectSize) * this->ViewContext.TileSizeY);
				offsetX += (int)((double)(WorkingObject->Position.X - (double)RenderX - WorkingObject->ObjectSize) * this->ViewContext.TileSizeX);

				switch (WorkingObject->ObjectType)
				{
				case MOBJ_PLAYER:
					Renderer->Texture[TEXTURE_GAME_PLAYER]->setSize(this->ViewContext.TileSizeY * (WorkingObject->ObjectSize * 2), this->ViewContext.TileSizeX * (WorkingObject->ObjectSize * 2)); // Scale sprite
					Renderer->Texture[TEXTURE_GAME_PLAYER]->render(offsetY, offsetX);
					break;
				default:
					break; // No sprite.
				}
			}

			PixelX += this->ViewContext.TileSizeX;
		}

		PixelY += this->ViewContext.TileSizeY;
		PixelX = (this->ViewContext.ScreenWidth / 2) - ((this->ViewContext.CenterX - this->ViewContext.StartX) * this->ViewContext.TileSizeX) - this->ViewContext.PositionOffsetX;
	}

	mgLineSegment Test;
	mgPoint a, b;
	a.Y = 3;
	a.X = 3;
	b.Y = 4;
	b.X = 4;
	Test.ImportLine(a, b);
	Test.Facing = LINEFACE_RIGHT;

	drawShadowHull(this->Renderer, this->ViewContext, Test, Position);
	//renderTriangle(80, 10, 5, 40, 80, 90, Renderer);

}

SEViewDisplay::SEViewDisplay()
{
	Renderer = nullptr;
	GameWorld = nullptr;
	MOBJList = nullptr;

	// These defaults will ensure the first calculation is done 100%
	// TODO: This can be handled better.
	ViewContext.Position.Y = ViewContext.Position.X = -100;
	ViewContext.zoom = -100;
}
