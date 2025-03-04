//-----------------------------------------------------------------------------------------------------
// #name OP.cpp
// #description オープニングシーン
// #make 2025/2/7　王泳心
// #update 2025/2/12
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------
#include "OP.h"
#include "keyboard.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include"Xinput.h"
#include"Xinput_controller.h"

static ID3D11ShaderResourceView* g_pause_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_skip_texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_skip_button_texture = NULL;//アンカーのテクスチャ

static bool g_pressed = false;

OP::OP()
{
}

OP::~OP()
{
}

void OP::Initialize()
{
	video.Initialize("asset/movie/hew_movie.mp4", false);
	g_pause_Texture = InitTexture(L"asset\\texture\\sample_texture\\video_pause.png");
	g_skip_texture = InitTexture(L"asset\\texture\\sample_texture\\skip.png");
	g_skip_button_texture = InitTexture(L"asset\\texture\\sample_texture\\skip_a.png");


	m_pause = false;
	
}

void OP::Update()
{
	bool finished = video.Update();

	if (finished)
	{
		SceneManager& sceneManager = SceneManager::GetInstance();


		sceneManager.ChangeScene(SCENE_STAGE_SELECT);
		return;

	}
#ifdef _DEBUG
	if (!g_pressed && Keyboard_IsKeyDownTrigger(KK_X))
	{
		Video_State state = video.GetState();
		if (state == Video_Start)
		{
			video.SetState(Video_Pause);
			m_pause = !m_pause;
		}
		else if (state == Video_Pause)
		{
			video.SetState(Video_Resume);
			m_pause = !m_pause;
		}
		else if (state == Video_Resume)
		{
			video.SetState(Video_Pause);
			m_pause = !m_pause;
		}
	}
	g_pressed = Keyboard_IsKeyDownTrigger(KK_X);

	if (Keyboard_IsKeyDownTrigger(KK_S))
	{
		video.SetState(Video_Skip);
	}
#endif // _DEBUG

	//コントローラーの入力の受け取り
	ControllerState state = GetControllerInput();
	if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
	{
		skip_cnt += 0.5;
	}
	else
	{
		if (skip_cnt != 0)
		{
			skip_cnt -= 0.5f;
		}

		if (skip_cnt < 0)
		{
			skip_cnt = 0;
		}
	}

	if (30 < skip_cnt)
	{
  		video.SetState(Video_Skip);
		skip_cnt = 0;
	}
	
}

void OP::Draw()
{
	//バッファクリア
	Clear();

	if (m_pause)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_pause_Texture);

		DrawSpriteOld(
			{ SCREEN_XCENTER, SCREEN_XCENTER },
			0.0f,
			{ 100.0f,100.0f }
		);

	}

 	video.Draw(XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),SCREEN_HEIGHT);

	if (g_skip_texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_skip_texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER+540, SCREEN_YCENTER + 300),
			0.0f,
			XMFLOAT2(200, 70),
			1, 1, 1
		);
	}

	if (g_skip_button_texture != nullptr)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_skip_button_texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER + 475, SCREEN_YCENTER + 300),
			0.0f,
			XMFLOAT2(100, 75),
			10, 3, skip_cnt
		);
	}

	//バックバッファ、フロントバッファ入れ替え
	Present();

}

void OP::Finalize()
{
	video.Finalize();
	if (g_pause_Texture) { UnInitTexture(g_pause_Texture); }
	if (g_skip_button_texture) { UnInitTexture(g_skip_button_texture); }
	if (g_skip_texture) { UnInitTexture(g_skip_texture); }
	
}
