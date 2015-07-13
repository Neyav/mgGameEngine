#ifndef MGCOLLISIONDETECTIONH
#define MGCOLLISIONDETECTIONH

#include <stdlib.h>

#include "mgMapDataHandler.h"
#include "mgMapObject.h"
#include "mgMapElement.h"
#include "mgVectorPoint.h"
#include "mgLineSegment.h"
#include "Containers/mgLinkedList.h"

struct mgDetectedCollision
{
	mgPoint PointOfCollision;
	mgLineSegment *CollisionLine;
	mgVector CollisionCorrection;
	mgVector CollisionNormal;
	bool Collision;
};

// This is used so we don't lose track of the collision object when we do the trace from the world against the object.
class mgCollisionPoint
{
	public:

	mgPoint Position;
	mgLineSegment *LineReference;

	// Comparative overloads so that this class works within the binary tree.
	bool operator>(const mgCollisionPoint& other)
	{
		if (Position > other.Position)
			return true;
		return false;
	}
	bool operator<(const mgCollisionPoint& other)
	{
		if (Position < other.Position)
			return true;
		return false;
	}
	bool operator==(const mgCollisionPoint& other)
	{
		if (Position == other.Position)
			return true;
		return false;
	}
};

//
// =------------------------------------=
// = mgCollisionDetection C++ class     =
// =------------------------------------=
//
// This class tests a MapObject that is moving against the world and reports back the results of the
// nature of the collision, and information relevant to moving the object to a safe position.

class mgCollisionDetection
{
	private:

	mgMapObject *MovingObject;
	mgVector AttemptedMovement;

	mgLinkedList<mgMapElement> MapElements;
	mgLinkedList<mgLineSegment> CollisionLines;
	mgLinkedList<mgDetectedCollision> DetectedCollisions;

	public:

	mgMapDataHandler *MapReference;

	void CollisionSetup(mgMapObject *MovingObject, mgVector Movement); // Stage One
	void SetupDetectionArea(unsigned int Range); // Stage Two
	void AggregateCollisionLines(void); // Stage Three
	void PerformCollisionTestsP1(void); // Stage Four: Part One
	void PerformCollisionTestsP2(void); // Stage Four: Part Two
	mgDetectedCollision CollisionTest(mgMapObject *MovingObject, mgVector Movement, unsigned int Range); // Stage Five

	mgCollisionDetection();
};

#endif
