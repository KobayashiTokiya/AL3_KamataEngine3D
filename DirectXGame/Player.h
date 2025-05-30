#pragma once
#include "KamataEngine.h"
#include "Math.h"

class Player
{
public:
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle,KamataEngine::Camera* camera);

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Camera* camera_ = nullptr;
};