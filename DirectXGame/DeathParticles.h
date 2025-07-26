#pragma once
#include <KamataEngine.h>
#include <array>
#include <numbers>
#include <algorithm>
#include "Math.h"

using namespace KamataEngine;

class DeathParticles 
{
public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	Model* model_ = nullptr;

	Camera* camera_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	//存続時間(消滅までの時間)<秒>
	static inline const float kDuration_ =2.0f ;
	//移動の速さ
	static inline const float kSpeed_ = 0.05f;
	//分割した一個分の角度(2π/分割数)
	static inline const float kAngleUnit_ =2.0f*std::numbers::pi_v<float>/kNumParticles;
	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;
	//色変更オブジェクト
	ObjectColor objectColor_;
	//色の数値
	Vector4 color_;
};
