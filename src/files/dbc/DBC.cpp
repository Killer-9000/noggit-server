#include "log.h"
#include "DBC.h"
#include <string>
#include "worker_thread_mgr.h"

AreaDB gAreaDB;
MapDB gMapDB;
LoadingScreensDB gLoadingScreensDB;
LightDB gLightDB;
LightParamsDB gLightParamsDB;
LightSkyboxDB gLightSkyboxDB;
LightIntBandDB gLightIntBandDB;
LightFloatBandDB gLightFloatBandDB;
GroundEffectDoodadDB gGroundEffectDoodadDB;
GroundEffectTextureDB gGroundEffectTextureDB;
LiquidTypeDB gLiquidTypeDB;

void OpenDBs()
{
	auto start = std::chrono::high_resolution_clock::now();
	LOG_INFO("Loading DBC Files...");
	gAreaDB.Open();
	gMapDB.Open();
	gLoadingScreensDB.Open();
	gLightDB.Open();
	gLightParamsDB.Open();
	gLightSkyboxDB.Open();
	gLightIntBandDB.Open();
	gLightFloatBandDB.Open();
	gGroundEffectDoodadDB.Open();
	gGroundEffectTextureDB.Open();
	gLiquidTypeDB.Open();
	
	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	LOG_INFO("Loaded DBC Files in %i ms", duration.count());
}

std::string AreaDB::GetAreaName(int areaID)
{
	std::string areaName = "Unknown location";
	if (!areaID || areaID == -1)
		return areaName;

	unsigned int parentID = 0;

	AreaDB::Record* rec = gAreaDB.GetByID(areaID);
	if (rec)
	{
		areaName = rec->GetLocalizedString(AreaDB::Name);
		parentID = rec->GetUInt(AreaDB::Parent);
	}

	if (parentID == 0)
		return areaName;

	rec = gAreaDB.GetByID(parentID);
	if (rec)
		areaName = rec->GetLocalizedString(AreaDB::Name) + ": " + areaName;

	return areaName;
}

std::uint32_t AreaDB::GetAreaParent(int areaID)
{
	if (!areaID || areaID == -1)
		return 0;

	AreaDB::Record* rec = gAreaDB.GetByID(areaID);
	if (rec)
		return rec->GetUInt(AreaDB::Parent);

	return 0;
}

std::string MapDB::GetMapName(int mapID)
{
	std::string mapName = "Unknown map";
	if (mapID < 0) 
		return mapName;

	MapDB::Record* rec = gMapDB.GetByID(mapID);
	if(rec)
		mapName = rec->GetLocalizedString(MapDB::Name);

	return mapName;
}

std::string GetGroundEffectDoodad(unsigned int effectID, int doodadNum)
{
	GroundEffectTextureDB::Record* texture = gGroundEffectTextureDB.GetByID(effectID);
	if (!texture)
	{
		LOG_ERROR("Tried to get a not existing row in GroundEffectTextureDB or GroundEffectDoodadDB ( effectID = %i, doodadNum = %i )!", effectID, doodadNum);
		return "";
	}

	uint32_t doodadID = texture->GetUInt(GroundEffectTextureDB::Doodads + doodadNum);
	GroundEffectDoodadDB::Record* doodad = gGroundEffectDoodadDB.GetByID(doodadID);
	if (!doodad)
	{
		LOG_ERROR("Tried to get a not existing row in GroundEffectTextureDB or GroundEffectDoodadDB ( effectID = %i, doodadNum = %i )!", effectID, doodadNum);
		return "";
	}

	return doodad->GetString(GroundEffectDoodadDB::Filename);
}

int LiquidTypeDB::GetLiquidType(int liquidID)
{
	int type = 0;

	LiquidTypeDB::Record* rec = gLiquidTypeDB.GetByID(liquidID);
	if(rec)
		type = rec->GetUInt(LiquidTypeDB::Type);
	
	return type;
}

std::string LiquidTypeDB::GetLiquidName(int liquidID)
{
	std::string type = "Unknown type";
	
	LiquidTypeDB::Record* rec = gLiquidTypeDB.GetByID(liquidID);
	if(rec)
		type = rec->GetString(LiquidTypeDB::Name);

	return type;
}
