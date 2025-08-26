#include "GameScene.h"
#include "Enemy.h"
#include "Math.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	delete sprite_;
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

	// 02_09 enemyを削除する
	delete enemy_;

	// 02_10
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	delete deathParticles_;
	delete deathParticlesModel_;
}

void GameScene::Initialize() {
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// 3Dモデルの生成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();

	// 3Dモデルの生成
	// Model::Create();からModel::CreateFromOBJ("block", true);の変更することによってブロックに変更出来る
	modelBlock_ = Model::CreateFromOBJ("block", true);

	// 02_03
	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_->Initialize(modelSkydome_, &camera_);

	camera_.farZ = 1000.0f;
	camera_.Initialize();

	// 02_04_マップチップフィールド
	mapChipField_ = new MapChipField;
	// LoadMapChipCsvの所にscvを入れることによってマップチップが出る
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// プレイヤーがマップチップの参照する関係上、
	//  プレイヤーがマップチップに依存するので
	//  02_07によってマップチップフィールドのあとにプレイヤーにする
	//  02_01からの追加
	//   自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	playerModel_ = Model::CreateFromOBJ("player");

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	// 02_07
	player_->SetMapChipField(mapChipField_);

	player_->Initialize(playerModel_, &camera_, playerPosition);

	// 02_06
	// 生成
	CController_ = new CameraController();
	// 初期化
	CController_->Initiallize(&camera_);
	// 追従対象セット
	CController_->SetTarget(player_);
	// リセット
	CController_->Reset();

	// カメラコントローラ
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	CController_->SetMovableArea(cameraArea);

	// 02_09 敵クラス
	enemy_ = new Enemy();
	// 敵モデル
	enemyModel_ = Model::CreateFromOBJ("enemy");
	// 敵位置決めて敵クラス初期化
	// Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12, 18);
	// enemy_->Initialize(enemyModel_, &camera_, enemyPosition);

	// 02_10
	for (int32_t i = 0; i < 2; i++) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14 + i * 2, 18);
		newEnemy->Initialize(enemyModel_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// 02_11
	deathParticlesModel_ = Model::CreateFromOBJ("deathParticle");

	// deathParticles_ = new DeathParticles;
	// deathParticles_->Initialize(deathParticlesModel_, &camera_, playerPosition);

	// 02_12
	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}

// 02_12
void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		// ゲームプレイフェーズの処理
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();


			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticlesModel_, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:

		break;
	}
}

// ブロック
void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
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
	// デバッグカメラの更新
	debugCamera_->Update();

	ChangePhase();

	// 02_12
	switch (phase_) {
	case Phase::kPlay:
		// ゲームプレイフェーズの処理
		break;
	case Phase::kDeath:
		if (deathParticles_&&deathParticles_->IsFinished())
		{
			finished_ = true;
		}
		break;
	}

	player_->Update();
	// 02_03
	skydome_->Update();

	// 02_06
	CController_->Update();

	// 02_09 敵更新
	// enemy_->Update();

	// 02_10
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

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
		//camera_.TransferMatrix();
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

	// 全ての当たり判定
	CheckAllCollisions();

	// 02_11
	if (deathParticles_) {
		deathParticles_->Update();
	}
}

void GameScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	if (!player_->IsDead())
	{
		player_->Draw();
	}

	// 02_03
	skydome_->Draw();
	
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}


	// 02_09 敵描画
	// enemy_->Draw();

	// 02_10
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 02_11
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// スプライト描画後処理
	Sprite::PostDraw();
}

void GameScene::CheckAllCollisions() {
	// 判定対象1と2の座標
	AABB aabb1, aabb2;
#pragma region 自キャラと敵キャラの当たり判定

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);

			// 敵弾の衝突時コールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}

#pragma endregion
}
