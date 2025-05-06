#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) 
{
	
	KamataEngine::Initialize(L"LE2C_09_コバヤシ_トキヤ_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	//デバッグテキストのインスタンス取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	while (true) 
	{
		if (KamataEngine::Update())
		{
			break;
		}
		//ImGui受付開始
		imguiManager->Begin();

		//ゲームシーンの更新
		gameScene->Update();
		
		imguiManager->End();

		//描画開始
		dxCommon->PreDraw();
		
		//ゲームシーンの描画
		gameScene->Draw();
		
		//軸表示の描画
		AxisIndicator::GetInstance()->Draw();

		//ImGui描画
		imguiManager->Draw();

		//描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの解放
	delete gameScene;//ループ外にdeleteする
	// nullptrの代入
	gameScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}
