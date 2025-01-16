//-----------------------------------------------------------------------------------------------------
// #name bg.h
// #description �w�i����
// #make 2024/12/12�@�@�����F��
// #update 2024/12/12
// #comment 
//          
//----------------------------------------------------------------------------------------------------

#ifndef BG_H_
#define BG_H_

class Bg
{
public:
	// �����t���R���X�g���N�^
	Bg(XMFLOAT2 pos) : m_bg_position(pos) {}

	// �����Ȃ��̃f�t�H���g�R���X�g���N�^��ǉ�
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
