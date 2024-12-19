//-----------------------------------------------------------------------------------------------------
// #name bg.h
// #description îwåièàóù
// #make 2024/12/12Å@Å@ç°ë∫óFç∆
// #update 2024/12/12
// #comment 
//          
//----------------------------------------------------------------------------------------------------

#ifndef BG_H_
#define BG_H_

class Bg
{
public:
	Bg(XMFLOAT2 pos) :m_bg_position(pos) {}

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
};

#endif
