//-----------------------------------------------------------------------------------------------------
// #name debug.cpp
// #description デバック用のパラメータ一覧
// #make 2024/11/9		王泳心
// #update 2024/11/20
// #comment 追加・修正予定
//          ・アンカーのポジション
//          ・アンカーの経験値
// 			・ アンカーのレベル
//          ・ アンカーの状態
//----------------------------------------------------------------------------------------------------

#include "debug.h"
#include"player_position.h"
#include"keyboard.h"
#include"sprite.h"
#include"renderer.h"
#include"word.h"
#include"main.h"
#include<iostream>
#include"player.h"
#include"anchor_spirit.h"
#include"player_stamina.h"


#define	MAX_WORD	(100)
#define NUMBER_PARAMETER	(6)

enum PARAMETER
{
	PARAMETER_PLAYER_POSITION = 0,
	PARAMETER_CAN_JUMP,
	PARAMETER_BEGINCONTACT,
	PARAMETER_ENDCONTACT,
	PARAMETER_STAMINA,
	PARAMETER_SPIRIT,

};



//文字列を画像に変換する際の情報
WORD_INFORM g_word[NUMBER_PARAMETER][MAX_WORD];


//デバック用パラメータ一覧を開く制御		
bool	g_pressed ;
//デバッグ用パラメータ一覧
bool	g_open;

//プレイヤーの位置情報
b2Vec2	g_player_position = { 0.0f,0.0f };

//プレイヤージャンプ可能かどうか
bool	g_can_jump;

//体力値
float	g_stamina;
//ソウル値
float	g_spirit;


//コンタクトリストの関数呼ばれているかどうか
bool	g_BeginContact;
//コンタクトリストの関数呼ばれているかどうか
bool	g_EndContact;

void SetBeginContact(bool a)
{
	g_BeginContact = a;
}

void SetEndContact(bool a)
{
	g_EndContact = a;
}

void	InitializeDebug()
{
	
}


void	FinalizeDebug()
{
	
}


void	UpdateDebug()
{
	//ボタンｘ
	if (Keyboard_IsKeyDown(KK_X) && !g_pressed)
	{
		g_open = !g_open;	//開くか閉じるかスイッチ

	}
	g_pressed = Keyboard_IsKeyDown(KK_X);	//トリガー

	//もしデバッグ用パラメータ一覧を開いている状態なら
	if (g_open)
	{
		PlayerPosition	player;
		Player player_collider = Player::GetInstance();


		std::wstring words;
		
		for (int i = 0; i < NUMBER_PARAMETER; i++)
		{
			switch (i)
			{
				//プレイヤー位置情報
			case PARAMETER_PLAYER_POSITION:
				//プレイヤーの位置情報
				g_player_position = player.GetPlayerPosition();
				//位置情報を文字列に変換
				words = ToString(L"プ レ イ ヤ ー Position : ", g_player_position.x, L" , ", g_player_position.y);
				//文字列を画像に変換
				SetWord(words, g_word[PARAMETER_PLAYER_POSITION], MAX_WORD);
				break;
				//プレイヤーがジャンプ可能かどうか
			case PARAMETER_CAN_JUMP:
				g_can_jump = player_collider.GetJumpFlag();
				words = ToString(L"Can ジ ャ ン プ : ", g_can_jump);
				SetWord(words, g_word[PARAMETER_CAN_JUMP], MAX_WORD);
				break;
			case PARAMETER_BEGINCONTACT:
				words = ToString(L"BeginContact : ", g_BeginContact);
				SetWord(words, g_word[PARAMETER_BEGINCONTACT], MAX_WORD);
				break;
			case PARAMETER_ENDCONTACT:
				words = ToString(L"EndContact : ", g_EndContact);
				SetWord(words, g_word[PARAMETER_ENDCONTACT], MAX_WORD);
				break;
			case PARAMETER_STAMINA:
				g_stamina = PlayerStamina::GetPlayerStaminaValue();
				words = ToString(L"ス タ ミ ナ ゲ ー ジ : ", g_stamina);
				SetWord(words, g_word[PARAMETER_STAMINA], MAX_WORD);
				break;
			case PARAMETER_SPIRIT:
				g_spirit = AnchorSpirit::GetAnchorSpiritValue();
				words = ToString(L"ソ ウ ル ゲ ー ジ : ", g_spirit);
				SetWord(words, g_word[PARAMETER_SPIRIT], MAX_WORD);
				break;
			}
		}
		


	}
}


void	DrawDebug()
{
	if (g_open)
	{
		//画面右上の位置
		XMFLOAT2 screen_rightup = { 16.0f, 0.4f };
		// コライダーと位置情報の補正をするため
		float scale = 80.0f;

		//表示する場所を設定
		XMFLOAT2 first_word_position;
		//表示するサイズを設定
		XMFLOAT2 word_scales = { 18.0f,33.0f };
		//表示する回転角度を設定
		float word_rotate = 0.0f;


		for (int i = 0; i < NUMBER_PARAMETER; i++)
		{
			switch (i)
			{
			case PARAMETER_PLAYER_POSITION:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_PLAYER_POSITION]);
				break;
			case PARAMETER_CAN_JUMP:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 0.3f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_CAN_JUMP]);
				break;
			case PARAMETER_BEGINCONTACT:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 0.6f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_BEGINCONTACT]);
				break;
			case PARAMETER_ENDCONTACT:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 0.9f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_ENDCONTACT]);
				break;
			case PARAMETER_STAMINA:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 1.2f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_STAMINA]);
				break;
			case PARAMETER_SPIRIT:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 1.5f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_SPIRIT]);
				break;

			}
		}




	}
}
