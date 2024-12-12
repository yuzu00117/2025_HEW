//-----------------------------------------------------------------------------------------------------
// #name display.cpp
// #description 画面のスケールのを変更できる　と言っても　この中でするというよりは　スプライトの中でしてる
// #make 2024/12/12　　永野義也
// #update 2024/12/12
// #comment 追加・修正予定
//          ・特になし
//----------------------------------------------------------------------------------------------------

#include"display.h"
#include"keyboard.h"
#include"anchor_spirit.h"

// 静的メンバ変数の定義（1回だけ行う）

float display::m_display_scale = 1;//スケールの初期値の倍率

float display::m_display_width = 650;//横幅
float display::m_display_height =300;//縦


display::display()
{
}

display::~display()
{
}

void display::Update()
{
	//デバック用

	//if (Keyboard_IsKeyDown(KK_D0))
	//{
	//	SetDisplayScale(-0.1f);
	//}
	//if (Keyboard_IsKeyDown(KK_D9))
	//{
	//	SetDisplayScale(+0.1);
	//}

	//if (Keyboard_IsKeyDown(KK_D8))
	//{
	//	SetDisplayWidth(-10);
	//}
	//if (Keyboard_IsKeyDown(KK_D7))
	//{
	//	SetDisplayWidth(+10);
	//}


	//-----------------------------------------------------------------------------------------------------------
	//アンカーのレベルに応じた変更


	if (AnchorSpirit::GetAnchorLevel() == 3)
	{
		if (GetDisplayScale() >= 0.5)
		{
			SetDisplayScale(-0.01);
		}
	}
	else
	{
		if (GetDisplayScale() <= 1)
		{
			SetDisplayScale(0.01);
		}
	}

	
}

//スケーリング
float display::GetDisplayScale()
{
	return m_display_scale;
}
void display::SetDisplayScale(float scale)
{
	m_display_scale = m_display_scale + scale;
}

//マップの横
float display::GetDisplayWidth()
{
	return m_display_width;
}
void display::SetDisplayWidth(float width)
{
	m_display_width = m_display_width + width;
}

//マップの縦
float display::GetDisplayHeight()
{
	return m_display_height;
}
void display::SetDisplayHeight(float height)
{
	m_display_height = m_display_height + height;
}