#include"player_UI.h"
#include"texture.h"
#include"sprite.h"
#include"anchor_spirit.h"


//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_ring_Texture = NULL;//真ん中の宝石の入れ物的なやつ

ID3D11ShaderResourceView* g_red_jewel_Texture = NULL;	//宝石　赤
ID3D11ShaderResourceView* g_blue_jewel_Texture = NULL;	//宝石　青
ID3D11ShaderResourceView* g_yellow_jewel_Texture = NULL;//宝石　黄

ID3D11ShaderResourceView* g_soul_gage_background_Texture = NULL;//ソウルゲージの背景
ID3D11ShaderResourceView* g_soul_gage_Texture = NULL;           //ソウルゲージのテクスチャ
ID3D11ShaderResourceView* g_soul_gage_HP_Texture = NULL;		//ソウルゲージのHP部分
ID3D11ShaderResourceView* g_soul_gage_border_Texture = NULL;	//ソウルゲージの外枠


ID3D11ShaderResourceView* g_anchor_level_background_Texture = NULL;//アンカーのゲージのバックグランド
ID3D11ShaderResourceView* g_anchor_level_1_Texture = NULL;		   //アンカーレベル１のテクスチャ
ID3D11ShaderResourceView* g_anchor_level_2_Texture = NULL;		   //アンカーレベル2のテクスチャ
ID3D11ShaderResourceView* g_anchor_level_3_Texture = NULL;		   //アンカーレベル3のテクスチャ
ID3D11ShaderResourceView* g_anchor_level_border_Texture = NULL;	   //アンカーレベルの外枠
ID3D11ShaderResourceView* g_anchor_level_division_Texture = NULL;  //アンカーレベルの仕切り
ID3D11ShaderResourceView* g_anchor_level_outline_Texture = NULL;  //アンカーレベルの外側の装飾

// 静的メンバー変数の初期化
DirectX::XMFLOAT2 player_UI::player_ui_position = DirectX::XMFLOAT2(130.f, 450.f);
DirectX::XMFLOAT2 player_UI::player_ui_size = DirectX::XMFLOAT2(250.f, 500.f);
float player_UI::player_ui_alpha = 1.0f;

player_UI::player_UI()
{
}

player_UI::~player_UI()
{
}

void player_UI::Initialize()
{
	//テクスチャのロード

	//なんか本体ぽいやつ
	g_ring_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\ring.png");

	//宝石たち
	g_red_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\red_jewel.png");
	g_blue_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\blue_jewel.png");
	g_yellow_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\yellow_jewel.png");

	//ソウルゲージ達
	/*g_soul_gage_background_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_back_ground.png");*/
	g_soul_gage_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_soul .png");
	g_soul_gage_HP_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_HP.png");
	g_soul_gage_border_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_border.png");

	//アンカーレベルのテクスチャたち
	g_anchor_level_background_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\level_back_ground.png");
	g_anchor_level_1_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\level1.png");
	g_anchor_level_2_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\level2.png");
	g_anchor_level_3_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\level3.png");
	g_anchor_level_border_Texture= InitTexture(L"asset\\texture\\UI_soul_gage\\level_line.png");
	g_anchor_level_division_Texture= InitTexture(L"asset\\texture\\UI_soul_gage\\gage_division.png");
	g_anchor_level_outline_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_out_line.png");

	




}

void player_UI::Update()
{

}

void player_UI::Draw()
{




	
	//-------------------------------------------------------------------------------------------
	//アンカーゲージ

	//アンカーゲージの背景を表示

	//ゲージの外枠
	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_border_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);
	
	//// シェーダリソースを設定
	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_background_Texture);

	//DrawSpriteOld(
	//	{ player_ui_position },
	//	0,
	//	{ player_ui_size },
	//	player_ui_alpha
	//);





	if (true) //ここはアンカーゲージの　数値によって変更が必要
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	
	}

	if (true) //ここはアンカーゲージのHP　数値によって変更が必要
	{
		//シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_HP_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}


	//枠を表示
	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_division_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);
	

	//外側の装飾を表示
	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_outline_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);




	//-----------------------------------------------------------------------------------------

	//----------------------------------------------------------
	// ringの描画
	// 
	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_ring_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);



	//-------------------------------------------------------------------------------------------
	//宝石の描画

	if (true)//赤の宝石
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_red_jewel_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}

	if (true)//青の宝石
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_blue_jewel_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}

	if (true)//黄色の宝石
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_yellow_jewel_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}
	//-------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------
	//アンカーレベルの表示

	//アンカーレベルの背景
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_background_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);

	//アンカーレベル1
	if (AnchorSpirit::GetAnchorLevel() >= 1)
	{

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_1_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}

	//アンカーレベル2
	if (AnchorSpirit::GetAnchorLevel() >= 2)
	{
		
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_2_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}


	//アンカーレベル1
	if (AnchorSpirit::GetAnchorLevel() >= 3)
	{
		
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_3_Texture);

		DrawSpriteOld(
			{ player_ui_position },
			0,
			{ player_ui_size },
			player_ui_alpha
		);
	}


	//アンカーレベルの外枠
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_anchor_level_border_Texture);

	DrawSpriteOld(
		{ player_ui_position },
		0,
		{ player_ui_size },
		player_ui_alpha
	);


}

void player_UI::Finalize()
{
	//本体ぽいやつ
	UnInitTexture(g_ring_Texture);
	//宝石
	UnInitTexture(g_red_jewel_Texture);
	UnInitTexture(g_blue_jewel_Texture);
	UnInitTexture(g_yellow_jewel_Texture);

	//ソウルゲージ達
	UnInitTexture(g_soul_gage_background_Texture);
	UnInitTexture(g_soul_gage_border_Texture);
	UnInitTexture(g_soul_gage_HP_Texture);
	UnInitTexture(g_soul_gage_border_Texture);

	//アンカーレベルのテクスチャたち
	UnInitTexture(g_anchor_level_background_Texture);
	UnInitTexture(g_anchor_level_1_Texture);
	UnInitTexture(g_anchor_level_2_Texture);
	UnInitTexture(g_anchor_level_3_Texture);
	UnInitTexture(g_anchor_level_border_Texture);
}

