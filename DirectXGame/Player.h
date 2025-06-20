#pragma once
#include "KamataEngine.h"
#include "Math.h"
// std::numbers::pi_vを使うためにnumbersをインクルードをする
#include <numbers>
// std::clamp()を使うためのもの
#include <algorithm>

using namespace KamataEngine;

class Player {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	enum class LRDirection
	{
		kRight,
		kLeft,
	};

	const WorldTransform& GetWorldTransform() const { return worldTransform_;}

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Camera* camera_ = nullptr;

	Vector3 velocity_ = {};
	// 02_05 フレームごとの加速度
	static inline const float kAcceleration = 0.01f;
	// 02_05 非入力時の摩擦係数
	static inline const float kAttenuation = 0.05f;
	// 02_05 最低/最大速度の制限のため
	static inline const float kLimitRunSpeed = 0.3f;
	//02_05 顔の向き
	LRDirection lrDirection_ = LRDirection::kRight;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 接地状態フラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 20.0f;
};