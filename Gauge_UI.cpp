#include"Gauge_UI.h"
#include"texture.h"
#include"sprite.h"
#include"anchor_spirit.h"
#include"player_stamina.h"


//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_ring_Texture = NULL;//真ん中の宝石の入れ物的なやつ

ID3D11ShaderResourceView* g_red_jewel_Texture = NULL;	//宝石　赤
ID3D11ShaderResourceView* g_blue_jewel_Texture = NULL;	//宝石　青
ID3D11ShaderResourceView* g_yellow_jewel_Texture = NULL;//宝石　黄

ID3D11ShaderResourceView* g_red_jewel_fit_effect = NULL;	//宝石　赤
ID3D11ShaderResourceView* g_blue_jewel_fit_effect = NULL;	//宝石　青
ID3D11ShaderResourceView* g_yellow_jewel_fit_effect = NULL;//宝石　黄
ID3D11ShaderResourceView* g_soul_gage_blue_Texture = NULL;          //ソウルゲージのテクスチャ（青）
ID3D11ShaderResourceView* g_soul_gage_yellow_Texture = NULL;        //ソウルゲージのテクスチャ（黄色）
ID3D11ShaderResourceView* g_soul_gage_red_Texture = NULL;           //ソウルゲージのテクスチャ（赤）
ID3D11ShaderResourceView* g_soul_gage_border_Texture = NULL;	//ソウルゲージの外枠
ID3D11ShaderResourceView* g_anchor_level_division_Texture = NULL;  //アンカーレベルの仕切り
ID3D11ShaderResourceView* g_anchor_level_outline_Texture = NULL;  //アンカーレベルの外側の装飾




// 静的メンバー変数の初期化
DirectX::XMFLOAT2 Gauge_UI::player_ui_position = DirectX::XMFLOAT2(155.f, 390.f);
DirectX::XMFLOAT2 Gauge_UI::player_ui_size = DirectX::XMFLOAT2(350.f, 700.f);

DirectX::XMFLOAT2 Gauge_UI::gauge_only_position = DirectX::XMFLOAT2(98.f, 355.f);	//枠を除いたゲージ色の部分のみの位置
DirectX::XMFLOAT2 Gauge_UI::gauge_only_size = DirectX::XMFLOAT2(74.f, 478.f);		//枠を除いたゲージ色の部分のみのサイズ

float Gauge_UI::player_ui_alpha = 1.0f;

bool	Gauge_UI::m_blue_jewel_collected = false;
bool	Gauge_UI::m_red_jewel_collected = false;
bool	Gauge_UI::m_yellow_jewel_collected = false;
float	Gauge_UI::m_blue_jewel_collected_effect_cnt = 0;
float	Gauge_UI::m_red_jewel_collected_effect_cnt = 0;
float	Gauge_UI::m_yellow_jewel_collected_effect_cnt = 0;
DirectX::XMFLOAT2 Gauge_UI::m_ring_position = player_ui_position;

Gauge_UI::Gauge_UI()
{
}

Gauge_UI::~Gauge_UI()
{
}

void Gauge_UI::Initialize()
{
	//テクスチャのロード

	//なんか本体ぽいやつ
	g_ring_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\ring.png");

	//宝石たち
	g_red_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\red_jewel.png");
	g_blue_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\blue_jewel.png");
	g_yellow_jewel_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\yellow_jewel.png");

	//宝石をゲットした時のエフェクト
	g_red_jewel_fit_effect = InitTexture(L"asset\\texture\\UI_soul_gage\\EFF_GemFit_Red_3x4.png");
	g_blue_jewel_fit_effect = InitTexture(L"asset\\texture\\UI_soul_gage\\EFF_GemFit_Blue_3x4.png");
	g_yellow_jewel_fit_effect = InitTexture(L"asset\\texture\\UI_soul_gage\\EFF_GemFit_Yellow_3x4.png");

	//ソウルゲージ達
	g_soul_gage_blue_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gauge_blue.png");
	g_soul_gage_yellow_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gauge_yellow.png");
	g_soul_gage_red_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gauge_red.png");
	g_soul_gage_border_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_border.png");

	//アンカーレベルのテクスチャたち
	g_anchor_level_division_Texture= InitTexture(L"asset\\texture\\UI_soul_gage\\gage_division.png");
	g_anchor_level_outline_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\gage_out_line.png");

	




}

void Gauge_UI::Update()
{

}

void Gauge_UI::Draw()
{	
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




	if (true) //ここはアンカーゲージの　数値によって変更が必要
	{
		int count_layer = 0;	//今何層目描いているのか
		//	今のアンカーレベルを取得
		int anchor_level = AnchorSpirit::GetAnchorLevel();
		//　もしソウルゲージがさっきダメージ入っていたのなら、ゲージの半透明のダメージを顧慮するために、半透明のやつの今のアンカーレベルを計算
		int prev_anchor_level = (AnchorSpirit::GetAnchorSpiritValue() + AnchorSpirit::GetAnchorSpiritDamage()) / 100.0f + 1;
		//半透明のやつの計算されたアンカーレベルが3を超えないための制御
		if (prev_anchor_level > 3) { prev_anchor_level = 3; }	
		//もし今実際のアンカーレベルが　1　レベルダウンしたけど、半透明のやつがまだ上のレベルにいる場合は、描画は半透明のやつのレベルに合わせる（まだレベルダウンしてない状態にしておく）
		// （実際ダメージはもうゲージに入ったけど、半透明は1フレームづつ減っていくから、こんな回りくどいしないといけない）
		if (anchor_level < prev_anchor_level) { anchor_level++; }

		//青色ゲージ
		if (anchor_level < 3)
		{
			//今のゲージ枠に相当するアンカーのソウル値を計算
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue();
			if (virtual_spirit > 100) { virtual_spirit = 100.0f; }

			//テクスチャUVを弄って表示しているので（真ん中を中心にサイズ変わっちゃから、それを左中心にしたい）下の処理をする
			//Max状態の時の長さに比例して、今のソウル値の長さを調整
			temp_spirit_scale_y = (virtual_spirit / 100) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max状態の時の位置に比例して、今のソウル値の場合の位置に移動
		//ブランチ前のバージョン	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;


			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_blue_Texture);

			DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, 100, 1, 1, (int)virtual_spirit);

			//もしこれが１層目なら次はダメージ層を描く
			count_layer++;
			if (count_layer == 1) { DrawGaugeDamaged(); }

		}
		//黄色ゲージ
		if (anchor_level > 1)
		{
			//今のゲージ枠に相当するアンカーのソウル値を計算
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue() - 100;
			if (virtual_spirit > 100) { virtual_spirit = 100.0f; }

			//テクスチャUVを弄って表示しているので（真ん中を中心にサイズ変わっちゃから、それを左中心にしたい）下の処理をする
			//Max状態の時の長さに比例して、今のソウル値の長さを調整
			temp_spirit_scale_y = (virtual_spirit / 100) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max状態の時の位置に比例して、今のソウル値の場合の位置に移動
		//ブランチ前のバージョン	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;


			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_yellow_Texture);

			DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, 100, 1, 1, (int)virtual_spirit);

			//もしこれが１層目なら次はダメージ層を描く
			count_layer++;
			if (count_layer == 1) { DrawGaugeDamaged(); }

		}
		//赤色ゲージ
		if (anchor_level == 3)
		{
			//今のゲージ枠に相当するアンカーのソウル値を計算
			float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue() - 200;

			//テクスチャUVを弄って表示しているので（真ん中を中心にサイズ変わっちゃから、それを左中心にしたい）下の処理をする
			//Max状態の時の長さに比例して、今のソウル値の長さを調整
			temp_spirit_scale_y = (virtual_spirit / 100) * gauge_only_size.y;
			scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
			//Max状態の時の位置に比例して、今のソウル値の場合の位置に移動
		//ブランチ前のバージョン	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
			temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;


			// シェーダリソースを設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_red_Texture);

			DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, 100, 1, 1, (int)virtual_spirit);

			//もしこれが１層目なら次はダメージ層を描く
			count_layer++;
			if (count_layer == 1) { DrawGaugeDamaged(); }

		}
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
	//宝石を取得した時のエフェクトの表示

	if (m_blue_jewel_collected_effect_cnt != 0)
	{

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_blue_jewel_fit_effect);

		DrawDividedSprite(XMFLOAT2(75, 600), 0.0f, XMFLOAT2(50, 50), 4, 3, m_blue_jewel_collected_effect_cnt, 1.0);

		m_blue_jewel_collected_effect_cnt += 0.3;

		if (12 < m_blue_jewel_collected_effect_cnt)
		{
			m_blue_jewel_collected_effect_cnt = 0;
		}
	}

	if (m_red_jewel_collected_effect_cnt != 0)
	{

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_red_jewel_fit_effect);

		DrawDividedSprite(XMFLOAT2(100, 650), 0.0f, XMFLOAT2(50, 50), 4, 3, m_red_jewel_collected_effect_cnt, 1.0);

		m_red_jewel_collected_effect_cnt += 0.3;

		if (12 < m_red_jewel_collected_effect_cnt)
		{
			m_red_jewel_collected_effect_cnt = 0;
		}
	}

	if (m_yellow_jewel_collected_effect_cnt != 0)
	{

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_yellow_jewel_fit_effect);

		DrawDividedSprite(XMFLOAT2(50, 650), 0.0f, XMFLOAT2(50, 50), 4, 3, m_yellow_jewel_collected_effect_cnt, 1.0);

		m_yellow_jewel_collected_effect_cnt += 0.3;

		if (12 < m_yellow_jewel_collected_effect_cnt)
		{
			m_yellow_jewel_collected_effect_cnt = 0;
		}
	}
	



}

void Gauge_UI::Finalize()
{

	if (g_ring_Texture) UnInitTexture(g_ring_Texture);

	if (g_red_jewel_Texture) UnInitTexture(g_red_jewel_Texture);
	if (g_blue_jewel_Texture) UnInitTexture(g_blue_jewel_Texture);
	if (g_yellow_jewel_Texture) UnInitTexture(g_yellow_jewel_Texture);

	if (g_red_jewel_fit_effect) UnInitTexture(g_red_jewel_fit_effect);
	if (g_blue_jewel_fit_effect) UnInitTexture(g_blue_jewel_fit_effect);
	if (g_yellow_jewel_fit_effect) UnInitTexture(g_yellow_jewel_fit_effect);

	if (g_soul_gage_blue_Texture) UnInitTexture(g_soul_gage_blue_Texture);
	if (g_soul_gage_yellow_Texture) UnInitTexture(g_soul_gage_yellow_Texture);
	if (g_soul_gage_red_Texture) UnInitTexture(g_soul_gage_red_Texture);
	if (g_soul_gage_border_Texture) UnInitTexture(g_soul_gage_border_Texture);

	if (g_anchor_level_division_Texture) UnInitTexture(g_anchor_level_division_Texture);
	if (g_anchor_level_outline_Texture) UnInitTexture(g_anchor_level_outline_Texture);

}

void Gauge_UI::DrawGaugeDamaged()
{
	//ダメージ受けてないなら描画を省く
	if (AnchorSpirit::GetAnchorSpiritDamage() <= 0)
	{
		return;
	}

	float temp_spirit_position_y;  //ソウルゲージが移動した後の座標　
	float temp_spirit_scale_y;	   //ソウルゲージの今の大きさ
	XMFLOAT2	scale;

	//ゲージ枠に相当する前フレームのアンカーのソウル値を計算
	float prev_virtual_spirit = (int)AnchorSpirit::GetAnchorSpiritValue() % 100 + AnchorSpirit::GetAnchorSpiritDamage();

	//テクスチャUVを弄って表示しているので（真ん中を中心にサイズ変わっちゃから、それを左中心にしたい）下の処理をする
	//Max状態の時の長さに比例して、今のソウル値の長さを調整
	temp_spirit_scale_y = (prev_virtual_spirit / 100) * gauge_only_size.y;
	scale = XMFLOAT2(gauge_only_size.x, temp_spirit_scale_y);
	//Max状態の時の位置に比例して、今のソウル値の場合の位置に移動
//ブランチ前のバージョン	//temp_spirit_position_y = temp_stamina_position_y - (temp_stamina_scale_y / 2) - (temp_spirit_scale_y / 2) - 1.0;
	temp_spirit_position_y = (gauge_only_size.y / 2) - (temp_spirit_scale_y / 2) + gauge_only_position.y;


	float virtual_spirit = AnchorSpirit::GetAnchorSpiritValue() + AnchorSpirit::GetAnchorSpiritDamage();
	if (virtual_spirit > 200.0f) {
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_red_Texture);
	}
	else if (virtual_spirit > 100.0f)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_yellow_Texture);
	}
	else
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_soul_gage_blue_Texture);
	}

	DrawSerialDividedSprite(XMFLOAT2(gauge_only_position.x, temp_spirit_position_y), 0.0f, scale, 100, 1, 1, (int)prev_virtual_spirit, 0.55f);


}

