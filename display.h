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

#define DEFAULT_DISPLAY_WIDTH (650)
#define DEFAULT_DISPLAY_HEIGHT (540)

#define DISPLAY_RANGE_TO_SCALE (0.6)

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

	static void SetDisplayWidth(float width);

	//�}�b�v�̏c
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
