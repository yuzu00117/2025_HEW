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
#include"player_stamina.h"
#include"anchor_spirit.h"


#define	MAX_WORD	(100)
#define NUMBER_PARAMETER	(3)

enum PARAMETER
{
	PARAMETER_PLAYER_POSITION = 0,
	PARAMETER_PLAYER_STAMINA,
	PARAMETER_SPIRIT_GAUGE,
};



//文字列を画像に変換する際の情報
WORD_INFORM g_word[NUMBER_PARAMETER][MAX_WORD];


//デバック用パラメータ一覧を開く制御		
bool	g_pressed ;
//デバッグ用パラメータ一覧
bool	g_open = true;

//プレイヤーの位置情報
b2Vec2	g_player_position = { 0.0f,0.0f };


//体力値
float	g_stamina;
//ソウル値
float	g_spirit;




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
				SetWord(words, g_word[i], MAX_WORD);
				break;				
				//プレイヤーの体力
			case PARAMETER_PLAYER_STAMINA:
				//プレイヤーの位置情報
				float stamina;
				stamina = PlayerStamina::GetPlayerStaminaValue();
				//位置情報を文字列に変換
				words = ToString(L"プ レ イ ヤ ー Stamina : ", stamina);
				//文字列を画像に変換
				SetWord(words, g_word[i], MAX_WORD);
				break;		
				//ソウルゲージ
			case PARAMETER_SPIRIT_GAUGE:
				//プレイヤーの位置情報
				float spirit = AnchorSpirit::GetAnchorSpiritValue();
				//位置情報を文字列に変換
				words = ToString(L"ソ ー ル ゲ ー ジ : ", spirit);
				//文字列を画像に変換
				SetWord(words, g_word[i], MAX_WORD);
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

		float distance_y = 0.3f;

		for (int i = 0; i < NUMBER_PARAMETER; i++)
		{
			switch (i)
			{
			case PARAMETER_PLAYER_POSITION:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[i]);
				break;			
			case PARAMETER_PLAYER_STAMINA:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[i]);
				break;			
			case PARAMETER_SPIRIT_GAUGE:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[i]);
				break;
			}
		}




	}
}
