//-----------------------------------------------------------------------------------------------------
// #name display.h
// #description 画面のスケールのを変更できる　と言っても　この中でするというよりは　スプライトの中でしてる
// #make 2024/12/12　　永野義也
// #update 2024/12/12
// #comment 追加・修正予定
//          ・特になし
//----------------------------------------------------------------------------------------------------

#ifndef DESPLAYE_H
#define DESPLAYE_H

class display
{
public:
	display();
	~display();

	
	static void Update();

	//スケーリング
	static float GetDisplayScale();
	
	static void SetDisplayScale(float scale);


	//マップの横
	static float GetDisplayWidth();

	static void AddDisplayWidth(float width);

	static void SetDisplayWidth(float width);

	//マップの縦
	static float GetDisplayHeight();

	static void AddDisplayHeight(float height);

	static void SetDisplayHeight(float height);

private:
	static float m_display_scale;
	static float m_display_width;
	static float m_display_height;
};


#endif // !DESPLAYE_H
