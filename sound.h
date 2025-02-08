//-----------------------------------------------------------------------------------------------------
// #name sound.h
// #description sound
// #make 2024/12/03　　今村友哉
// #update 2024/12/03
// #comment 
//          
//----------------------------------------------------------------------------------------------------
#ifndef SOUND_H
#define SOUND_H


 /**************************************************************************
  * インクルード
  * Header files
  **************************************************************************/
#include <cri_adx2le.h>
#include <CriSmpFramework.h>

  /* インゲームプレビューを有効にする場合、定義する */
#define USE_INGAME_PREVIEW

/**************************************************************************
 * 定数マクロ定義
 * Constant macro definitions
 **************************************************************************/
 /* ACF/ACBのマクロ定義ヘッダ */
#include "../../cridata/Public/CueSheet_0.h"
#include "../../cridata/Public/HEW_sound_acf.h"

/* データディレクトリへのパス */
#define PATH	"./cri/cridata/Public/"

/* サンプルで使用するファイル名 */
#define ACF_FILE			"HEW_sound.acf"
#define ACB_FILE			"CueSheet_0.acb"
//#define AWB_FILE			"Basic.awb"

/* 最大ボイス数を増やすための関連パラメータ */
#define MAX_VOICE			(24)
#define MAX_VIRTUAL_VOICE	(64)		/* ざっくり多め(通常ボイス＋HCA-MXボイス＋α) */
#define MAX_CRIFS_LOADER	(64)		/* ざっくり多め(通常ボイス＋HCA-MXボイス＋α) */

/* 最大サンプリングレート（ピッチ変更含む） */
#define MAX_SAMPLING_RATE	(48000*2)

/* HCA-MXコーデックのサンプリングレート */
#define SAMPLINGRATE_HCAMX		(32000)


#define X_POS_OFFSET_FOR_MAP_DRAW	(20)
#define PITCH_CHANGE_VALUE			(-200.0f)

/**************************************************************************
 * 型定義
 * Type definitions
 **************************************************************************/

 /* キューリストアイテム */
typedef struct AppCueListItemTag {
	CriAtomExCueId id;
} AppCueListItem;

/* アプリケーション構造体 */
typedef struct AppTag {
	CriAtomExPlayerHn		player;		/* 再生プレーヤ */
	CriAtomExVoicePoolHn	standard_voice_pool;	/* ボイスプール(ADX/HCAコーデック用) */
	CriAtomExVoicePoolHn	hcamx_voice_pool;		/* ボイスプール(HCA-MX用) */
	CriAtomExAcbHn			acb_hn;		/* ACBハンドル(音声データ) */
	CriAtomDbasId			dbas_id;	/* D-BASの作成*/

	CriAtomExPlaybackId		playback_id;	/* VoiceキューのプレイバックID(再生開始時に保持する) */

	CriSint32 ui_cue_idnex;				/* ユーザ選択中のキュー   */

} AppObj;


enum Sound_Manager
{
	GAME_BGM,
	TITLE_BGM,
	STAGESELECT_BGM,
	RESULT_BGM,
	BOSS1_BGM,
	STAGE1_BGM,
	POP_BGM,
	ADVENTURE_BGM,
	STAGE2_BGM,
	BOSS2_BGM,

	Select_Sound,
	Decision_Sound,
	Cancel_Sound,
	Pose_Sound,
	Result_Sound,

	Anchor_Hit_Sound,//なんやこれ
	Anchor_Hit_Miss_Sound,//なし
	Anchor_Pulling_Sound,//player.cpp connected
	Anchor_Thorw_Sound,//player.cpp crate_state
	Anchor_Mark_Sound, //anchor_point.cpp

	Player_Soul_Colect1_Sound,//playercpp update
	Player_Damege_Sound, //player.cpp Player_Damaged
	Player_Dead_Sound,//player_Stamina.cpp  EditPlayerStaminaValue
	Player_Coin_Colect_Sound,//Item_coni Update
	Player_Walk_Sound,// player.cpp Update
	Player_Jump_Start_Sound,//player.cpp update かえたい
	Player_Jump_End_Sound,//実装なし
	Player_Attack_Sound,//player.cpp update CreateNormalAttack_State
	Player_Jewelry_Colect_Sound,//Item_Jewl.cpp update
	Player_GameOver_Sound,//ゲームおーば完成後に導入したい
	Player_Soul_Colect2_Sound,//contactliast.h   // プレーヤーとアイテムが衝突したかを判定
	Player_Fall_Sound,//実装なし
	Player_Warp_Sound,//実装なし
	Player_Frame_Up_Sound,//player.cpp DrawAnchorLevel3Frame

	Object_Rock_Fall_Sound,
	Object_Get_Treasure_Sound,
	Object_Wood_Fall_Sound,
	Object_Rock_Roll_Sound,
	Object_Geyser_Sound,
	Object_Pillar_Break_Sound,
	Object_Pillar_Hit_Sound,

	Boss_Charge_Attack_Sound,//update charge_attack_state
	Boss_Jump_Sound,//update jumpstate
	Boss_Attack_Wave_Sound,//update shock_wave_state
	Boss_Walk_Sound,//update walk_state
	Boss_Charge_Sound,//update charge_attack_state
	Boss_Core_Damege_Sound, //DestroyBossCore
	Boss_Damege_Sound,//boss.cpp CreateBossCore
	Boss_Stun_Sound,//Update　down_state

	Enemy_Knock_Down1_Sound,
	Enemy_MiniGolem_Create_Sound,//boss.cpp  CreateMiniGolem
	Enemy_MiniGolem_Explosion_Sound,//boss.cpp  DestroyMiniGolemBody
	Enemy_Attack_Sound,
	Enemy_Knock_Down2_Sound,
	Enemy_Shot_Down_Sound

};
/***************************************************************************
 * 関数宣言
 * Function declarations
 ***************************************************************************/
static void* user_alloc_func(void* obj, CriUint32 size);
static void user_free_func(void* obj, void* ptr);

static CriBool app_atomex_initialize(AppObj* app_obj);
static CriBool app_atomex_finalize(AppObj* app_obj);
static CriBool app_execute_main(AppObj* app_obj);

CriBool app_atomex_start(Sound_Manager sound_name);
CriBool app_atomex_stop_player();
CriBool app_atomex_stop_cue(Sound_Manager sound_name);

/**************************************************************************
 * 変数定義
 * Variable definitions
 **************************************************************************/

 /* キュー一覧 */
static AppCueListItem g_cue_list[] = {
	CRI_CUESHEET_0_1_1BGM,
	CRI_CUESHEET_0_タイトルBGM,
	CRI_CUESHEET_0_BGM_STAGESELECT,
	CRI_CUESHEET_0_リザルトBGM,
	CRI_CUESHEET_0_BGM_ボス,
	CRI_CUESHEET_0_BGM_ジャングル,
	CRI_CUESHEET_0_BGM_ポップ,
	CRI_CUESHEET_0_BGM_探検,
	CRI_CUESHEET_0_BGM_ジャングル2,
	CRI_CUESHEET_0_BGM_ボス2,
	CRI_CUESHEET_0_選択音,
	CRI_CUESHEET_0_決定音,
	CRI_CUESHEET_0_キャンセル音,
	CRI_CUESHEET_0_一時停止音_メニュー音_,
	CRI_CUESHEET_0_リザルト発表,
	CRI_CUESHEET_0_アンカーヒットポイント,
	CRI_CUESHEET_0_アンカーポイントミス,
	CRI_CUESHEET_0_アンカーを引っ張る音,
	CRI_CUESHEET_0_アンカー投げる音,
	CRI_CUESHEET_0_アンカーポイントマークした時の音,
	CRI_CUESHEET_0_ソウル吸収1,
	CRI_CUESHEET_0_ダメージを受ける音,
	CRI_CUESHEET_0_やられた音,
	CRI_CUESHEET_0_コイン取得音,
	CRI_CUESHEET_0_プレイヤーの足音,
	CRI_CUESHEET_0_ジャンプ,
	CRI_CUESHEET_0_着地音,
	CRI_CUESHEET_0_打撃音,
	CRI_CUESHEET_0_宝石取得,
	CRI_CUESHEET_0_死亡音,
	CRI_CUESHEET_0_ソウル吸収2,
	CRI_CUESHEET_0_落下,
	CRI_CUESHEET_0_ワープ音,
	CRI_CUESHEET_0_フレームアップ,
	CRI_CUESHEET_0_岩が落ちる音,
	CRI_CUESHEET_0_宝を取る音,
	CRI_CUESHEET_0_木が倒れる音,
	CRI_CUESHEET_0_岩が転がる音,
	CRI_CUESHEET_0_間欠泉,
	CRI_CUESHEET_0_柱倒してバラバラ音,
	CRI_CUESHEET_0_柱とボスがぶつかる音,
	CRI_CUESHEET_0_ボス溜め攻撃衝撃時,
	CRI_CUESHEET_0_ボスジャンプ着地音,
	CRI_CUESHEET_0_ボス衝撃波発生中,
	CRI_CUESHEET_0_ボス足音,
	CRI_CUESHEET_0_ボス貯め攻撃,
	CRI_CUESHEET_0_コアダメージ時,
	CRI_CUESHEET_0_ボスに対するダメージ,
	CRI_CUESHEET_0_ボススタン音,
	CRI_CUESHEET_0_敵を倒す音,
	CRI_CUESHEET_0_ちびゴーレム生成音,
	CRI_CUESHEET_0_ちびゴーレム爆発音,
	CRI_CUESHEET_0_エネミー攻撃,
	CRI_CUESHEET_0_エネミーダウン,
	CRI_CUESHEET_0_エネミー撃墜音
};





static CriUint32 g_num_cue_items = sizeof(g_cue_list) / sizeof(AppCueListItem);


/***************************************************************************
 * 関数定義
 * Function definitions
 ***************************************************************************/

void CRIInitialize();
void CRIUpdate();
void CRIFinalize();

#endif // SOUND_H
