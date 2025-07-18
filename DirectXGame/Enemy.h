#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;

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


};