#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>

class GameScene {
public:
	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	uint32_t textureHandle_ = 0;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

	//Player* player_ = nullptr;

	std::vector < std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	KamataEngine::Model* modelBlock_ = nullptr;

	bool isDebugCameraActive_ = false;

	KamataEngine::DebugCamera* debugCamera_ = nullptr;
};