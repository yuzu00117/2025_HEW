//-----------------------------------------------------------------------------------------------------
// #name Stamina_UI.cpp
// #description 体力のUI
// #make 2025/2/24　王泳心
// #update 2025/2/24
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------
#include "Stamina_UI.h"
#include "texture.h"
#include "sprite.h"
#include "player_stamina.h"



//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_player_icon_Texture = NULL;//プレイヤーアイコン付き枠
ID3D11ShaderResourceView* g_heart_frame_Texture = NULL;//ハートの枠
ID3D11ShaderResourceView* g_heart_content_Texture = NULL;//ハートの中身


DirectX::XMFLOAT2 Stamina_UI::player_icon_position = {160.0f, 70.0f};
DirectX::XMFLOAT2 Stamina_UI::player_icon_size = {300.0f, 120.0f};

float Stamina_UI::heart_sheet_cnt = 0;



DirectX::XMFLOAT2 Stamina_UI::heart_position[3] =
{
	{150.0f,75.0f},
	{200.0f,75.0f},
	{250.0f,75.0f}
};
DirectX::XMFLOAT2 Stamina_UI::heart_size[3] =
{
	{40.0f,40.0f},
	{40.0f,40.0f},
	{40.0f,40.0f}
};


Stamina_UI::Stamina_UI()
{
}

Stamina_UI::~Stamina_UI()
{
}

void Stamina_UI::Initialize()
{
	g_player_icon_Texture = InitTexture(L"asset\\texture\\UI_stamina\\player_icon3.png");
	g_heart_frame_Texture = InitTexture(L"asset\\texture\\UI_stamina\\heart_frame.png");
	g_heart_content_Texture = InitTexture(L"asset\\texture\\UI_stamina\\hp_pinch.png");
}

void Stamina_UI::Update()
{
	float player_stamina = PlayerStamina::GetPlayerStaminaValue();
	if (player_stamina <= 100)
	{
		heart_sheet_cnt += 0.7;
	}
	else
	{
		heart_sheet_cnt += 0.3;
	}

	if (30 < heart_sheet_cnt)
	{
		heart_sheet_cnt = 0;
	}
}

void Stamina_UI::Draw()
{
	//プレイヤーアイコン付き枠を表示
// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_icon_Texture);

	DrawSpriteOld(
		{ player_icon_position },
		0,
		{ player_icon_size },
		1.0f
	);

	float player_stamina = PlayerStamina::GetPlayerStaminaValue();

	for (int i = 0; i < 3; i++)
	{

		//ハートの枠を表示
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_heart_frame_Texture);

		DrawSpriteOld(
			{ heart_position[i] },
			0,
			{ heart_size[i] },
			1.0f
		);

		float ratio_of_filling = (int)player_stamina % 100 /100.0f;
		if (player_stamina >= (i+1)*100) { ratio_of_filling = 1.0f; }
		else if (player_stamina < i * 100) { ratio_of_filling = 0.0f; }

		XMFLOAT2 draw_scale;
		draw_scale.x = ratio_of_filling * heart_size[i].x;
		draw_scale.y = heart_size[i].y;

		XMFLOAT2 draw_position;
		draw_position.x = heart_position[i].x - (heart_size[i].x - draw_scale.x) / 2;
		draw_position.y = heart_position[i].y;

		float rotate = 0.0f;

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_heart_content_Texture);
		
		

		//描画
		DrawDividedSprite(draw_position, rotate, { 93,65 }, 10, 3, heart_sheet_cnt, ratio_of_filling * 100);
	}
}

void Stamina_UI::Finalize()
{
	if (g_player_icon_Texture) UnInitTexture(g_player_icon_Texture);
	if (g_heart_frame_Texture) UnInitTexture(g_heart_frame_Texture);
	if (g_heart_content_Texture) UnInitTexture(g_heart_content_Texture);

}
