#include "GameScene.h"
#include "Math.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	// delete sprite_;
	delete model_;

	delete modelBlock_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;
	// 02_03
	// delete skydome_;
	delete modelSkydome_;

	// 02_04
	// mapChipField_をdeleteする
	delete mapChipField_;
}

void GameScene::Initialize() {
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// スプライトの生成
	// sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// 3Dモデルの生成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();

	// 02_01からの追加
	//  自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	playerModel_ = Model::CreateFromOBJ("player");
	player_->Initialize(playerModel_, textureHandle_, &camera_);
	// 3Dモデルの生成
	// Model::Create();からModel::CreateFromOBJ("block", true);の変更することによってブロックに変更出来る
	modelBlock_ = Model::CreateFromOBJ("block", true);

	// 02_03
	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_->Initialize(modelSkydome_, &camera_);

	camera_.farZ = 1000.0f;
	camera_.Initialize();

	// 要素数
	// const uint32_t kNumBlockVirtical = 10;
	// const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	// const float kBlockWidth = 1.0f;
	// const float kBlockHeight = 1.0f;

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	// orldTransformBlocks_.resize(kNumBlockVirtical);
	// for (uint32_t i = 0; i < kNumBlockVirtical; ++i)
	//{
	//	// 1列の要素数を設定（横方向のブロック数）
	//	worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//}
	//// ブロックの生成
	// for (uint32_t i = 0; i < kNumBlockVirtical; ++i)
	//{
	//	for (uint32_t j = 0; j < kNumBlockHorizontal; ++j)
	//	{
	//
	//		if ((i + j) % 2 == 1)
	//			continue;
	//
	//		worldTransformBlocks_[i][j] = new WorldTransform();
	//		worldTransformBlocks_[i][j]->Initialize();
	//		worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
	//		worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
	//	}
	// }

	// 02_04_マップチップフィールド
	mapChipField_ = new MapChipField;
	//LoadMapChipCsvの所にscvを入れることによってマップチップが出る
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < 20; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Update() {

	player_->Update();

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// フラグをトグル
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock)
				continue;

			Matrix4x4 affin_mat = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->matWorld_ = affin_mat;

			worldTransformBlock->TransferMatrix();
		}
	}

	// デバッグカメラの更新
	debugCamera_->Update();

	// 02_03
	skydome_->Update();
}

void GameScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	player_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}
	// 02_03
	skydome_->Draw();

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// スプライト描画後処理
	Sprite::PostDraw();
}
