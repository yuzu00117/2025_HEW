//-----------------------------------------------------------------------------------------------------
// #name word.h
// #description �����̉摜
// #make 2024/11/9		���j�S
// #update 2024/11/12
// #comment �ǉ��E�C���\��
//          �E�֐��@SecondToString�@�͎g��Ȃ��ł��������I�I�I�I�I�I�I
//          �E
// 
//          
//----------------------------------------------------------------------------------------------------


#ifndef WORD_H
#define	WORD_H

#include	"main.h"
#include<iostream>
#include<sstream>
#include<iomanip>


struct WORD_INFORM
{
	int	index = -1;
	int type = -1;
	int	length;
};


void	FinalizeWord();
void	InitializeWord();
void	DrawWord(XMFLOAT2 position, XMFLOAT2 scales, float rotate, WORD_INFORM* word);

//��������摜�ɕϊ����邽�߂̊֐��iwords�F ������̑O��L��t����;;;  word_inform�F�����ŗp��;;;  max_word�Fword_inform�̃A�h���X��̗v�f���j
void	SetWord(std::wstring words, WORD_INFORM* word_inform, int max_word);

/////////////////////////////////////////���g���Ă����֐���ToString�֐������A���͐G��Ȃ����������������I////////////////////////////////////////////////////////////////////////////////


//���l�𕶎���ɕϊ����邽�߂̊֐��̑�3�H���i��2�H���ŃI�[�o�[���[�h�ŌĂ΂��j
inline void SecondToString(std::wstringstream& word)
{
}
//���l�𕶎���ɕϊ����邽�߂̊֐��̑�2�H��
template<typename Previous, typename... Latter>
void SecondToString(std::wstringstream& word, const Previous& previous, const Latter&... latter)
{
	// float�^��double�^�̏ꍇ
	if (typeid(previous).name() == typeid(float).name() || typeid(previous).name() == typeid(double).name())
	{
		//�����_��2���܂�
		word << std::fixed << std::setprecision(2) << previous;
	}
	else if (typeid(previous).name() == typeid(bool).name())
	{
		if (previous == 0)
		{
			word << L"false";
		}
		else
		{
			word << L"true";
		}
	}
	else
	{
		word << previous;

	}
	SecondToString(word, latter...);
}

//���l�𕶎���ɕϊ����邽�߂̊֐�
template<typename... Args>
std::wstring ToString(const Args&... args)
{
	std::wstringstream word;
	SecondToString(word, args...);
	return word.str();
}









#endif // !WORD_H