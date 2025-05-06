#pragma once
#include "KamataEngine.h"

class GameScene 
{
public:
	//GameScene();
	~GameScene();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

private:
	//画像
	uint32_t textureHandle_ = 0;

	KamataEngine::Sprite* sprite_ = nullptr;

	KamataEngine::Model* model_ = nullptr;
	
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;
	//デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	//音声
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;
	
	//ImGui
	float inputFloat3[3] = {0, 0, 0};
};
