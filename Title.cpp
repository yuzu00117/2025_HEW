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
static ID3D11ShaderResourceView* g_title_logo_text2_Texture = NULL;
static ID3D11ShaderResourceView* g_title_logo_text3_Texture = NULL;

//決定してステージにとぶまでにテクスチャなくなる問題を潰すためのくろ
static ID3D11ShaderResourceView* g_black_Texture = NULL;

static ID3D11ShaderResourceView* g_bottom_A_texture = NULL;

static ID3D11ShaderResourceView* g_start_texture = NULL;





void TitleScene::Initialize()
{
	g_title_background_Texture = InitTexture(L"asset\\texture\\title_texture\\title_background.jpg");
	g_title_logo_text_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_text_start.png");
	g_title_logo_text2_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_text_loop.png");
	g_title_logo_text3_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_text_end.png");

	g_black_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
	
	g_bottom_A_texture = InitTexture(L"asset\\texture\\sample_texture\\A_push.png");

	g_start_texture = InitTexture(L"asset\\texture\\sample_texture\\UI_start_A.png");

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
		sceneManager.ChangeScene(SCENE_OP);
	}

	
	




	//----------------------------------------------------------------------------------------
	//文字のテキストのシート管理
	int max_text_sheet = 72;
	if (text_sheet_cnt < max_text_sheet)
	{
		text_sheet_cnt += 0.5f;

	}
	else
	{
		text_sheet_cnt = 36;
	}


	//フラグ　チェンジシシーンのフラグ
	if (scene_change_flag == true)
	{
		scene_change_cnt++;
		
		if (bottom_sheet_cnt < 15)
		{
			bottom_sheet_cnt += 0.8;
		}

		if (text_sheet_end_cnt < 15)
		{
			text_sheet_end_cnt += 0.3;
		}
		
	}

	





}

void TitleScene::Draw()
{

	//バッファクリア
	Clear();

	//2D描画なので深度無効
	SetDepthEnable(false);


	float draw_x = 685;

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

	if (scene_change_flag == false)
	{
		if (text_sheet_cnt < 36)
		{
			//ロゴのチェイン
			if (g_title_logo_text_Texture != nullptr)
			{
				// シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_text_Texture);

				DrawDividedSprite(
					XMFLOAT2(draw_x, SCREEN_YCENTER),
					0.0f,
					XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
					6, 6, text_sheet_cnt
				);
			}
		}
		else if (36 <= text_sheet_cnt && text_sheet_cnt <= 72)
		{
			//ロゴのテキスト
			if (g_title_logo_text2_Texture != nullptr)
			{
				// シェーダリソースを設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_text2_Texture);

				DrawDividedSprite(
					XMFLOAT2(draw_x, SCREEN_YCENTER),
					0.0f,
					XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
					6, 6, text_sheet_cnt
				);
			}

		}
	}


	if (text_sheet_end_cnt != 0)
	{
		//選択後
		if (g_title_logo_text3_Texture != nullptr)
		{
			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_text3_Texture);

			DrawDividedSprite(
				XMFLOAT2(draw_x, SCREEN_YCENTER),
				0.0f,
				XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
				4, 4, text_sheet_end_cnt
			);
		}
	}

	if (g_start_texture != nullptr)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_start_texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER+500, SCREEN_YCENTER + 300),
			0.0f,
			XMFLOAT2(200,90),
			1, 1, 1
		);
	}


	if (g_bottom_A_texture != nullptr)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_bottom_A_texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER+440, SCREEN_YCENTER + 300),
			0.0f,
			XMFLOAT2(100, 75),
			5, 4, bottom_sheet_cnt
		);
	}




	//バックバッファ、フロントバッファ入れ替え
	Present();

}

void TitleScene::Finalize()
{
	if (g_title_background_Texture) UnInitTexture(g_title_background_Texture);
	if (g_title_logo_text_Texture) UnInitTexture(g_title_logo_text_Texture);
	if (g_title_logo_text2_Texture) UnInitTexture(g_title_logo_text2_Texture);
	if (g_title_logo_text3_Texture) UnInitTexture(g_title_logo_text3_Texture);
	if (g_black_Texture) UnInitTexture(g_black_Texture);
	if (g_bottom_A_texture) UnInitTexture(g_bottom_A_texture);
	if (g_start_texture) UnInitTexture(g_start_texture);
}