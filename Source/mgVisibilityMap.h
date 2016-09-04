#ifndef MGVISIBILITYMAPH
#define MGVISIBILITYMAPH

#include "mgVectorPoint.h"
#include "mgMapDataHandler.h"
#include "Containers/mgRBTBinaryTree.h"

#define MGVISIBILITYEDGEPERCISION 0.49999	// How far from the center to move to calculate the visibility
											// from the edge of a block. As close as reasonable to the line without going over.

// =------------------------------------=
// = mgVisibilityMap C++ class			=
// =------------------------------------=
//
// Uses the mgVector class to calculate and store blocks that are visible from a principal source block
// They can be merged together easily and checked quickly. Intended for any renderer that only wants to display
// relevant blocks to a person standing on any particular block.
class mgVisibilityMap
{
private:
	mgRBTBinaryTree<mgPoint> VisibilityTree;
	mgMapDataHandler *MapReference;
	int VisiblePoints;

	void AddVisiblePoint(int AddY, int AddX);
	void AddVisiblePoint(mgPoint Point);
public:

	bool IsMarkedVisible(int CheckY, int CheckX);
	int MappedItems(void);
	void LinkToMapHandler(mgMapDataHandler *LinkMap);
	void CalculateVisibility(mgPoint CheckPosition);
	void CalculateVisibilityBlock(int BlockY, int BlockX);

	mgVisibilityMap(); // Constructor
	~mgVisibilityMap(); // Destructor
};

#endif
