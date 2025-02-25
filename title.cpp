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
ID3D11ShaderResourceView* g_title_background_Texture = NULL;
ID3D11ShaderResourceView* g_title_Texture = NULL;


float sheet_cnt = 0;

void TitleScene::Initialize()
{
	g_title_background_Texture = InitTexture(L"asset\\texture\\title_texture\\title_background.jpg");
	g_title_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo.png");

	

	//全ての音を止める
	app_atomex_stop_player();
	//タイトルのBGMをかける
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

	if (Keyboard_IsKeyDown(KK_R) || (state.buttonA))
	{
		sheet_cnt = 0;
	}

	int max_sheet = 62;

	if (sheet_cnt <= max_sheet)
	{
		sheet_cnt += 0.5f;
	}
}

void TitleScene::Draw()
{

	//バッファクリア
	Clear();

	//2D描画なので深度無効
	SetDepthEnable(false);

	if (g_title_background_Texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_background_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);
	}

	if (g_title_Texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_Texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
			8,8, sheet_cnt
		);
	}

	//バックバッファ、フロントバッファ入れ替え
	Present();

}

void TitleScene::Finalize()
{
	if (g_title_background_Texture) UnInitTexture(g_title_background_Texture);
	if (g_title_Texture) UnInitTexture(g_title_Texture);

}