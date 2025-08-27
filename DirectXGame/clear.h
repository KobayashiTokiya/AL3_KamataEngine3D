#pragma once
#pragma once
#include "Fade.h"
#include <KamataEngine.h>
using namespace KamataEngine;

class Clear {
public:
	// シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	~Clear();

	void Initialize();

	void Update();

	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeClearMove = 2.0f;

	Camera camera_;
	WorldTransform worldTransformClear_;
	WorldTransform worldTransformPlayer_;
	WorldTransform worldTransformEnemy1_;
	WorldTransform worldTransformEnemy2_;

	Model* modelPlayer_ = nullptr;
	Model* modelClear_ = nullptr;
	Model* modelEnemy1_ = nullptr;
	Model* modelEnemy2_ = nullptr;

	float counter_ = 0.0f;

	// 終了フラグ
	bool finished_ = false;

	Fade* fade_ = nullptr;

	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;
};
