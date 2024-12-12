

#include"title.h"
#include "sprite.h"
#include "texture.h"
#include"scene.h"
#include"Xinput_controller.h"
#include"keyboard.h"



//グローバル変数
static ID3D11ShaderResourceView* g_Texture = NULL;

//初期化処理
void InitializeTitle(void)
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
}

//終了処理
void FinalizeTitle(void)
{
	//テクスチャの解放
	UnInitTexture(g_Texture);
}

//更新処理
void UpdateTitle(void) 
{
	//ゲームのセレクトに移行
	if (Keyboard_IsKeyDown(KK_SPACE))
	{
		ChangeScene(SCENE_GAMESELECT);
	}
	
}

//描画処理
void DrawTitle(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
