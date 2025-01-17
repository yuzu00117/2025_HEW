//-----------------------------------------------------------------------------------------------------
// #name title.cpp
// #description タイトルつくったよー
// #make 2024/12/17　　永野義也
// #update 2024/12/17
// #comment 追加・修正予定
//          ・タイトル作ったさねー
//           
//----------------------------------------------------------------------------------------------------

#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"


//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_title_Texture = NULL;

void TitleScene::Initialize()
{
	g_title_Texture= InitTexture(L"asset\\texture\\sample_texture\\sample_title.png");
	app_atomex_start(TITLE_BGM);
}

void TitleScene::Update()
{
	//コントローラーの入力の受け取り
	ControllerState state = GetControllerInput();

	if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_STAGE_SELECT);
	};
}

void TitleScene::Draw()
{

	//バッファクリア
	Clear();

	//2D描画なので深度無効
	SetDepthEnable(false);

	if (g_title_Texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);
	}

	//バックバッファ、フロントバッファ入れ替え
	Present();

}

void TitleScene::Finalize()
{
	UnInitTexture(g_title_Texture);
}