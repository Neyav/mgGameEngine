#ifndef MGMAPDATAHANDLERH
#define MGMAPDATAHANDLERH

#include "mgMapElement.h"

// =------------------------------------=
// = mgMapDataHandler C++ class         =
// =------------------------------------=
//
// The container for the map data. Creates a dynamically allocated memory block based on the definition of MapElement_s
// and then manages both accessing it, writing to it, and recovery of information from it. Just a general encapsulor for the
// map data.
class mgMapDataHandler
{
private:
	mgMapElement *InternalMapData;
	int MapSizeY, MapSizeX;

public:

	bool PositionBoundsCheck(int PosY, int PosX);
	void InitalizeMapData(int SizeY, int SizeX);
	void CloneMapIntoHost(mgMapDataHandler *MapToClone);
	bool IsBlockClippable(int PosY, int PosX);
	bool WillObjectFit(double PosY, double PosX, float ObjectSize);
	mgMapElement *ReturnMapBlockReference(int PosY, int PosX);
	//mgMapElement ReturnMapBlock(int PosY, int PosX);
	//void SetMapBlock(int PosY, int PosX, mgMapElement MapBlock);
	int mapsizeY(void);
	int mapsizeX(void);
};
#endif