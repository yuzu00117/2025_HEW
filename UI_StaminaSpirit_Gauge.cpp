#include"UI_StaminaSpirit_Gauge.h"
#include"anchor_spirit.h"
#include"sprite.h"
#include"texture.h"
#include"renderer.h"
#include"player_stamina.h"



//テクスチャのダウンロード グローバル変数にしてる


//体力ゲージとソウルゲージの画像が一つにまとめてる場合のテクスチャ
ID3D11ShaderResourceView* g_gauge_Texture;

//体力ゲージとソウルゲージの画像が分かれている場合のテクスチャ
//ID3D11ShaderResourceView* g_spirit_gauge_Texture;
//ID3D11ShaderResourceView* g_stamina_gauge_Texture;


void	StaminaSpiritGauge::Initialize()
{
	//テクスチャのロード
	//体力ゲージとソウルゲージの画像が一つにまとめてる場合のテクスチャ
	g_gauge_Texture = InitTexture(L"asset\\texture\\sample_texture\\gauge.png");

	//体力ゲージとソウルゲージの画像が分かれている場合のテクスチャ
	//g_spirit_gauge_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");
	//g_stamina_gauge_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");


}

void	StaminaSpiritGauge::Finalize()
{
	//体力ゲージとソウルゲージの画像が一つにまとめてる場合の処理
	if (g_gauge_Texture != nullptr)
	{
		UnInitTexture(g_gauge_Texture);
	}	

	//体力ゲージとソウルゲージの画像が分かれている場合の処理
	//if (g_spirit_gauge_Texture != nullptr)
	//{
	//	UnInitTexture(g_spirit_gauge_Texture);
	//}	
	//if (g_stamina_gauge_Texture != nullptr)
	//{
	//	UnInitTexture(g_stamina_gauge_Texture);
	//}
}

void	StaminaSpiritGauge::Draw()
{
	//体力ゲージとソウルゲージの画像が一つにまとめてる場合の描画処理
	{
		float stamina = PlayerStamina::GetPlayerStaminaValue();
		float spirit = AnchorSpirit::GetAnchorSpiritValue();

		float total_value = stamina + spirit;

		XMFLOAT2 temp_scale;
		temp_scale.x = (total_value / (MAX_STAMINA + MAX_ANCHOR_SPIRIT)) * m_scale.x;
		temp_scale.y = m_scale.y;

		XMFLOAT2 temp_position;
		temp_position.x = (temp_scale.x / m_scale.x) * m_position.x;
		temp_position.y = m_position.y;

		float rotate = 0.0f;

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_gauge_Texture);
		//描画
		DrawSerialDividedSprite(temp_position, rotate, temp_scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, 0, (int)total_value);

	}

	////体力ゲージとソウルゲージの画像が分かれている場合の描画処理
	//----------------------------------------------------------------------------------------------------------------------------------------------
	//{
	//	float temp_stamina_position_x;
	//	float temp_stamina_scale_x;
	//	float temp_spirit_position_x;
	//	float temp_spirit_scale_x;
	//	float rotate;
	//	XMFLOAT2	scale;

	//	//体力ゲージ
	//	{
	//		//今のアンカーのソウル値を取る
	//		float stamina = PlayerStamina::GetPlayerStaminaValue();

	//		//テクスチャUVを弄って表示しているので（真ん中を中心にサイズ変わっちゃから、それを左中心にしたい）下の処理をする
	//		//Max状態の時の長さに比例して、今のソウル値の長さを調整
	//		temp_stamina_scale_x = (stamina / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_scale.x;
	//		scale = XMFLOAT2(temp_stamina_scale_x, m_scale.y);
	//		//Max状態の時の位置に比例して、今のソウル値の場合の位置に移動
	//		//temp_position_x = (stamina / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_position.x;
	//		temp_stamina_position_x = (temp_stamina_scale_x / m_scale.x) * m_position.x;


	//		rotate = 0.0f;
	//		// シェーダリソースを設定
	//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stamina_gauge_Texture);
	//		//描画
	//		DrawSerialDividedSprite(XMFLOAT2(temp_stamina_position_x, m_position.y), rotate, scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, 0, (int)stamina);

	//	}
	//	//ソウルゲージ
	//	{
	//		//今のアンカーのソウル値を取る
	//		float spirit = AnchorSpirit::GetAnchorSpiritValue();

	//		//テクスチャUVを弄って表示しているので（真ん中を中心にサイズ変わっちゃから、それを左中心にしたい）下の処理をする
	//		//Max状態の時の長さに比例して、今のソウル値の長さを調整
	//		temp_spirit_scale_x = (spirit / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_scale.x;
	//		scale = XMFLOAT2(temp_spirit_scale_x, m_scale.y);
	//		//Max状態の時の位置に比例して、今のソウル値の場合の位置に移動
	//		temp_spirit_position_x = temp_stamina_scale_x + (temp_spirit_scale_x / 2);
	//		//temp_position_x = (spirit / (MAX_ANCHOR_SPIRIT + MAX_STAMINA)) * m_position.x;


	//		rotate = 0.0f;
	//		// シェーダリソースを設定
	//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_spirit_gauge_Texture);
	//		//描画
	//		DrawSerialDividedSprite(XMFLOAT2(temp_spirit_position_x, m_position.y), rotate, scale, (int)MAX_STAMINA + (int)MAX_ANCHOR_SPIRIT, 1, (int)MAX_STAMINA + 1, (int)spirit);

	//	}

	//}//end_体力ゲージとソウルゲージの画像が分かれている場合の描画処理


}