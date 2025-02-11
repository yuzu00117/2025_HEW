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

#define DEFAULT_DISPLAY_WIDTH (650)
#define DEFAULT_DISPLAY_HEIGHT (540)

#define DISPLAY_RANGE_TO_SCALE (0.6)

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

	static void SetDisplayWidth(float width);

	//マップの縦
	static float GetDisplayHeight();

	static void SetDisplayHeight(float height);



	static void AddDisplayWidth(float width);

	static void AddDisplayHeight(float height);



	static void SetDisplayDefault(void);


private:
	static float m_display_scale;
	static float m_display_width;
	static float m_display_height;
};


#endif // !DESPLAYE_H
