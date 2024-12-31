// ������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// 
// [ Easing.cpp ]
// 
// �����: �i��`��
// ���t:  2024/06/26
// 
// [��] = �V�[���W�����v
// [��] = �֐����ɃW�����v
// 
// ������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������



// ������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �v���v���Z�b�T���� [��]

#pragma once

#ifndef _Easing_H
#define _Easing_H

// ������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �\���� [��]


/**
 * @brief �C�[�W���O
 */
class Ease
{

	/**
	* @brief �C�[�W���O�̎��
	*/
	enum class Type
	{
		None,
		Back,
		Bounce,
		Cubic,
		Circle,
		Elastic,
		Quint
	};

	/**
	 * @brief �C�[�W���O�̃C���A�E�g
	 */
	enum class IO
	{
		In,
		Out,
		InOut,
		OutIn
	};

public:

	/**
	 * @brief ���R�ȃC�[�W���O���w�肷��
	 * @param x - �l�i0~1�j
	 * @param io - �C�����A�E�g��
	 * @param ease - �C�[�W���O�^�C�v
	 * @return �C�[�Y�����������l
	 */
	static double Easing(const double& x, const Ease::IO& io, const Ease::Type& ease);

	static double InBounce(const double&);// �e���C�[�Y�C��
	static double InCubic(const double&);// ��`�C�[�Y�C��
	static double InCirc(const double&);// �ی`�C�[�Y�C��
	static double InElastic(const double&);// �����C�[�Y�C��
	static double InQuint(const double&);// �N�C���g�C�[�Y�C��
	static double InBack(const double&);// �o�b�N�C�[�Y�C��

	static double OutBounce(const double&);// �e���C�[�Y�A�E�g
	static double OutCubic(const double&);// ��`�C�[�Y�A�E�g
	static double OutCirc(const double&);// �ی`�C�[�Y�A�E�g
	static double OutElastic(const double&);// �����C�[�Y�A�E�g
	static double OutQuint(const double&);// �N�C���g�C�[�Y�A�E�g
	static double OutBack(const double&);// �o�b�N�C�[�Y�A�E�g

	static double InOutBounce(const double&);
	static double InOutCubic(const double&);// ��`�C�[�Y�C���C�[�Y�A�E�g
	static double InOutCirc(const double&);// �ی`�C�[�Y�C���C�[�Y�A�E�g
	static double InOutElastic(const double&);// �����C�[�Y�C���C�[�Y�A�E�g
	static double InOutQuint(const double&);// �N�C���g�C�[�Y�C���C�[�Y�A�E�g
	static double InOutBack(const double&);// �o�b�N�C�[�Y�C���C�[�Y�A�E�g

	static double OutInQuint(const double&);// �N�C���g�C�[�Y�A�E�g�C�[�Y�C��

	static double easeInOutElastic(const double& x); //�C�[�Y�C���A�E�g�G���X�e�B�b�N[��]

	static double easeOutElastic(const double& x); //�C�[�Y�A�E�g�G���X�e�B�b�N[��]

	static double easeOutBack(const double& x);// �C�[�Y�A�E�g�o�b�N [��]

	static double easeInElastic(const double& x);// �C�[�Y�C���G���X�e�B�b�N [��]

};

#endif // !_Easing_H