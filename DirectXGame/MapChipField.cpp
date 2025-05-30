#include "MapChipField.h"
#include <map>
#include <string>

namespace 
{
    std::map<std::string, MapChipType> mapChipTable =
    {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
    };
}