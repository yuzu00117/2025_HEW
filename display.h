//-----------------------------------------------------------------------------------------------------
// #name display.h
// #description ��ʂ̃X�P�[���̂�ύX�ł���@�ƌ����Ă��@���̒��ł���Ƃ������́@�X�v���C�g�̒��ł��Ă�
// #make 2024/12/12�@�@�i��`��
// #update 2024/12/12
// #comment �ǉ��E�C���\��
//          �E���ɂȂ�
//----------------------------------------------------------------------------------------------------

#ifndef DESPLAYE_H
#define DESPLAYE_H

class display
{
public:
	display();
	~display();

	
	static void Update();

	//�X�P�[�����O
	static float GetDisplayScale();
	
	static void SetDisplayScale(float scale);


	//�}�b�v�̉�
	static float GetDisplayWidth();

	static void AddDisplayWidth(float width);

	static void SetDisplayWidth(float width);

	//�}�b�v�̏c
	static float GetDisplayHeight();

	static void AddDisplayHeight(float height);

	static void SetDisplayHeight(float height);

private:
	static float m_display_scale;
	static float m_display_width;
	static float m_display_height;
};


#endif // !DESPLAYE_H
