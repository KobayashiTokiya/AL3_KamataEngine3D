#pragma once
#include "KamataEngine.h"
#include "Math.h"
//std::numbers::pi_vを使うためにnumbersをインクルードをする
#include <numbers>
//std::clamp()を使うためのもの
#include <algorithm>

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	Vector3 velocity_ = {};

	enum class LRDirection
	{
		kRight,
		kLeft,
	};

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Camera* camera_ = nullptr;
	//02_05 フレームごとの加速度
	static inline const float kAcceleration = 0.01f;
	//02_05 非入力時の摩擦係数
	static inline const float kAttenuation = 0.05f;
	//02_05 最低/最大速度の制限のため
	static inline const float kLimitRunSpeed = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;
};