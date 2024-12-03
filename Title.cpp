//-----------------------------------------------------------------------------------------------------
// #name square_collider.cpp
// #description 四角のあたり判定
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//          ・ここで四角のあたり判定のコンストラクタしてる
// 　　　　 ・ここでほとんどの処理を管理してる
//          ・引数足らない気がするもっといろいろつけれるけどデフォルトはこれとして、これの継承さきで作ろうかなって思ってる（反発係数とかね）
// 　　　　 ・とあいえ結構な情報入っているので、いや分けろよって言われたら　うんってなる　あんまり細分化してもやりずらくねって思ったけど　かえるなら別にいいよ
//          
//----------------------------------------------------------------------------------------------------


#include "Title.h"
#include "sprite.h"
#include "texture.h"

//マクロ定義
#define RADIUS 300.0f
#define INNNER_RAD (((NUM_VERTEX - 2) * M_PI) / NUM_VERTEX)

//グローバル変数
static ID3D11ShaderResourceView* g_Texture = NULL;

//初期化処理
void InitTitle(void)
{
	g_Texture = InitTexture(L"asset\\texture\\majo002.jpg");
}

//終了処理
void UninitTitle(void)
{
	//テクスチャの解放
	UnInitTexture(g_Texture);
}

//更新処理
void UpdateTitle(void) 
{
	
}

//描画処理
void DrawTitle(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
