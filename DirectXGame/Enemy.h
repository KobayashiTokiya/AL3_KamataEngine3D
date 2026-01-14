#pragma once
#include "Math.h"

class MapChipField;

using namespace KamataEngine;

class Player;

class Enemy 
{
public:
	//~Enemy();

	// 02_15 振るまい
	enum class Behavior 
	{
		kUnknown = -1, // 無効な状態
		kWalk,         // 歩行状態
		kDefeated,     // やられ状態
	};

	// 初期化
	void Initialize(Model *model,Camera* camera,const Vector3 &position);

	// 更新
	void Update();

	// 描画
	void Draw();

	//02_10
	AABB GetAABB();
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	//衝突応答
	void OnCollision(const Player* player);

	// 02_15 
	bool IsDead() const { return isDead_; }
	
	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

	bool IsHitWall();

	bool IsOnGround();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:
	WorldTransform worldTransform_;

	Model* model_=nullptr;
	
	Camera* camera_;

	//歩行の速さ
	static inline const float kWalkSpeed =0.02f;

	//速度
	Vector3 velocity_ = {};

	//最初の速度[度]
	static inline const float kWalkMotionAngleStart = 0.0f;
	//最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 30.0f;
	//アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime =1.0f;

	//経過時間
	float walkTimer = 0.0f;

	// 02_10 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 02_15 
	bool isDead_ = false;


	Behavior behavior_ = Behavior::kWalk;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f; // カウンター

	bool isCollisionDisabled_ = false;

	//敵が全員死ぬときのフラグ
	bool isAllEnemyDead = false;

	 MapChipField* mapChipField_ = nullptr;

	 //重力 
	 float kGravity = 0.03f;
	 //最大落下速度
	 float kMaxFallSpeed = 0.5f;
};