//-----------------------------------------------------------------------------------------------------
// #name word.cpp
// #description 文字の画像
// #make 2024/11/9		王泳心
// #update 2024/11/12
// #comment 追加・修正予定
//          ・まだない
//          ・
// 
//          
//----------------------------------------------------------------------------------------------------

#include "word.h"
#include"sprite.h"
#include"texture.h"
#include"main.h"
#include"renderer.h"
#include"texture.h"

#define	MAX_WORD_LIST_NUMBER	(100)




//グローバル変数
static ID3D11ShaderResourceView* g_Texture;



//wchar_t g_hiragana_list[] = L"ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん";
//wchar_t g_katakana_list[] = L"ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ";
//wchar_t g_alphahead_big_list[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//wchar_t g_alphahead_small_list[] = L"abcdefghijklmnopqrstuvwxyz";
const wchar_t g_symbol_list[] = L"　 ー-。.、,：:（）()？?＜＞<>";

enum LIST
{
	LIST_NUMBER = 0,
	LIST_ALPHA_HEAD_BIG = 10,
	LIST_ALPHA_HEAD_SMALL = 36,
	LIST_HIRAGANA = 62,
	LIST_KATAKANA = 145,
	LIST_SYMBOL = 231,

};



void	InitializeWord()
{
	setlocale(LC_ALL, "japanese"); // 日本語のロケールを設定
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\moji_list.png");

}

void	FinalizeWord()
{
	//	テクスチャのメモリ解放
	if (g_Texture != nullptr) {

		UnInitTexture(g_Texture);
	}

}



void	DrawWord(XMFLOAT2 position, XMFLOAT2 scales, float rotate, WORD_INFORM* word)
{


	for (int i = 0; i < word->length; i++, word++)
	{

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
		DrawDividedSprite(position, rotate, scales, 13, 20, word->index);

		position.x += scales.x / 2;
	}
}


void	SetWord(std::wstring words, WORD_INFORM* word_inform, int max_word)
{
	//文字列の文字数を取得
	int	length = words.length();

	//渡された文字画像情報のインデクスを更新（一文字ずつ調べる）
	for (int i = 0; i < length && i < max_word; i++, word_inform++)
	{
		//文字が数字の場合
		if ((int)words[i] >= (int)L'0' && (int)words[i] <= (int)L'9')
		{
			word_inform->index = (int)words[i] - (int)L'0' + LIST_NUMBER;
			word_inform->type = LIST_NUMBER;
		}
		//大文字アルファベット
		else if ((int)words[i] >= (int)L'A' &&
			(int)words[i] <= (int)L'Z')
		{
			word_inform->index = (int)words[i] - (int)L'A' + LIST_ALPHA_HEAD_BIG;
			word_inform->type = LIST_ALPHA_HEAD_BIG;
		}
		//小文字アルファベット
		else if ((int)words[i] >= (int)L'a' &&
			(int)words[i] <= (int)L'z')
		{
			word_inform->index = (int)words[i] - (int)L'a' + LIST_ALPHA_HEAD_SMALL;
			word_inform->type = LIST_ALPHA_HEAD_SMALL;
		}
		//ひらがな
		else if ((int)words[i] >= (int)L'ぁ' &&
			(int)words[i] <= (int)L'ん')
		{
			word_inform->index = (int)words[i] - (int)L'ぁ' + LIST_HIRAGANA;
			word_inform->type = LIST_HIRAGANA;

		}
		//カタカナ
		else if ((int)words[i] >= (int)L'ァ' &&
			(int)words[i] <= (int)L'ヶ')
		{
			word_inform->index = (int)words[i] - (int)L'ァ' + LIST_KATAKANA;
			word_inform->type = LIST_KATAKANA;
		}
		//符号
		else
		{
			word_inform->index = LIST_SYMBOL;

			for (int j = 0; j < sizeof(g_symbol_list) / sizeof(g_symbol_list[0]); j++)
			{
				if (words[i] == g_symbol_list[j])
				{
					word_inform->index = j + LIST_SYMBOL;
				}

			}
			word_inform->type = LIST_SYMBOL;
		}

		word_inform->length = length;
	}

	//使われていないところのインデクスを消す
	for (int i = length + 1; i < max_word; i++)
	{
		word_inform->index = LIST_SYMBOL;
	}

	return;

}





