#include <algorithm>
#include "CameraController.h"
#include "Math.h"
#include "Player.h"

void CameraController::Initiallize(Camera *camera) 
{
	camera_=camera; 
}

void CameraController::Update() 
{
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// プレイヤーを常に画面中央に
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;

	// 移動範囲制限だけ残す
	camera_->translation_.x = std::clamp(camera_->translation_.x, movableArea_.left, movableArea_.right);

	camera_->translation_.y = std::clamp(camera_->translation_.y, movableArea_.bottom, movableArea_.top);

	camera_->UpdateMatrix();
}

void CameraController::Reset()
{
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}