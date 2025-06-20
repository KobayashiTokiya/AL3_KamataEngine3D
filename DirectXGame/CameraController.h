#pragma once
#include "KamataEngine.h"

//前方宣言
class Player;

using namespace KamataEngine;

class CameraController 
{
public:
	void Initiallize(Camera* camera);

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	void SetMovableArea();

	struct Rect 
	{
		//左端
		float left = 0.0f;
		//右端
		float right = 1.0f;
		//下端
		float bottom = 0.0f;
		//上端
		float top = 1.0f;
	};

private:
	Camera* camera_ = nullptr;

	Player* target_ = nullptr;
	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};
	//カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};
};
