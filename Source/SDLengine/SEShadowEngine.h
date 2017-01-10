#ifndef SESHADOWENGINE_H
#define SESHADOWENGINE_H

#include "../mgVectorPoint.h"
#include "../mgLineSegment.h"

#include "SERenderHandler.h"
#include "SEViewDisplay.h"
#include "SETextureHandler.h"

class SEViewDisplay;

class SEShadowEngine
{
private:
	SERenderHandler *RenderHandler;
	SEViewDisplay *ViewDisplay;

	void renderRightTriangle(mgPoint Spine1, mgPoint Spine2, mgPoint P3);
	void renderTriangle(int Y1, int X1, int Y2, int X2, int Y3, int X3);
public:
	void drawShadowHull(mgLineSegment HullLine, mgPoint LightPosition);

	SEShadowEngine(SERenderHandler *Renderer, SEViewDisplay *Displayer);
	~SEShadowEngine();
};

#endif
