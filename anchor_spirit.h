//-----------------------------------------------------------------------------------------------------
// #name anchor_spirit.h
// #description �A���J�[�̃\�E���Q�[�W�̒l
// #make 2024/11/20			���j�S
// #update 2024/11/20
// #comment �ǉ��E�C���\��
//
//           �E�A���J�[�ǉ����ꂽ��A�\�E���Q�[�W�̒l�����l�ɂȂ����Ƃ��̏�����ǉ�����
// 
//----------------------------------------------------------------------------------------------------

#pragma once


#ifndef ANCHOR_SPIRIT_H
#define ANCHOR_SPIRIT_H

#define		MAX_ANCHOR_SPIRIT (300.0)

class AnchorSpirit
{
public:

	static void	Initialize();

	static	void Update();

	//�������񕜒����擾
	static	bool	GetIfAutoHeal() { return m_auto_heal; }
	//�������񕜒����Z�b�g
	static	void	SetIfAutoHeal(bool flag) { m_auto_heal = flag; }

	//���̃\�E���Q�[�W�̒l�����炤
	static float	GetAnchorSpiritValue();

	//���̃\�E���Q�[�W�̒l���}�C�i�X�A�������̓v���X�i�}�C�i�X�������ꍇ�͕�����n���ĂˁI�j
	static void	EditAnchorSpiritValue(float value);

	//���̃\�E���Q�[�W�̒l�𒼐ڕύX
	static void	SetAnchorSpiritValueDirectly(float value);


	static int GetAnchorLevel()
	{
		int anchor_level;//�A���J�[�̃��x��
		int spirit = GetAnchorSpiritValue();

		if (spirit < 101) {
			anchor_level = 1;
		}
		else if (spirit < 201)
		{
			anchor_level = 2;
		}
		else
		{
			anchor_level = 3;
		}

		return anchor_level;
	}

private:
	AnchorSpirit(){}
	~AnchorSpirit(){}

	//�\�[���Q�[�W�i��ŃA���J�[�Ɉړ����邩���j
	static float	m_spirit;
	//�\�[�����ꂩ�猸��l
	static float	m_damage;

	//�����񕜂̃t���O
	static bool		m_auto_heal;
};


#endif // !ANCHOR_SPIRIT_H

