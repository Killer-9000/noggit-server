#pragma once

#include "DBCFile.h"
#include <string>

class AreaDB : public DBCFile
{
public:
	AreaDB() : DBCFile("./data/DBFilesClient/AreaTable.dbc")
	{ }

	/// Fields
	static const size_t AreaID = 0;		// uint
	static const size_t Continent = 1;	// uint
	static const size_t Parent = 2;		// uint [AreaID]
	static const size_t Flags = 4;		// bit field
	static const size_t Name = 11;		// localisation string

	static std::string	 GetAreaName(int areaID);
	static std::uint32_t GetAreaParent(int areaID);
};

class MapDB : public DBCFile
{
public:
	MapDB() : DBCFile("./data/DBFilesClient/Map.dbc")
	{ }

	/// Fields
	static const size_t MapID = 0;				// uint
	static const size_t InternalName = 1;		// string
	static const size_t AreaType = 2;			// uint
	static const size_t IsBattleground = 4;		// uint
	static const size_t Name = 5;				// loc
	static const size_t LoadingScreen = 57;		// uint [LoadingScreen]

	static std::string GetMapName(int mapID);
};

class LoadingScreensDB : public DBCFile
{
public:
	LoadingScreensDB() : DBCFile("./data/DBFilesClient/LoadingScreens.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Name = 1;			// string
	static const size_t Path = 2;			// string
};

class LightDB : public DBCFile
{
public:
	LightDB() : DBCFile("./data/DBFilesClient/Light.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Map = 1;			// uint
	static const size_t PositionX = 2;		// float
	static const size_t PositionY = 3;		// float
	static const size_t PositionZ = 4;		// float
	static const size_t RadiusInner = 5;	// float
	static const size_t RadiusOuter = 6;	// float
	static const size_t DataIDs = 7;		// uint[8]
};

class LightParamsDB : public DBCFile{
public:
	LightParamsDB() : DBCFile("./data/DBFilesClient/LightParams.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t skybox = 2;			// uint ref to LightSkyBox
	static const size_t water_shallow_alpha = 5;
	static const size_t water_deep_alpha = 6;
	static const size_t ocean_shallow_alpha = 7;
	static const size_t ocean_deep_alpha = 8;
};

class LightSkyboxDB : public DBCFile
{
public:
	LightSkyboxDB() : DBCFile("./data/DBFilesClient/LightSkybox.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t filename = 1;		// string
	static const size_t flags = 2;			// uint
};

class LightIntBandDB : public DBCFile
{
public:
	LightIntBandDB() : DBCFile("./data/DBFilesClient/LightIntBand.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Entries = 1;		// uint
	static const size_t Times = 2;			// uint
	static const size_t Values = 18;		// uint
};

class LightFloatBandDB : public DBCFile
{
public:
	LightFloatBandDB() : DBCFile("./data/DBFilesClient/LightFloatBand.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Entries = 1;		// uint
	static const size_t Times = 2;			// uint
	static const size_t Values = 18;		// float
};

class GroundEffectTextureDB : public DBCFile
{
public:
	GroundEffectTextureDB() : DBCFile("./data/DBFilesClient/GroundEffectTexture.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Doodads = 1;		// uint[4]
	static const size_t Weights = 5;		// uint[4]
	static const size_t Amount = 9;			// uint
	static const size_t TerrainType = 10;	// uint
};

class GroundEffectDoodadDB : public DBCFile
{
public:
	GroundEffectDoodadDB() : DBCFile("./data/DBFilesClient/GroundEffectDoodad.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t InternalID = 1;		// uint
	static const size_t Filename = 2;		// string
};

class LiquidTypeDB : public DBCFile
{
public:
	LiquidTypeDB() : DBCFile("./data/DBFilesClient/LiquidType.dbc")
	{ }

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Name = 1;			// string
	static const size_t Type = 3;			// uint
	static const size_t ShaderType = 14;	// uint
	static const size_t TextureFilenames = 15;		// string[6]
	static const size_t TextureTilesPerBlock = 23;	// uint
	static const size_t Rotation = 24;	// uint
	static const size_t AnimationX = 23;	// uint
	static const size_t AnimationY = 24;	// uint

	static int GetLiquidType(int liquidID);
	static std::string GetLiquidName(int liquidID);
};

void OpenDBs();

std::string getGroundEffectDoodad(unsigned int effectID, int DoodadNum);

extern AreaDB gAreaDB;
extern MapDB gMapDB;
extern LoadingScreensDB gLoadingScreensDB;
extern LightDB gLightDB;
extern LightParamsDB gLightParamsDB;
extern LightSkyboxDB gLightSkyboxDB;
extern LightIntBandDB gLightIntBandDB;
extern LightFloatBandDB gLightFloatBandDB;
extern GroundEffectDoodadDB gGroundEffectDoodadDB;
extern GroundEffectTextureDB gGroundEffectTextureDB;
extern LiquidTypeDB gLiquidTypeDB;
