//-----------------------------------------------------------------------------------------------------
// #name word.cpp
// #description �����̉摜
// #make 2024/11/9		���j�S
// #update 2024/11/12
// #comment �ǉ��E�C���\��
//          �E�܂��Ȃ�
//          �E
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




//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture;



//wchar_t g_hiragana_list[] = L"�����������������������������������������������������������������������ÂĂłƂǂȂɂʂ˂̂͂΂ςЂт҂ӂԂՂւׂ؂قڂۂ܂݂ނ߂��������������������";
//wchar_t g_katakana_list[] = L"�@�A�B�C�D�E�F�G�H�I�J�K�L�M�N�O�P�Q�R�S�T�U�V�W�X�Y�Z�[�\�]�^�_�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y�z�{�|�}�~����������������������������������������������";
//wchar_t g_alphahead_big_list[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//wchar_t g_alphahead_small_list[] = L"abcdefghijklmnopqrstuvwxyz";
const wchar_t g_symbol_list[] = L"�@ �[-�B.�A,�F:�i�j()�H?����<>";

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
	setlocale(LC_ALL, "japanese"); // ���{��̃��P�[����ݒ�
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\moji_list.png");

}

void	FinalizeWord()
{
	//	�e�N�X�`���̃��������
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
	//������̕��������擾
	int	length = words.length();

	//�n���ꂽ�����摜���̃C���f�N�X���X�V�i�ꕶ�������ׂ�j
	for (int i = 0; i < length && i < max_word; i++, word_inform++)
	{
		//�����������̏ꍇ
		if ((int)words[i] >= (int)L'0' && (int)words[i] <= (int)L'9')
		{
			word_inform->index = (int)words[i] - (int)L'0' + LIST_NUMBER;
			word_inform->type = LIST_NUMBER;
		}
		//�啶���A���t�@�x�b�g
		else if ((int)words[i] >= (int)L'A' &&
			(int)words[i] <= (int)L'Z')
		{
			word_inform->index = (int)words[i] - (int)L'A' + LIST_ALPHA_HEAD_BIG;
			word_inform->type = LIST_ALPHA_HEAD_BIG;
		}
		//�������A���t�@�x�b�g
		else if ((int)words[i] >= (int)L'a' &&
			(int)words[i] <= (int)L'z')
		{
			word_inform->index = (int)words[i] - (int)L'a' + LIST_ALPHA_HEAD_SMALL;
			word_inform->type = LIST_ALPHA_HEAD_SMALL;
		}
		//�Ђ炪��
		else if ((int)words[i] >= (int)L'��' &&
			(int)words[i] <= (int)L'��')
		{
			word_inform->index = (int)words[i] - (int)L'��' + LIST_HIRAGANA;
			word_inform->type = LIST_HIRAGANA;

		}
		//�J�^�J�i
		else if ((int)words[i] >= (int)L'�@' &&
			(int)words[i] <= (int)L'��')
		{
			word_inform->index = (int)words[i] - (int)L'�@' + LIST_KATAKANA;
			word_inform->type = LIST_KATAKANA;
		}
		//����
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

	//�g���Ă��Ȃ��Ƃ���̃C���f�N�X������
	for (int i = length + 1; i < max_word; i++)
	{
		word_inform->index = LIST_SYMBOL;
	}

	return;

}





