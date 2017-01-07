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
	// Calculate the pixel location of StartY, StartX
	int PixelY = (DisplayContext.ScreenHeight / 2) - ((DisplayContext.CenterY - DisplayContext.StartY) * DisplayContext.TileSizeY) - DisplayContext.PositionOffsetY;
	int PixelX = (DisplayContext.ScreenWidth / 2) - ((DisplayContext.CenterX - DisplayContext.StartX) * DisplayContext.TileSizeX) - DisplayContext.PositionOffsetX;

	// Convert RenderStart/End to a relative value of StartY/X
	ConversionPoint.Y -= DisplayContext.StartY;
	ConversionPoint.X -= DisplayContext.StartX;

	// Multiply by the tilesize and add the offset to get our pixel location.
	ConversionPoint.Y *= DisplayContext.TileSizeY;
	ConversionPoint.X *= DisplayContext.TileSizeX;
	ConversionPoint.Y -= DisplayContext.PositionOffsetY;
	ConversionPoint.X -= DisplayContext.PositionOffsetX;

	return ConversionPoint;
}

// Render a triangle to the screen by using a image of a 90 degree triangle on a quad
// and rotating/flipping two of them to get the shape of our custom triangle.
void renderTriangle(int Y1, int X1, int Y2, int X2, int Y3, int X3)
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

	// The second split point is where the normal of the longest line projected from the split point
	// would meet with the first split point. To figure this out we are going to get the normal of the longest line.
	// and then we are going to reverse it, project it from the first split point and determine where the collision occurs.
	LineNormal = TriangleLines[LongestLine].NormalFacingPosition(SplitPoint[0]);
	LineNormal.ReverseDirection();

	TestLine.ImportLine(SplitPoint[0], LineNormal, 5000);	// Rediculous length as a hack to ensure it collides. Doesn't effect speed any,
															// no reason to do real calculations.

	TestLineResults = TestLine.CollisionTest(&TriangleLines[LongestLine]);

	SplitPoint[1] = TestLineResults.CollisionPoint;
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

	//RenderHandler->Texture[SHADOWHULL_BOTTOMLEFT]->setSize(RenderEnd.X - RenderStart.X, RenderEnd.Y - RenderStart.Y);
	//RenderHandler->Texture[SHADOWHULL_BOTTOMLEFT]->render(RenderStart.Y, RenderStart.X, NULL);
	//if (DotProduct > 0)
	//	RenderHandler->Texture[SHADOWHULL_BOTTOMLEFT]->render(1, 1, NULL);
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

	/*mgLineSegment Test;
	mgPoint a, b;
	a.Y = 5.4;
	a.X = 5.4;
	b.Y = 7;
	b.X = 6;
	Test.ImportLine(a, b);
	Test.Facing = LINEFACE_RIGHT;

	drawShadowHull(this->Renderer, this->ViewContext, Test, Position);*/
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
