#include "Enemy.h"
#include "Math.h"
#include "Player.h"
#include <cassert>
#include <numbers>
#include "MapChipField.h"

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	camera_ = camera;

	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer = 0.0f;
}

void Enemy::Update() {
	// 変更リクエストがあったら
	if (behaviorRequest_ != Behavior::kUnknown) {
		// 振るまいを変更
		behavior_ = behaviorRequest_;

		// 各振るまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kDefeated:
		default:
			counter_ = 0;
			break;
		}

		// 振るまいリクエストをリセット
		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	// 歩行
	case Behavior::kWalk:
		// 壁に当たったら反転
		if (IsHitWall()) {
			velocity_.x *= -1.0f;

			// 向きも反転（見た目用）
			worldTransform_.rotation_.y += std::numbers::pi_v<float>;
		}

		// 移動
		worldTransform_.translation_ += velocity_;

		// タイマー
		walkTimer += 1.0f / 60.0f;

		// 回転アニメーション
		worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);

		WorldTransformUpdate(worldTransform_);
		break;
	// やられ
	case Behavior::kDefeated:
		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.rotation_.x = EaseOut(ToRadians(kDefeatedMotionAngleStart), ToRadians(kDefeatedMotionAngleEnd), counter_ / kDefeatedTime);

		WorldTransformUpdate(worldTransform_);

		if (counter_ >= kDefeatedTime) {
			isDead_ = true;
		}
		break;
	}
}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision(const Player* player) 
{
	//(void)player;
	// 02_15 6枚目 → 14枚目で削除
	//	isDead_ = true;

	if (behavior_ == Behavior::kDefeated) {
		// 敵がやられているなら何もしない
		return;
	}

	// プレイヤーが攻撃中なら敵が死ぬ
	// player.hをインクルード
	if (player->IsAttack()) {
		// 敵の振るまいをやられに変更
		behaviorRequest_ = Behavior::kDefeated;
		isAllEnemyDead = true;
		// 02_15 20枚目 衝突を無効化
		isCollisionDisabled_ = true;
	}
}

bool Enemy::IsHitWall()
{
	// 進行方向
	float dir = (velocity_.x > 0.0f) ? 1.0f : -1.0f;

	// 体の横先端
	Vector3 checkPos = worldTransform_.translation_;
	checkPos.x += dir * (kWidth * 0.5f + 0.01f);

	auto index = mapChipField_->GetMapChipIndexSetByPosition(checkPos);

	return mapChipField_->GetMapChipTypeByIndex(index.xIndex, index.yIndex) == MapChipType::kBlock;
}