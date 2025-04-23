#include "GameScene.h"
#include "imgui.h"

using namespace KamataEngine;

GameScene::~GameScene()
{ 
	delete sprite_; 
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() 
{
	textureHandle_ = TextureManager::Load("mabodofu.jpg"); 
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	model_ = Model::Create();

	worldTransform_.Initialize();
	camera_.Initialize();

	//音声
	soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);

	//ライン描画の使用
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	//デバックカメラ
	debugCamera_ = new DebugCamera(1280, 720);
	
}

void GameScene::Update() 
{
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);
	//音声ループ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))//スペース押した瞬間
	{
		Audio::GetInstance()->StopWave(voiceHandle_);//音声を停止する
	}

	//デバッグテキスト表示
	ImGui::Begin("Debug1");
	ImGui::Text("Kobayashi Tokiya %d.%d.%d", 2050, 12, 31);
	ImGui::InputFloat3("ImputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();

	//デバックカメラ更新
	debugCamera_->Update();
}
//1280,720
void GameScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	sprite_->Draw();
	
	//スプライト描画後処理
	Sprite::PostDraw();


	//3Dの描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	model_->Draw(worldTransform_, camera_, textureHandle_);
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);
	//3Dの画像後処理
	Model::PostDraw();
	
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	//モデル連動
	
}