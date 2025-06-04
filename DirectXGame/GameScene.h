#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include "MapChipField.h"
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
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデル
	KamataEngine::Model* model_ = nullptr;
	//ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	//カメラ
	KamataEngine::Camera camera_;
	//プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	//ブロックモデル
	std::vector < std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	KamataEngine::Model* modelBlock_ = nullptr;
	//デバックカメラ有効
	bool isDebugCameraActive_ = false;
	//デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	//スカイドーム
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	WorldTransform worldTransformSkydome_;

	//02_04
	//マップチップフィールド
	MapChipField* mapChipField_;
};