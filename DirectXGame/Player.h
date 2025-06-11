#pragma once
#include "KamataEngine.h"
#include "Math.h"
//std::numbers::pi_vを使うためにnumbersをインクルードをする
#include <numbers>

class Player
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position);

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Camera* camera_ = nullptr;
};