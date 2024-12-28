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

#include"easing.h"

#include <math.h>
#include <DirectXMath.h>
using namespace DirectX;

// ������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �֐� [��]



double Ease::Easing(const double& x, const Ease::IO& io, const Ease::Type& ease)
{
	// �C�[�Y�C��
	if (io == Ease::IO::In)
		switch (ease)
		{
		case Ease::Type::Back:		return InBack(x);		break;
		case Ease::Type::Bounce:	return InBounce(x);		break;
		case Ease::Type::Cubic:		return InCubic(x);		break;
		case Ease::Type::Circle:	return InCirc(x);		break;
		case Ease::Type::Elastic:	return InElastic(x);	break;
		case Ease::Type::Quint:		return InQuint(x);		break;
		}

	// �C�[�Y�A�E�g
	else if (io == Ease::IO::Out)
		switch (ease)
		{
		case Ease::Type::Back:		return OutBack(x);		break;
		case Ease::Type::Bounce:	return OutBounce(x);	break;
		case Ease::Type::Cubic:		return OutCubic(x);		break;
		case Ease::Type::Circle:	return OutCirc(x);		break;
		case Ease::Type::Elastic:	return OutElastic(x);	break;
		case Ease::Type::Quint:		return OutQuint(x);		break;
		}

	// �C�[�Y�C���A�E�g
	else if (io == Ease::IO::InOut)
		switch (ease)
		{
		case Ease::Type::Back:		return InOutBack(x);	break;
		case Ease::Type::Bounce:	return InOutBounce(x);	break;
		case Ease::Type::Cubic:		return InOutCubic(x);	break;
		case Ease::Type::Circle:	return InOutCirc(x);	break;
		case Ease::Type::Elastic:	return InOutElastic(x); break;
		case Ease::Type::Quint:		return InOutQuint(x);	break;
		}

	// �C�[�Y�A�E�g�C�[�Y�C��
	else if (io == Ease::IO::OutIn)
		switch (ease)
		{
		case Ease::Type::Quint:		return OutInQuint(x);	break;
		}

	return x;
}





// ==================================================
// �e���C�[�Y�C�� [��]
// ==================================================
double Ease::InBounce(const double& x)
{
	return 1 - Ease::OutBounce(1 - x);
}





// ==================================================
// �e���C�[�Y�A�E�g [��]
// ==================================================
double Ease::OutBounce(const double& x) {
	double n1 = 7.5625;
	double d1 = 2.75;

	if (x < 1.0 / d1)
		return double(n1 * pow(x, 2));

	else if (x < 2.0 / d1)
		return double(n1 * (x - 1.5 / d1) * x + 0.75);

	else if (x < 2.5 / d1)
		return double(n1 * (x - 2.25 / d1) * x + 0.9375);

	else
		return double(n1 * (x - 2.625 / d1) * x + 0.984375);
}





// ==================================================
// �e���C�[�Y�C���A�E�g [��]
// ==================================================
double Ease::InOutBounce(const double& x)
{
	return x < 0.5f
		? (1.0 - Ease::OutBounce(1 - 2.0 * x)) / 2.0
		: (1.0 + Ease::OutBounce(2.0 * x - 1)) / 2.0;
}





// ==================================================
// �����C�[�Y�C�� [��]
// ==================================================
double Ease::InElastic(const double& x)
{
	double c4 = (2.0 * XM_PI) / 3.0;

	return x == 0.0
		? 0.0
		: x == 1.0
		? 1.0
		: -pow(2.0, 10.0 * x - 10.0) * sin((x * 10.0 - 10.75 * c4));
}





// ==================================================
// �����C�[�Y�A�E�g [��]
// ==================================================
double Ease::OutElastic(const double& x)
{
	double n1 = 7.5625;
	double d1 = 2.75;

	if (x < 1 / d1)
		return double(n1 * x * x);

	else if (x < 2 / d1)
		return double(n1 * (x - 1.5 / d1) * x + 0.75);

	else if (x < 2.5 / d1)
		return double(n1 * (x - 2.25 / d1) * x + 0.9375);

	else
		return double(n1 * (x - 2.625 / d1) * x + 0.984375);
}





double Ease::InOutElastic(const double& x)
{
	double c5 = (2.0 * XM_PI) / 4.0;

	return x == 0.0 ? 0.0
		: x == 1.0 ? 1.0
		: x < 0.5f ? -(pow(2, 20.0 * x - 10.0) * sin((20.0 * x - 11.125 * c5)) / 2.0)
		: (pow(2, -20.0 * x + 10.0) * sin((20.0 * x - 11.125 * c5)) / 2.0 + 1.0);
}





// ==================================================
// ��`�C�[�Y�C�� [��]
// ==================================================
double Ease::InCubic(const double& x)
{
	return pow(x, 3);
}





// ==================================================
// ��`�C�[�Y�A�E�g [��]
// ==================================================
double Ease::OutCubic(const double& x)
{
	return double(1.0 - pow(1.0 - x, 3.0));
}





// ==================================================
// ��`�C�[�Y�C���A�E�g [��]
// ==================================================
double Ease::InOutCubic(const double& x)
{
	return x < 0.5 ? 4.0 * pow(x, 3) : 1 - pow(-2.0 * x + 2.0, 3) / 2.0;
}





// ==================================================
// �ی`�C�[�Y�C���C�[�Y�A�E�g [��]
// ==================================================
double Ease::InOutCirc(const double& x)
{
	return double(x < 0.5
		? (1.0 - sqrt(1.0 - pow(2.0 * x, 2.0))) / 2.0
		: (sqrt(1.0 - pow(-2.0 * x + 2, 2.0)) + 1.0) / 2.0);
}





// ==================================================
// �ی`�C�[�Y�A�E�g [��]
// ==================================================
double Ease::OutCirc(const double& x)
{
	return double(sqrt(1.0 - pow(x - 1.0, 2.0)));
}





// ==================================================
// �ی`�C�[�Y�C�� [��]
// ==================================================
double Ease::InCirc(const double& x)
{
	double res = (double)(int)(sqrt(1.0 - pow(x, 2)) * 100.0);
	return double(100.0 - res) / 100.0;
}





// ==================================================
// �C�[�Y�A�E�g�C�[�Y�C���N�C���g [��]
// ==================================================
double Ease::OutInQuint(const double& x)
{
	return double(x < 0.5 ?
		1.0 - pow(1.0 - x * 2.0, 5.0) - 0.5 :
		pow(x * 2.0 - 1.0, 5.0) + 0.5);
}





// ==================================================
// �C�[�Y�C���C�[�Y�A�E�g�N�C���g [��]
// ==================================================
double Ease::InOutQuint(const double& x)
{
	return x < 0.5 ? 8.0 * pow(x, 4) : 1 - pow(-2.0 * x + 2.0, 4) / 2.0;
}





// ==================================================
// �C�[�Y�C���N�C���g [��]
// ==================================================
double Ease::InQuint(const double& x)
{
	return pow(x, 5.0);
}





// ==================================================
// �C�[�Y�A�E�g�N�C���g [��]
// ==================================================
double Ease::OutQuint(const double& x)
{
	return double(1.0 - pow(1.0 - x, 5.0));
}





// ==================================================
// �C�[�Y�A�E�g�o�b�N [��]
// ==================================================
double Ease::OutBack(const double& x)
{
	double c1 = 1.70158;
	double c3 = c1 + 1.0;

	return double(1.0 + c3 * pow(x - 1.0, 3.0) + c1 * pow(x - 1.0, 2.0));
}





// ==================================================
// �C�[�Y�C���o�b�N [��]
// ==================================================
double Ease::InBack(const double& x) {
	double c1 = 1.70158;
	double c3 = c1 + 1.0;

	return double(c3 * x * x * x - c1 * x * x);
}





// ==================================================
// �C�[�Y�A�E�g�o�b�N [��]
// ==================================================
double Ease::InOutBack(const double& x)
{
	double c1 = 1.70158;
	double c2 = c1 * 1.525;

	return x < 0.5
		? (pow(2.0 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2.0
		: (pow(2.0 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2.0;
}



// ==================================================
// �C�[�Y�C���A�E�g�G���X�e�B�b�N [��]
// ==================================================
double Ease::easeInOutElastic(const double& x)
{
	const double c5 = (2.0 * XM_PI) / 4.5;

	if (x == 0.0)
		return 0.0;
	if (x == 1.0)
		return 1.0;

	return x < 0.5
		? -(pow(2.0, 20.0 * x - 10.0) * sin((20.0 * x - 11.125) * c5)) / 2.0
		: (pow(2.0, -20.0 * x + 10.0) * sin((20.0 * x - 11.125) * c5)) / 2.0 + 1.0;
}


// ==================================================
// �C�[�Y�A�E�g�G���X�e�B�b�N [��]
// ==================================================
double Ease::easeOutElastic(const double& x)
{
	const double c4 = (2.0 * XM_PI) / 3.0;

	if (x == 0.0)
		return 0.0;
	if (x == 1.0)
		return 1.0;

	return pow(2.0, -10.0 * x) * sin((x * 10.0 - 0.75) * c4) + 1.0;
}


// ==================================================
// �C�[�Y�A�E�g�o�b�N [��]
// ==================================================
double Ease::easeOutBack(const double& x)
{
	const double c1 = 1.70158;
	const double c3 = c1 + 1.0;

	return 1.0 + c3 * pow(x - 1.0, 3.0) + c1 * pow(x - 1.0, 2.0);
}

// ==================================================
// �C�[�Y�C���G���X�e�B�b�N [��]
// ==================================================
double Ease::easeInElastic(const double& x)
{
	const double c4 = (2.0 * XM_PI) / 3.0;

	if (x == 0.0)
		return 0.0;
	if (x == 1.0)
		return 1.0;

	return -pow(2.0, 10.0 * x - 10.0) * sin((x * 10.0 - 10.75) * c4);
}