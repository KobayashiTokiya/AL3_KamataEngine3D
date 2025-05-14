#pragma once
#include "KamataEngine.h"
#include "vector"
#include "Math.h"

class GameScene 
{
public:
	~GameScene();
	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();
private:
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera camera_;

	std::vector < std::vector<KamataEngine::WorldTransform* >> worldTransformBlocks_;

	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	

};
