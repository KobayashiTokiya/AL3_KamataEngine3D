#include "GameScene.h"
#include "imgui.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("mabodofu.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();

	// 音声
	soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
	
	//ライン描画が参照するカメラを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
	
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

void GameScene::Update() {
	// スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	// 移動した座標をスプライトに反映
	sprite_->SetPosition(position);

	// 音声ループ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) // スペース押した瞬間
	{
		Audio::GetInstance()->StopWave(voiceHandle_); // 音声を停止する
	}

#ifdef _DEBUG
	// デバッグテキスト表示
	ImGui::Begin("Debug1");
	ImGui::Text("Kobayashi Tokiya %d.%d.%d", 2050, 12, 31);
	ImGui::Begin("Debug2");
	ImGui::InputFloat3("ImputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::ShowDemoWindow();
	ImGui::End();
#endif
	
	//デバックカメラ更新
	 debugCamera_->Update();
}
// 1280,720
void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

	// 3Dの描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	//model_->Draw(worldTransform_, camera_, textureHandle_);
	// モデル連動
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);
	//  3Dの画像後処理
	Model::PostDraw();
	//ラインを描画
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
}