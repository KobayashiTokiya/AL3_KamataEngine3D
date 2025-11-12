#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "clear.h"

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

	//普通のブロック
	void GenerateBlocks();

	//梯子ブロック
	void LadderBlocks();

	// 02_12
	void ChangePhase();

	bool IsFinished() const { return finished_; }
	bool IsGameClear() const { return isGameClear_;}
	bool IsPlayerDead() const { return isPlayerDead_; }

private:
	// 02_12 ゲームのフェーズ(型)
	enum class Phase {
		kFadeIn,  // フェーズイン
		kPlay,    // ゲームプレイ
		kDeath,   // デス演出
		kFadeOut, // フェードアウト
	};

	// ゲームの現在フェーズ(変数)
	Phase phase_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;
	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	// ブロックモデル
	KamataEngine::Model* modelBlock_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	//梯子関連
	//モデル
	Model* modelLadder_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformLadders_;



	// デバックカメラ有効
	bool isDebugCameraActive_ = false;
	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// スカイドーム
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	WorldTransform worldTransformSkydome_;

	// 02_04
	// マップチップフィールド
	MapChipField* mapChipField_;

	// 02_06
	CameraController* CController_ = nullptr;

	// 02_09 エネミークラス
	Enemy* enemy_ = nullptr;
	// エネミーモデル
	Model* enemyModel_ = nullptr;

	// 02_10
	std::list<Enemy*> enemies_;
	// 全ての当たり判定を行う
	void CheckAllCollisions();

	// 02_11
	DeathParticles* deathParticles_ = nullptr;

	Model* deathParticlesModel_ = nullptr;

	// 02_12
	bool finished_ = false;

	bool isGameClear_ = false;

	bool isPlayerDead_ = false;

	// 02_13
	Fade* fade_ = nullptr;

	// 02_14
	Model* modelAttack_ = nullptr;

	Model* kamaModel_ = nullptr;
	WorldTransform worldTransformKama_;

	
};