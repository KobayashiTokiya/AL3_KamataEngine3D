#include "GameOver.h"
#include "Math.h"
#include <numbers>

using namespace KamataEngine;

GameOver::~GameOver() {
	delete modelPlayer_;
	delete modelGameOver_;
	delete modelEnemy1_;
	delete modelEnemy2_;
	// 02_13
	delete fade_;
}

void GameOver::Initialize() {
	modelGameOver_ = Model::CreateFromOBJ("gameOver", true);
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelEnemy1_ = Model::CreateFromOBJ("enemy");
	modelEnemy2_ = Model::CreateFromOBJ("enemy");
	// カメラ初期化
	camera_.Initialize();

	const float kGameOver = 2.0f;

	worldTransformGameOver_.Initialize();
	worldTransformGameOver_.scale_ = {kGameOver, kGameOver, kGameOver};

	const float kPlayerScale = 7.0f;

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.z = 80.0f;
	worldTransformPlayer_.rotation_.y = 1.0f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.x = 0.0f;
	worldTransformPlayer_.translation_.y = -10.0f;

	const float kEnemyScale1 = 10.0f;

	worldTransformEnemy1_.Initialize();
	worldTransformEnemy1_.scale_ = {kEnemyScale1, kEnemyScale1, kEnemyScale1};
	worldTransformEnemy1_.rotation_.y = -0.8f * std::numbers::pi_v<float>;
	worldTransformEnemy1_.translation_.x = 15.0f;
	worldTransformEnemy1_.translation_.y = -10.0f;
	worldTransformEnemy1_.translation_.z = 20.0f;

	const float kEnemyScale2 = 10.0f;

	worldTransformEnemy2_.Initialize();
	worldTransformEnemy2_.scale_ = {kEnemyScale2, kEnemyScale2, kEnemyScale2};
	worldTransformEnemy2_.rotation_.y = 0.8f * std::numbers::pi_v<float>;
	worldTransformEnemy2_.translation_.x = -15.0f;
	worldTransformEnemy2_.translation_.y = -10.0f;
	worldTransformEnemy2_.translation_.z = 20.0f;
	
	// 02_13
	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameOver::Update() {
	// 02_13
	// fade_->Update();

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	// if (Input::GetInstance()->PushKey(DIK_SPACE))
	//{
	//	finished_ = true;
	// }

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeGameOverMove);

	float angle = counter_ / kTimeGameOverMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformGameOver_.translation_.y = std::sin(angle) + 10.0f;

	camera_.TransferMatrix();

	// アフィン変換～DirectXに転送(タイトル座標)
	WorldTransformUpdate(worldTransformGameOver_);
	// アフィン変換～DirectXに転送（プレイヤー座標）
	WorldTransformUpdate(worldTransformPlayer_);
	// アフィン変換～DirectXに転送（エネミー1座標）
	WorldTransformUpdate(worldTransformEnemy1_);
	// アフィン変換～DirectXに転送（エネミー2座標）
	WorldTransformUpdate(worldTransformEnemy2_);
};

void GameOver::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelGameOver_->Draw(worldTransformGameOver_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);
	modelEnemy1_->Draw(worldTransformEnemy1_, camera_);
	modelEnemy2_->Draw(worldTransformEnemy2_, camera_);
	// 02_13
	fade_->Draw();
	Model::PostDraw();
};