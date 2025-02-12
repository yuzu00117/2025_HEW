#include"player_UI.h"
#include"texture.h"
#include"sprite.h"
#include"anchor_spirit.h"
#include"player_stamina.h"


//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_ring_Texture = NULL;//真ん中の宝石の入れ物的なやつ

ID3D11ShaderResourceView* g_red_jewel_Texture = NULL;	//宝石　赤
ID3D11ShaderResourceView* g_blue_jewel_Texture = NULL;	//宝石　青
ID3D11ShaderResourceView* g_yellow_jewel_Texture = NULL;//宝石　黄


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
DirectX::XMFLOAT2 player_UI::player_ui_position = DirectX::XMFLOAT2(155.f, 390.f);
DirectX::XMFLOAT2 player_UI::player_ui_size = DirectX::XMFLOAT2(350.f, 700.f);

DirectX::XMFLOAT2 player_UI::gauge_only_position = DirectX::XMFLOAT2(98.f, 355.f);
DirectX::XMFLOAT2 player_UI::gauge_only_size = DirectX::XMFLOAT2(74.f, 478.f);

float player_UI::player_ui_alpha = 1.0f;

bool	player_UI::m_blue_jewel_collected = false;
bool	player_UI::m_red_jewel_collected = false;
bool	player_UI::m_yellow_jewel_collected = false;

DirectX::XMFLOAT2 player_UI::m_ring_position = player_ui_position;

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
	g_soul_gage_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_soul_no_white.png");
	g_soul_gage_HP_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_HP_no_white.png");
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
	float temp_stamina_position_y;	//体力ゲージが移動した後の座標
	float temp_stamina_scale_y;		//体力ゲージの今の大きさ
	float temp_spirit_position_y;  //ソウルゲージが移動した後の座標　
	float temp_spirit_scale_y;	   //ソウルゲージの今の大きさ
	XMFLOAT2	scale;

	



	
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


	if (true) //ここはアンカーゲージのHP　数値によって変更が必要
	{
		//今のアンカーのソウル値を取る
		float stamina = PlayerStamina::GetPlayerStaminaValue();

		//テクスチャUVを弄って表示しているので（真ん中を中心にサイズ変わっちゃから、それを左中心にしたい）下の処理をする
		//Max状態の時の長さに比例して、今のソウル値の長さを調整
		temp_stamina_scale_y = (stamina / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * gauge_only_size.y;
		scale = XMFLOAT2(gauge_only_size.x, temp_stamina_scale_y);
		//Max状態の時の位置に比例して、今のソウル値の場合の位置に移動
		//temp_position_x = (stamina / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_position.x;
		temp_stamina_position_y = (gauge_only_size.y / 2) - (temp_stamina_scale_y / 2) + gauge_only_position.y;


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_HP_Texture);
		DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_stamina_position_y), 0.0f, scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, (int)MAX_STAMINA + 1, (int)stamina);

	}



	if (true) //ここはアンカーゲージの　数値によって変更が必要
	{

		//今のアンカーのソウル値を取る
		float spirit = AnchorSpirit::GetAnchorSpiritValue();

		//テクスチャUVを弄って表示しているので（真ん中を中心にサイズ変わっちゃから、それを左中心にしたい）下の処理をする
		//Max状態の時の長さに比例して、今のソウル値の長さを調整
		temp_spirit_scale_y = (spirit / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * gauge_only_size.y;
		scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
		//Max状態の時の位置に比例して、今のソウル値の場合の位置に移動
		temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;


		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_Texture);

		DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, (int)MAX_STAMINA + 1, (int)spirit);
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

	if (m_red_jewel_collected)//赤の宝石
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

	if (m_blue_jewel_collected)//青の宝石
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

	if (m_yellow_jewel_collected)//黄色の宝石
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

	if (g_ring_Texture != NULL)
	{
		//本体ぽいやつ
		UnInitTexture(g_ring_Texture);
		//宝石
		UnInitTexture(g_red_jewel_Texture);
		UnInitTexture(g_blue_jewel_Texture);
		UnInitTexture(g_yellow_jewel_Texture);

		//ソウルゲージ達
	
		UnInitTexture(g_soul_gage_Texture);
		UnInitTexture(g_soul_gage_HP_Texture);
		UnInitTexture(g_soul_gage_border_Texture);

		//アンカーレベルのテクスチャたち
		UnInitTexture(g_anchor_level_background_Texture);
		UnInitTexture(g_anchor_level_1_Texture);
		UnInitTexture(g_anchor_level_2_Texture);
		UnInitTexture(g_anchor_level_3_Texture);
		UnInitTexture(g_anchor_level_border_Texture);

		UnInitTexture(g_anchor_level_division_Texture);
		UnInitTexture(g_anchor_level_outline_Texture);

		g_ring_Texture = NULL;

		g_red_jewel_Texture = NULL;
		g_blue_jewel_Texture = NULL;
		g_yellow_jewel_Texture = NULL;

	
		g_soul_gage_Texture = NULL;
		g_soul_gage_HP_Texture = NULL;
		g_soul_gage_border_Texture = NULL;

		g_anchor_level_background_Texture = NULL;
		g_anchor_level_1_Texture = NULL;
		g_anchor_level_2_Texture = NULL;
		g_anchor_level_3_Texture = NULL;
		g_anchor_level_border_Texture = NULL;


		g_anchor_level_division_Texture = NULL;
		g_anchor_level_outline_Texture = NULL;
		
	}
}

