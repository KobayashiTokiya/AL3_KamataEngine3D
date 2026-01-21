#include "TitleScene.h"
#include "Math.h"
#include <numbers>

using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete modelPlayer_;
	delete modelTitle_;

	delete operationSprite_;
	delete startSprite_;

	// 02_13
	delete fade_;
}

void TitleScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("titleFont", true);
	modelPlayer_ = Model::CreateFromOBJ("player");

	// 背景
	modelBackground_ = Model::CreateFromOBJ("titleBackground", true);
	worldTransformBackground_.Initialize();
	worldTransformBackground_.scale_ = {50.0f, 50.0f, 1.0f};       // 背景の大きさ
	worldTransformBackground_.translation_ = {0.0f, 0.0f, -10.0f}; // カメラより奥

	// 操作説明
	textureHandle_ = TextureManager::Load("sprite/operation.png");
	operationSprite_ = Sprite::Create(textureHandle_, {850, 600});

	// スタートボタン
	startTextureHandle_ = TextureManager::Load("sprite/push_space.png");
	startSprite_ = Sprite::Create(startTextureHandle_, {440, 620});

	// カメラ初期化
	camera_.Initialize();

	const float kPlayerTitle = 5.0f;

	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kPlayerTitle, kPlayerTitle, kPlayerTitle};

	const float kPlayerScale = 10.0f;

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.x = -2.0f;
	worldTransformPlayer_.translation_.y = -10.0f;

	// 02_13
	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::Update() {
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
	worldTransformPlayer_.translation_.x += 0.3f * moveX;
	worldTransformPlayer_.rotation_.y = 2.0f * moveX;
	if (worldTransformPlayer_.translation_.x >= screenSize.x) 
	{
		moveX = -1;
	} 
	else if (worldTransformPlayer_.translation_.x <= -30.0f)
	{
		moveX = 1;
	}
	
	
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);

	float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	camera_.TransferMatrix();

	// アフィン変換～DirectXに転送(タイトル座標)
	WorldTransformUpdate(worldTransformTitle_);
	// アフィン変換～DirectXに転送（プレイヤー座標）
	WorldTransformUpdate(worldTransformPlayer_);
};

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelTitle_->Draw(worldTransformTitle_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	// 背景
	modelBackground_->Draw(worldTransformBackground_, camera_);

	Model::PostDraw();

	Sprite::PreDraw(commandList);

	operationSprite_->Draw();
	startSprite_->Draw();

	Sprite::PostDraw();

	// フェード用
	Model::PreDraw(commandList);
	// 02_13
	fade_->Draw();
	Model::PostDraw();
};