//-----------------------------------------------------------------------------------------------------
// #name word.h
// #description 文字の画像
// #make 2024/11/9		王泳心
// #update 2024/11/12
// #comment 追加・修正予定
//          ・関数　SecondToString　は使わないでください！！！！！！！
//          ・
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

//文字列を画像に変換するための関数（words： 文字列の前にLを付ける;;;  word_inform：自分で用意;;;  max_word：word_informのアドレス先の要素数）
void	SetWord(std::wstring words, WORD_INFORM* word_inform, int max_word);

/////////////////////////////////////////下使っていい関数はToString関数だけ、他は触るなぁぁぁぁぁぁぁぁ！////////////////////////////////////////////////////////////////////////////////


//数値を文字列に変換するための関数の第3工程（第2工程でオーバーロードで呼ばれる）
inline void SecondToString(std::wstringstream& word)
{
}
//数値を文字列に変換するための関数の第2工程
template<typename Previous, typename... Latter>
void SecondToString(std::wstringstream& word, const Previous& previous, const Latter&... latter)
{
	// float型かdouble型の場合
	if (typeid(previous).name() == typeid(float).name() || typeid(previous).name() == typeid(double).name())
	{
		//小数点後2桁まで
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

//数値を文字列に変換するための関数
template<typename... Args>
std::wstring ToString(const Args&... args)
{
	std::wstringstream word;
	SecondToString(word, args...);
	return word.str();
}









#endif // !WORD_H