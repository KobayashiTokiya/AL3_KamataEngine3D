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

private:
	Camera* camera_ = nullptr;

	Player* target_ = nullptr;
	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};
};
