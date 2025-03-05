//-----------------------------------------------------------------------------------------------------
// #name 1_1_boss_state_debug.cpp
// #description デバック用のパラメータ一覧
// #make 2025/01/24		永野義也
// #update 2025/01/24
// #comment 追加・修正予定
//----------------------------------------------------------------------------------------------------

#include"1_1boss_state_debug.h"
#include"debug.h"
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
#include"1-1_boss.h"




enum PARAMETER_BOSS
{
	BOSS_POSITION = 0,
	BOSS_HP,
	BOSS_STATE,
	BOSS_PLAYER_NEAR,
	BOSS_ELAPSED_TIME,
};



//文字列を画像に変換する際の情報
WORD_INFORM g_boss_word[NUMBER_PARAMETER][MAX_WORD];


//デバック用パラメータ一覧を開く制御		
bool	g_boss_pressed;
//デバッグ用パラメータ一覧
bool	g_boss_open = true;

//プレイヤーの位置情報
b2Vec2	g_boss_position = { 0.0f,0.0f };


//体力値
float	g_boss_stamina;
//ソウル値
float	g_boss_spirit;




void	InitializeBossDebug()
{
	g_boss_open = 1;
}


void	FinalizeBossDebug()
{

}


void	UpdateBossDebug()
{
	
	Boss_1_1& boss = Boss_1_1::GetInstance();


	//もしデバッグ用パラメータ一覧を開いている状態なら
	if (g_boss_open)
	{
		PlayerPosition	player;
		Player &player_collider = Player::GetInstance();


		std::wstring words;

		for (int i = 0; i < NUMBER_PARAMETER; i++)
		{
			switch (i)
			{
				//プレイヤー位置情報
			case BOSS_POSITION:
				//プレイヤーの位置情報
				g_boss_position = boss.GetBossBody()->GetPosition();
				//位置情報を文字列に変換
				words = ToString(L"Boss Position: ", g_boss_position.x, L" , ", g_boss_position.y);
				//文字列を画像に変換
				SetWord(words, g_boss_word[i], MAX_WORD);
				break;
				//プレイヤーの体力
			case BOSS_HP:
				//プレイヤーの位置情報
				int boss_hp;
				boss_hp = boss.GetBossHP();
				//位置情報を文字列に変換
				words = ToString(L"Boss HP : ", boss_hp);
				//文字列を画像に変換
				SetWord(words, g_boss_word[i], MAX_WORD);
				break;
				//ソウルゲージ
			case BOSS_STATE:
				//プレイヤーの位置情報

				//位置情報を文字列に変換


				switch (boss.GetNowBossState())
				{
				case wait_state:
					words = ToString(L"Boss State:wait_state              ");
					break;
				case panic_state:
					words = ToString(L"Boss State:panic_state             ");

					break;
				case walk_state:
					words = ToString(L"Boss State:walk_state              ");

					break;
				case jump_state:
					words = ToString(L"Boss State:jump_state              ");

					break;
				case charge_attack_state:
					words = ToString(L"Boss State:charge_attack_state     ");

					break;
				case shock_wave_state:
					words = ToString(L"Boss State:shock_wave_state        ");

					break;
				case create_mini_golem_state:
					words = ToString(L"Boss State:create_mini_golem_state ");
					break;

				default:
					words = ToString(L"Boss State:NULL");
					break;
				}

				//文字列を画像に変換
				SetWord(words, g_boss_word[i], MAX_WORD);
				break;

			case BOSS_PLAYER_NEAR://プレイヤーが近くにいるかどうか
			{
				bool near_flag = boss.GetPlayerisNearbyFlag();
				words = ToString(L"Boss_near : ", near_flag);
				//文字列を画像に変換
				SetWord(words, g_boss_word[i], MAX_WORD);
			}
			break;

			case BOSS_ELAPSED_TIME:
				// ボスの経過時間を取得
				float boss_elapsed_time = boss.GetBossElapsedTime();
				// 経過時間を文字列に変換
				words = ToString(L"Boss Elapsed Time: ", boss_elapsed_time);
				// 文字列を画像に変換
				SetWord(words, g_boss_word[i], MAX_WORD);
				break;
			}
		}



	}
}


void	DrawBossDebug()
{
	if (g_boss_open)
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
			case BOSS_POSITION:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			case BOSS_HP:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			case BOSS_STATE:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			case BOSS_PLAYER_NEAR:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			case BOSS_ELAPSED_TIME:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			}
		}




	}
}
