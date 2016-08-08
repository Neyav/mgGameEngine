#ifndef MGMAPDATAHANDLERH
#define MGMAPDATAHANDLERH

#include "mgMapElement.h"

struct mgMapElementArray
{
	mgMapElement *MapBlock;
};

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
	mgMapElementArray *InternalMapData;
	int MapSizeY, MapSizeX;

	void CleanHouseProtocol(void);  // JARVIS.....

public:

	bool PositionBoundsCheck(int PosY, int PosX);
	void InitalizeMapData(int SizeY, int SizeX);
	void CloneMapIntoHost(mgMapDataHandler *MapToClone);
	bool IsBlockClippable(int PosY, int PosX);
	mgMapElement *ReturnMapBlockReference(int PosY, int PosX);
	int mapsizeY(void);
	int mapsizeX(void);

	mgMapDataHandler();
	~mgMapDataHandler();

};
#endif
