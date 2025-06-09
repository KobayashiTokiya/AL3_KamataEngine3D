#include "Player.h"
#include <cassert>

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) 
{
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_.x = 2.0f;
	worldTransform_.translation_.y = 1.0f;
	
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
