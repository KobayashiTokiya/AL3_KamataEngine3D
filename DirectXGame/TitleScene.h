#pragma once
#include <KamataEngine.h>
#include "Fade.h"
using namespace KamataEngine;

class TitleScene
{
public:
	//シーンのフェーズ
	enum class Phase 
	{
		kFadeIn, //フェードイン
		kMain,   //メイン部
		kFadeOut,//フェードアウト
	};

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	//デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeTitleMove = 2.0f;

	Camera camera_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* modelPlayer_ = nullptr;
	Model* modelTitle_=nullptr;

	float counter_ = 0.0f;
	
	//終了フラグ
	bool finished_ = false;

	Fade* fade_ = nullptr;

	//現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// 背景モデル
	Model* modelBackground_ = nullptr;
	WorldTransform worldTransformBackground_;

	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* operationSprite_ = nullptr;

	uint32_t startTextureHandle_ = 0;
	KamataEngine::Sprite* startSprite_ = nullptr;
};
