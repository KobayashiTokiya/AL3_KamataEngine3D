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
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	
	// 02_06 p29
	const Vector3& targetVelocty = target_->GetVelocity();
	
	//追従対象のオフセットと追従対象の速度からカメラの座標を計算
	//destination_は、目標座標
	destination_ = targetWorldTransform.translation_ + targetOffset_+targetVelocty*kVelocityBias;
	
	//座標補間によりゆったり追従
	camera_->translation_ = Lerp(camera_->translation_, destination_, kInterpolationRate);

	//追従対象が画面外に出ないように補正
	camera_->translation_.x = max(camera_->translation_.x, destination_.x + targetMargin.left);
	camera_->translation_.x = min(camera_->translation_.x, destination_.x + targetMargin.right);
	camera_->translation_.y = max(camera_->translation_.y, destination_.y + targetMargin.bottom); 
	camera_->translation_.y = min(camera_->translation_.y, destination_.y + targetMargin.top);


	//移動範囲制限
	camera_->translation_.x = max(camera_->translation_.x, movableArea_.left);
	camera_->translation_.x = min(camera_->translation_.x, movableArea_.right);
	camera_->translation_.y = max(camera_->translation_.y, movableArea_.bottom);
	camera_->translation_.y = min(camera_->translation_.y, movableArea_.top);
	
	//行列を更新する
	camera_->UpdateMatrix();
}

void CameraController::Reset()
{
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}