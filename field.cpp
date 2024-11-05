#include "field.h"
#include "sprite.h"
#include "texture.h"

//マクロ定義
#define RADIUS 300.0f
#define INNNER_RAD (((NUM_VERTEX - 2) * M_PI) / NUM_VERTEX)

//グローバル変数
static ID3D11ShaderResourceView* g_Texture = NULL;

//初期化処理
void InitField(void)
{
	g_Texture = InitTexture(L"asset\\texture\\BG.png");
}

//終了処理
void UninitField(void)
{
	//テクスチャの解放
	UnInitTexture(g_Texture);
}

//更新処理
void UpdateField(void)
{

}

//描画処理
void DrawField(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
