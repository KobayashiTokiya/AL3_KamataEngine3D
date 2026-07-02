#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank   },
    {"1", MapChipType::kBlock   },
    {"2", MapChipType::kLadder  },
    {"3", MapChipType::kIceBlock},
    {"4", MapChipType::kCollapse},
    {"5", MapChipType::kGoal},
};
}

void MapChipField::Update() 
{
	UpdateCollapseChips(); 
}

void MapChipField::ResetMapChipData() {
	// マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}

	// マップチップデータをリセット
	collapseChipData_.clear();
	collapseChipData_.resize(kNumBlockVirtical);

	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		collapseChipData_[y].resize(kNumBlockHorizontal);

		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			auto& chip = collapseChipData_[y][x];
			chip.state = CollapseState::Appear; //最初は表示
			chip.timer = 0.0f;
			chip.shakeTime = 0.0f;
		}
	}
};

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//  マップチップCSV
	std::stringstream mapChipCsv;

	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();

	// ファイルを閉じる
	file.close();

	// マップチップデータをリセット
	ResetMapChipData();

	std::string line;

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	 MapChipType type = mapChipData_.data[yIndex][xIndex];

	//崩れる床は state を見る
	if (type == MapChipType::kCollapse) {
		const auto& chip = collapseChipData_[yIndex][xIndex];

		if (chip.state == CollapseState::Disappear) {
			return MapChipType::kBlank;//完全に空気
		}
	}

	return type;
}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	// 座標からマップチップ番号を計算
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2.0f) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>(position.y + kBlockHeight / 2.0f / kBlockHeight);
	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	// 指定ブロックの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockWidth / 2.0f;
	rect.top = center.y + kBlockWidth / 2.0f;

	return rect;
}

CollapseChipData& MapChipField::GetCollapseChip(uint32_t xIndex, uint32_t yIndex) {
	assert(xIndex < kNumBlockHorizontal);
	assert(yIndex < kNumBlockVirtical);
	return collapseChipData_[yIndex][xIndex];
};

void MapChipField::StartCollapse(uint32_t x, uint32_t y) {
	auto& chip = collapseChipData_[y][x];

	if (chip.state == CollapseState::Appear) {
		chip.state = CollapseState::WaitCollapse;
		chip.timer = 60.0f; // 1秒（60FPS想定）
	}
}

void MapChipField::UpdateCollapseChips() {
	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			if (mapChipData_.data[y][x] != MapChipType::kCollapse) {
				continue;
			}

			auto& chip = collapseChipData_[y][x];

			switch (chip.state) {
			case CollapseState::Appear:
				break;

			case CollapseState::WaitCollapse:
				chip.timer--;
				chip.shakeTime += 1.0f;

				if (chip.timer<=0.0f)
				{
					chip.state = CollapseState::Disappear;
					chip.timer = 180.0f;
				}
				break;
			case CollapseState::Disappear:
				chip.timer--;
				if (chip.timer<=0.0f)
				{
					chip.state = CollapseState::Appear;
				}
				break;
			}
		}
	}
}