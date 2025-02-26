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
static ID3D11ShaderResourceView* g_title_background_Texture = NULL;
static ID3D11ShaderResourceView* g_title_logo_text_Texture = NULL;
static ID3D11ShaderResourceView* g_title_logo_chain_Texture = NULL;
static ID3D11ShaderResourceView* g_title_logo_anchor_Texture = NULL;

//決定してステージにとぶまでにテクスチャなくなる問題を潰すためのくろ
static ID3D11ShaderResourceView* g_black_Texture = NULL;





void TitleScene::Initialize()
{
	g_title_background_Texture = InitTexture(L"asset\\texture\\title_texture\\title_background.jpg");
	g_title_logo_text_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_text.png");
	g_title_logo_chain_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_chain.png");
	g_title_logo_anchor_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_anchor.png");

	g_black_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
	

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

		scene_change_flag = true;
		
	};

	if (150 < scene_change_cnt)
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_STAGE_SELECT);
	}

	if (Keyboard_IsKeyDown(KK_R) || (state.buttonA))
	{
		text_sheet_cnt = 0;
	}




	//----------------------------------------------------------------------------------------
	//文字のテキストのシート管理
	int max_text_sheet = 32;
	if (text_sheet_cnt <= max_text_sheet)
	{
		text_sheet_cnt += 0.5f;
	}


	//フラグ　チェンジシシーンのフラグ
	if (scene_change_flag == true)
	{
		//----------------------------------------------------------------------------------------
		//チェインのシートカウント
		int max_chain_sheet = 47;
		if (chain_sheet_cnt <= max_chain_sheet)
		{
			chain_sheet_cnt += 0.5;
		}

		//----------------------------------------------------------------------------------------
		//アンカーのシートカウント
		int max_anchor_sheet = 47;
		if (anchor_sheet_cnt <= max_anchor_sheet)
		{
			anchor_sheet_cnt += 0.5;
		}

		scene_change_cnt++;

		
		if (black_fade_cnt<0.4)
		{
			black_fade_cnt += 0.005;
		}
		
	}



}

void TitleScene::Draw()
{

	//バッファクリア
	Clear();

	//2D描画なので深度無効
	SetDepthEnable(false);


	//背景
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

	if (g_black_Texture)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_black_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
			black_fade_cnt
		);
	}

	//ロゴのチェイン
	if (g_title_logo_chain_Texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_chain_Texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			7, 7, chain_sheet_cnt
		);
	}

	//ロゴのテキスト
	if (g_title_logo_text_Texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_text_Texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
			6,6, text_sheet_cnt
		);
	}

	//アンカーのテキスト
	if (g_title_logo_anchor_Texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_anchor_Texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			7, 7, anchor_sheet_cnt
		);
	}




	//バックバッファ、フロントバッファ入れ替え
	Present();

}

void TitleScene::Finalize()
{
	if (g_title_background_Texture) UnInitTexture(g_title_background_Texture);
	if (g_title_logo_chain_Texture) UnInitTexture(g_title_logo_chain_Texture);
	if (g_title_logo_text_Texture) UnInitTexture(g_title_logo_text_Texture);
	if (g_title_logo_anchor_Texture) UnInitTexture(g_title_logo_anchor_Texture);
	if (g_black_Texture) UnInitTexture(g_black_Texture);



}