#pragma once
#include "Math.h"

using namespace KamataEngine;

class Player;

class Enemy 
{
public:
	//~Enemy();

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


};