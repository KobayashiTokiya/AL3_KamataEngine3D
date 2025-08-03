#include "DeathParticles.h"

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() 
{
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	// カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	// 存続時間の上限に達したら
	if (counter_ >= kDuration_)
	{
		counter_ = kDuration_;
		// 終了扱いにする
		isFinished_ = true;
	}

	for (uint32_t i = 0; i < kNumParticles; ++i)
	{
		// 基本となる速度ベクトル
		Vector3 velocity = {kSpeed_, 0, 0};
		// 回転角の計算する
		float angle = kAngleUnit_ * i;
		// Z軸まわり回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させる速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		// 移動処理
		worldTransforms_[i].translation_ += velocity;
	}
	
	color_.w = std::clamp(1.0f-counter_/kDuration_,0.0f,1.0f);
	//色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	
	for (auto& worldTransform : worldTransforms_)
	{
		WorldTransformUpdate(worldTransform);
	}
}

void DeathParticles::Draw()
{
	// 終了なら何もしない
	if (isFinished_)
	{
		return;
	}
	
	for (auto& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, *camera_,&objectColor_);
	}
}