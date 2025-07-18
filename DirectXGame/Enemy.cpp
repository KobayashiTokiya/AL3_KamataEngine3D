#include "Enemy.h"
#include "Math.h"
#include <cassert>
#include <numbers>


void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) 
{
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	camera_ = camera;

	velocity_ = {-kWalkSpeed, 0, 0};
	
	walkTimer = 0.0f;
}

void Enemy::Update()
{	
	worldTransform_.translation_ += velocity_;

	//タイマーを加算
	walkTimer += 1.0f / 60.0f;
	
	//回転アニメーション
	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);

	WorldTransformUpdate(worldTransform_); 
}

void Enemy::Draw() 
{
	model_->Draw(worldTransform_, *camera_); 
}