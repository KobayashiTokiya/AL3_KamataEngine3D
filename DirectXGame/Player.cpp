// std::maxを使うためのもの
#define NOMINMAX

#include "Player.h"
#include "MapChipField.h"
#include <cassert>

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	// プレイヤーの初期の座標と回転
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	camera_ = camera;
}

void Player::Update() {
	worldTransform_.translation_ += velocity_;

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) {
		// Y座標が地面以上になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に以降
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態の移行
			onGround_ = true;
		}
	}

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	WorldTransformUpdate(worldTransform_);

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量の速度の値をコピー
	collisionMapInfo.move = velocity_;

	// マップ衝突チェック
	CheckMapCollision(collisionMapInfo);
}

//
void Player::InputMove() {
	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					// 旋回の最初は移動減衰をかける
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration / 60.0f;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					// 旋回の最初は移動減衰をかける
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration / 60.0f;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		// ほぼ0の場合に0にする
		if (std::abs(velocity_.x) <= 0.0001f) {
			velocity_.x = 0.0f;
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration / 60.0f, 0);
		}
	} else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration / 60.0f, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CheckMapCollision(CollisionMapInfo& info) {
	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

//上方向判定
void Player::CheckMapCollisionUp(CollisionMapInfo& info) {

	if (info.move.y <= 0) 
	{
		return;
	}

	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i)
	{
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipField mapChipType;
	//
	bool hit = false;
	//
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType==MapChipType::kBlock)
	{
		hit = true;
	}
	//
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::Draw() 
{
	model_->Draw(worldTransform_, *camera_);
}
