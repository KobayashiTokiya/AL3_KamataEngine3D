#include <cassert>
#include <fstream>
#include <map>
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
