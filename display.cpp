//-----------------------------------------------------------------------------------------------------
// #name display.cpp
// #description ��ʂ̃X�P�[���̂�ύX�ł���@�ƌ����Ă��@���̒��ł���Ƃ������́@�X�v���C�g�̒��ł��Ă�
// #make 2024/12/12�@�@�i��`��
// #update 2024/12/12
// #comment �ǉ��E�C���\��
//          �E���ɂȂ�
//----------------------------------------------------------------------------------------------------

#include"display.h"
#include"keyboard.h"
#include"anchor_spirit.h"

// �ÓI�����o�ϐ��̒�`�i1�񂾂��s���j

float display::m_display_scale = 1;//�X�P�[���̏����l�̔{��

float display::m_display_width = 650;//����
float display::m_display_height =300;//�c


display::display()
{
}

display::~display()
{
}

void display::Update()
{
	//�f�o�b�N�p

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
	//�A���J�[�̃��x���ɉ������ύX


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

//�X�P�[�����O
float display::GetDisplayScale()
{
	return m_display_scale;
}
void display::SetDisplayScale(float scale)
{
	m_display_scale = m_display_scale + scale;
}

//�}�b�v�̉�
float display::GetDisplayWidth()
{
	return m_display_width;
}
void display::SetDisplayWidth(float width)
{
	m_display_width = m_display_width + width;
}

//�}�b�v�̏c
float display::GetDisplayHeight()
{
	return m_display_height;
}
void display::SetDisplayHeight(float height)
{
	m_display_height = m_display_height + height;
}