#include "GameScene.h"
#include "Math.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("sample.png");

	model_ = Model::Create();
	modelBlock_ = Model::Create();
	const uint32_t kNumBlockHorizontal = 20;

	const float kBlockWidth = 2.0f;

	worldTransformBlocks_.resize(kNumBlockHorizontal);
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {

		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}

	camera_ = new Camera();
	camera_->Initialize();

	player_ = new Player();

	player_->Initialize(model_, textureHandle_, camera_);
}

void GameScene::Update() {
	player_->Update();

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {

		Matrix4x4 affin_mat = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
		worldTransformBlock->matWorld_ = affin_mat;

		// 定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	//player_->Draw();
	
	for (WorldTransform* worldTranceformBlock : worldTransformBlocks_)
	{
		modelBlock_->Draw(*worldTranceformBlock, *camera_);
	}

	Model::PostDraw();
}

GameScene::~GameScene() 
{
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) 
	{
		delete worldTransformBlock;
	}
	delete model_;
	worldTransformBlocks_.clear();
	delete player_;
	delete camera_;
}