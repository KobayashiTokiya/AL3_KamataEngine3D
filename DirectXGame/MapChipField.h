#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

enum class MapChipType 
{
	kBlank,   //空白
	kBlock,   //ブロック
	kLadder,  //梯子
	kIceBlock,//滑る床
	kCollapse, //崩れる床
	kGoal
};

enum class CollapseState 
{
	Appear,
	WaitCollapse, // 乗られてから消えるまで3秒
	Disappear     // 消えている10秒
};

struct CollapseChipData 
{
	CollapseState state = CollapseState::Appear;
	float timer = 3.0f;
	bool isTriggered = false;

	float shakeTime = 0.0f;
};


struct MapChipData
{
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField 
{
public:
	
	//02_07
	struct  IndexSet 
	{
		uint32_t xIndex;
		uint32_t yIndex;
	};

	// 範囲矩形
	struct Rect 
	{
		//左端
		float left;
		//右端
		float right;
		//下端
		float bottom;
		//上端
		float top;
	
	};
	void Update();

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	// 02_07
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	//崩れるブロック用
	CollapseChipData& GetCollapseChip(uint32_t xIndex, uint32_t yIndex);
	void UpdateCollapseChips();
	void StartCollapse(uint32_t x, uint32_t y);

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	
	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
	
	//崩れるブロック用
	std::vector<std::vector<CollapseChipData>> collapseChipData_;
};
