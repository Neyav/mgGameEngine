#ifndef MGMAPDATAHANDLERH
#define MGMAPDATAHANDLERH

#include "mgMapElement.h"

// Since this is practically considered the base element of the game engine it makes sense to keep version
// information here.
struct _mgGameEngineVersionstruct
{
	unsigned int ReleaseVersion;
	unsigned int FeatureVersion;
	unsigned int PatchVersion;
};

extern _mgGameEngineVersionstruct mgGameEngineVersion;

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
	bool WillObjectFit(double PosY, double PosX, float ObjectSize);
	mgMapElement *ReturnMapBlockReference(int PosY, int PosX);
	int mapsizeY(void);
	int mapsizeX(void);

	mgMapDataHandler();
	~mgMapDataHandler();

};
#endif