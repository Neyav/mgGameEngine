#ifndef MGCOLLISIONDETECTIONH
#define MGCOLLISIONDETECTIONH

#include <stdlib.h>

#include "mgMapDataHandler.h"
#include "mgMapObject.h"
#include "mgMapElement.h"
#include "mgVectorPoint.h"
#include "mgLineSegment.h"
#include "Containers/mgLinkedList.h"

// Stage 1 - 	Collect information about the movement that is to occur. The object that is moving, primarily,
// 		and the momentum that object is moving with.

// Stage 2 -	Determine the possible area of effect. This is going to be the 8 block area around the block the
//		object resides in, and the immediate block that the object resides in by default. There will be a
//		tunable in case this needs to be increased, or for some reason there is a desire to decrease it.

// Stage 3 -	Add all the lines in these blocks to a list of potential impact lines, including map objects that are inside the blocks in question.
//		Potentially use mgVisibilyMap data to verify that the block is reachable from the center block as a quick
//		means of ruling unusable information out. This will require that maps have full mgVisibilty profiles generated and
//		that in turn means changes in map structure will require a reprocessing of that visibility map.
//		I'm thinking of making the visibilty map a pointer in the map block, which if it is a nullptr, means visibility detection
//		is done manually. That way we get the best of both worlds.

// Stage 4 -	Project a line from each point of the map object's structure out towards the momentum vector. Save any collisions
//		in a list of collisions. Then reverse the direction of the vector and project it out in a similar fashion off the
//		points from the lines that are considered potential occluders. These ones will only be tested against the lines of
//		the map object only.

// Stage 5 - 	Go over the list of collisions to find the collision with the smallest distance delta. This is the first line the
//		movement of the object would have impacted. Reduce the magnitude of the momentum vector by
//		( momentum vector magnitude - distance delta ) + 0.000001 ( or some other safety value to make sure you aren't inside
//		the wall ) and then process the movement. Allow, as accessible, information pertaining to the object or map block collided
//		with as this code is intended to be used for item pickups as well. If the collision is resolved by the removal of another
//		object then reprocessing the remainder will be possible by calling the function again on the remaining leftover momentum.
//		The return of the line that was impacted will also be useful for calculating the normal vector that we need to adjust the
//		objects total momentum ( not just the movement that was processed ) so future tests have it slide against the line instead of
//		just come to a dead halt.

struct mgDetectedCollision
{
	mgPoint PointOfCollision;
	mgLineSegment *CollisionLine;
	mgVector CollisionCorrection;
};

class mgCollisionDetection
{
	private:

	mgMapObject *MovingObject;
	mgVector RemainingMovement;

	mgLinkedList<mgMapElement> MapElements;
	mgLinkedList<mgLineSegment> CollisionLines;
	mgLinkedList<mgDetectedCollision> DetectedCollisions;

	public:

	mgMapDataHandler *MapReference;

	void CollisionSetup(mgMapObject *MovingObject, mgVector Movement); // Stage One
	void SetupDetectionArea(unsigned int Range); // Stage Two
	void AggregateCollisionLines(void); // Stage Three
	void PerformCollisionTests(void); // Stage Four

	mgCollisionDetection();
};

#endif
