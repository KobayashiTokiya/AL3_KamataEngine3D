#include "Player.h"
#include <cassert>

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position)
{
	assert(model);

	model_ = model;

	//textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	//プレイヤーの初期の座標と回転
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	camera_ = camera;
}

void Player::Update() 
{ 
	WorldTransformUpdate(worldTransform_);
}

void Player::Draw() 
{ 
	model_->Draw(worldTransform_, *camera_); 
}
