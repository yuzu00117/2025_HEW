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
#include"tool.h"

// 静的メンバ変数の定義（1回だけ行う）
//表示範囲を渡して　その範囲を表示するための拡大率を受け取っている
float display::m_display_scale = calculateScale(DISPLAY_RANGE_TO_SCALE);//スケールの初期値の倍率



float display::m_display_width = DEFAULT_DISPLAY_WIDTH;//横幅
float display::m_display_height = DEFAULT_DISPLAY_HEIGHT;//縦


display::display()
{
}

display::~display()
{
}

void display::Update()
{
#ifdef _DEBUG
	//デバック用

	if (Keyboard_IsKeyDown(KK_D0))
	{
		SetDisplayScale(-0.01f);
	}
	if (Keyboard_IsKeyDown(KK_D9))
	{
		SetDisplayScale(+0.01);
	}

	if (Keyboard_IsKeyDown(KK_D8))
	{
		SetDisplayWidth(-10);
	}
	if (Keyboard_IsKeyDown(KK_D7))
	{
		SetDisplayWidth(+10);
	}
#endif

	//-----------------------------------------------------------------------------------------------------------
	//アンカーのレベルに応じた変更


	if (AnchorSpirit::GetAnchorLevel() == 3)
	{
		//自動で倍率を調整するよ
		if (GetDisplayScale() >= 0.7 * calculateScale(DISPLAY_RANGE_TO_SCALE))
		{
			SetDisplayScale(-0.01);
		}
	}
	else
	{
		//自動で倍率を調整するよ
		if (GetDisplayScale() <= 1 * calculateScale(DISPLAY_RANGE_TO_SCALE))
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
	m_display_width = width;
}

//マップの縦
float display::GetDisplayHeight()
{
	return m_display_height;
}
void display::SetDisplayHeight(float height)
{
	m_display_height = height;
}


//マップの横

void display::AddDisplayWidth(float width)
{
	m_display_width = m_display_width + width;
}


void display::AddDisplayHeight(float height)
{
	m_display_height = m_display_height + height;
}

//初期値に戻す
void display::SetDisplayDefault(void)
{
	m_display_width = DEFAULT_DISPLAY_WIDTH;
	m_display_height = DEFAULT_DISPLAY_HEIGHT;
}