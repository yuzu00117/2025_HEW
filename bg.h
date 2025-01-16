//-----------------------------------------------------------------------------------------------------
// #name bg.h
// #description 背景処理
// #make 2024/12/12　　今村友哉
// #update 2024/12/12
// #comment 
//          
//----------------------------------------------------------------------------------------------------

#ifndef BG_H_
#define BG_H_

class Bg
{
public:
	// 引数付きコンストラクタ
	Bg(XMFLOAT2 pos) : m_bg_position(pos) {}

	// 引数なしのデフォルトコンストラクタを追加
	Bg() : m_bg_position(XMFLOAT2(0.0f, 0.0f)) {}

	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	XMFLOAT2 GetPosition()
	{
		return m_bg_position;
	}
	void SetPosition(XMFLOAT2 pos)
	{
		m_bg_position = pos;
	}

private:
	XMFLOAT2 m_bg_position;


	XMFLOAT2 texture_1_pos[4];
	XMFLOAT2 texture_2_pos[4];
	XMFLOAT2 texture_3_pos[4];
	XMFLOAT2 texture_4_pos[4];
	XMFLOAT2 texture_5_pos[4];
};

#endif
