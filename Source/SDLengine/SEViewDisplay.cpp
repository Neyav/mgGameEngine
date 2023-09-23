#include "SERenderHandler.h"
#include "SETextureHandler.h"
#include "SEViewDisplay.h"
#include "SEShadowEngine.h"
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

mgPoint SEViewDisplay::convertToScreen(mgPoint ConversionPoint)
{
	double precisiony, precisionx;
	
	// Pixel location of StartY/StartX
	int PixelY = (ViewContext.ScreenHeight / 2) - ((ViewContext.CenterY - ViewContext.StartY) * ViewContext.TileSizeY) - ViewContext.PositionOffsetY;
	int PixelX = (ViewContext.ScreenWidth / 2) - ((ViewContext.CenterX - ViewContext.StartX) * ViewContext.TileSizeX) - ViewContext.PositionOffsetX;

	ConversionPoint.Y = PixelY + (ConversionPoint.Y - ViewContext.StartY) * ViewContext.TileSizeY;
	ConversionPoint.X = PixelX + (ConversionPoint.X - ViewContext.StartX) * ViewContext.TileSizeX;

	return ConversionPoint;
}

void SEViewDisplay::Initialize(SERenderHandler *RenderHandler, mgMapDataHandler *MapDataHandler, mgLinkedList<mgMapObject> *MOBJList)
{
	Renderer = RenderHandler;
	GameWorld = MapDataHandler;
	this->MOBJList = MOBJList;
}

void SEViewDisplay::InitializeShadowEngine(void)
{
	if (Renderer == nullptr)
		return; // Can't start up shadow engine without a renderer.

	ShadowEngine = new SEShadowEngine(Renderer, this);
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
	mgLinkedList<mgLineSegment> ShadowGeometry;

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
			{
				Renderer->Texture[TEXTURE_GAME_DEFAULTWALL]->render(PixelY - this->ViewContext.WalloffsetY, PixelX - this->ViewContext.WalloffsetX);

				mgMapElement *WorkingElement;
				WorkingElement = GameWorld->ReturnMapBlockReference(RenderY, RenderX);
				if (WorkingElement != nullptr)
				{
					mgListIterator<mgLineSegment> BlockGeometry = WorkingElement->BlockGeometry();
					while (!BlockGeometry.IteratorAtEnd())
					{
						mgLineSegment* WorkingLine = BlockGeometry.ReturnElementReference();
						// Add the line to our shadow geometry.
						ShadowGeometry.AddElementReference(WorkingLine, false);
					}
				}
			}

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

	mgListIterator<mgLineSegment> ShadowIterator;

	ShadowIterator.LinktoList(&ShadowGeometry);

	while (!ShadowIterator.IteratorAtEnd())
	{
		mgLineSegment *WorkingLine = ShadowIterator.ReturnElementReference();
		ShadowEngine->drawShadowHull(*WorkingLine, Position);
	}

}

SEViewDisplay::SEViewDisplay()
{
	Renderer = nullptr;
	GameWorld = nullptr;
	MOBJList = nullptr;
	ShadowEngine = nullptr;

	// These defaults will ensure the first calculation is done 100%
	// TODO: This can be handled better.
	ViewContext.Position.Y = ViewContext.Position.X = -100;
	ViewContext.zoom = -100;
}

SEViewDisplay::~SEViewDisplay()
{
	if (ShadowEngine != nullptr)
		delete ShadowEngine;
}
