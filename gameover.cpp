#include "gameover.h"
#include "sprite.h"
#include "texture.h"
#include "keyboard.h"
#include "scene.h"




//グローバル変数
static ID3D11ShaderResourceView* g_Texture = NULL;

//初期化処理
void InitializeGameOver(void)
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_Yellow.png");
}

//終了処理
void FinalizeGameOver(void)
{
	//テクスチャの解放
	UnInitTexture(g_Texture);
}

//更新処理
void UpdateGameOver(void)
{
	//タイトル画面に移行
	if (Keyboard_IsKeyDown(KK_M))
	{
		ChangeScene(SCENE_TITLE);
	}

	//ゲームのセレクトに移行
	if (Keyboard_IsKeyDown(KK_N))
	{
		ChangeScene(SCENE_GAMESELECT);
	}
}

//描画処理
void DrawGameOver(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
