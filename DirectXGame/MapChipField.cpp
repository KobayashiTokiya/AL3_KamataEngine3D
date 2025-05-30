#include <cassert>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include "MapChipField.h"
namespace 
{
    std::map<std::string, MapChipType> mapChipTable =
    {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
    };
}

void MapChipField::ResetMapChipData() 
{
    mapChipData_.data.clear();
	mapChipData_.data.reserve(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine:mapChipData_.data )
    {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
};

void MapChipField::LoadMapChipCsv(const std::string& filePath) 
{
    ResetMapChipData(); 

    std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

    std::stringstream mapChipCsv;

    mapChipCsv << file.rdbuf();

    file.close();
}