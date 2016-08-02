#include <math.h>
#include <stdlib.h>

#include "SEMovementHandler.h"

#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../mgLineSegment.h"
#include "../mgVectorPoint.h"
#include "../mgCollisionDetection.h"
#include "../Containers/mgLinkedList.h"

/************************************************
 *		SEMovementHandler						*
 *	Responsible for processing MOBJList			*
 *	movement based on momentum and handling		*
 *	both the collisions and the atrophy of		*
 *	momentum.									*
 ************************************************/

void SEMovementHandler::ProcessMomentum(void)
{
	mgListIterator<mgMapObject> MOBJIterator(MOBJList);

	while (!MOBJIterator.IteratorAtEnd())
	{
		mgMapObject *MobileObject = MOBJIterator.ReturnElementReference();
		mgVector Movement = MobileObject->Momentum;

 		while (Movement.Y != 0 || Movement.X != 0) // There is attempted movement.
		{
			// Let's check for collisions first.
			mgCollisionDetection CollisionTest;
			mgDetectedCollision Results;

			CollisionTest.MapReference = GameWorld;
			Results = CollisionTest.CollisionTest(MobileObject, Movement, 2);

			if (Results.Collision == false) // No collision
			{
				MobileObject->Position.Y += Movement.Y;
				MobileObject->Position.X += Movement.X;

				Movement.Y = Movement.X = 0; // We completed the move.
			}
			else
			{	// Handle it.				
				mgVector Projected;
				double dotproduct;

				// Push us 0.0000001 units away from the wall. Moments like this make me regret using
				// double point precision and not a fixed fractional unit size like everyone else does. sigh.
				Results.CollisionCorrection.NormalizeVector(Results.CollisionCorrection.Magnitude + 0.00001);

				// Complete the movement in a manner that doesn't have us clip through the wall.
				Movement = Movement + Results.CollisionCorrection;

				MobileObject->Position.Y += Movement.Y;
				MobileObject->Position.X += Movement.X;

				Movement = Results.CollisionCorrection; // The remainder of the movement we want to attempt.
				Movement.ReverseDirection(); // We want this to go back into the wall we collided with.

				// Lose all velocity in the direction of the wall normal by projecting the direction onto it.
				dotproduct = Movement * Results.CollisionNormal;
				Projected = Results.CollisionNormal * dotproduct;
				Movement = Movement - Projected;
			}
		}

		// ATROPHY MOMENTUM
		MobileObject->Momentum = MobileObject->Momentum * 0.9;

		if (MobileObject->Momentum.Magnitude < 0.001 )
			MobileObject->Momentum.Y = MobileObject->Momentum.X = MobileObject->Momentum.Magnitude = 0;
	}
}

void SEMovementHandler::Initialize(mgMapDataHandler *Map, mgLinkedList<mgMapObject> *MOBJList)
{
	this->GameWorld = Map;
	this->MOBJList = MOBJList;
}

SEMovementHandler::SEMovementHandler()
{
	GameWorld = nullptr;
	MOBJList = nullptr;
}
