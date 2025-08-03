#pragma once
#include "KamataEngine.h"
#include "Math.h"
// std::numbers::pi_vを使うためにnumbersをインクルードをする
#include <numbers>
// std::clamp()を使うためのもの
#include <algorithm>

using namespace KamataEngine;

// MapChipFieldを前方宣言
class MapChipField;

class Enemy;

class Player {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	enum class LRDirection {
		kRight,
		kLeft,
	};

	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocty() const { return velocity_; }

	// 02_07
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void InputMove();

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 02_10
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//AABBを取得
	AABB GetAABB();

	//衝突応答
	void OnCollision(const Enemy* enemy);

	//02_12 デスフラグ
	bool IsDead() const { return isDead_; }
private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Camera* camera_ = nullptr;

	Vector3 velocity_ = {};
	// 02_05 フレームごとの加速度
	static inline const float kAcceleration = 0.01f;
	// 02_05 非入力時の摩擦係数
	static inline const float kAttenuation = 0.05f;
	// 02_05 最低/最大速度の制限のため
	static inline const float kLimitRunSpeed = 0.3f;
	// 02_05 顔の向き
	LRDirection lrDirection_ = LRDirection::kRight;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 接地状態フラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 20.0f;

	// 02_07
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// キャラクターの当たり判定サイズ
	// ブロックより小さい設定にすることで隙間を無理なく通れる
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// 　マップとの当たり判定情報
	struct CollisionMapInfo {
		// 天井衝突フラグ
		bool ceiling = false;
		// 着地フラグ
		bool landing = false;
		// 壁接触フラグ
		bool hitWall = false;
		// 移動量
		Vector3 move;
	};

	void CheckMapCollision(CollisionMapInfo& info);
	// 上下左右のマップ衝突判定
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	static inline const float kBlank = 0.04f;

	// 設置状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);

	// 壁接触している場合の処理
	void UpdateOnWall(const CollisionMapInfo& info);

	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.2f;
	// 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;
	// 着地時
	static inline const float kAttenuationWall = 0.2f;

	//02_12
	//デスフラグ
	bool isDead_ = false;
};