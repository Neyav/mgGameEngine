#ifndef MGVISIBILITYMAPH
#define MGVISIBILITYMAPH

#define USEBINARYTREE

#include "mgVectorPoint.h"
#include "mgMapDataHandler.h"
#ifdef USEBINARYTREE
#include "mgBinaryTree.h"
#else
#include "mgLinkedList.h"
#endif

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
#ifdef USEBINARYTREE
	mgBinaryTree<mgPoint> VisibilityTree;
#else
	mgLinkedList<mgPoint> VisibilityList;
#endif
	mgMapDataHandler *MapReference;
	int VisiblePoints;

	void AddVisiblePoint(int AddY, int AddX);
	void AddVisiblePoint(mgPoint Point);
public:

	bool IsMarkedVisible(int CheckY, int CheckX);
	int MappedItems(void);
	mgPoint SequentialObtainItem(void);
	void SequentialMarkerReset(void);
	void MergeMaps(mgVisibilityMap *OtherMap);
	void LinkToMapHandler(mgMapDataHandler *LinkMap);
	void CalculateVisibility(int BlockY, int BlockX);
	void CalculateAdjacentVisibility(int BlockY, int BlockX);

	mgVisibilityMap(); // Constructor
	~mgVisibilityMap(); // Destructor
};

#endif
