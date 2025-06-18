#include "Player.h"
#include <cassert>

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	// プレイヤーの初期の座標と回転
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	camera_ = camera;
}

void Player::Update() {
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) 
	{
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT))
		{
			// 左移動中の右入力
			if (velocity_.x < 0.0f) 
			{
				// 速度の逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		}
		else if (Input::GetInstance()->PushKey(DIK_LEFT))
		{
			// 右移動中の左入力
			if (velocity_.x > 0.0f)
			{
				// 速度の逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		else 
		{
			velocity_.x *= (1.0f - kAttenuation);
		}

		// 加速/減速
		velocity_ += acceleration;
		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	}

	worldTransform_.translation_ += velocity_;

	WorldTransformUpdate(worldTransform_);
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }
