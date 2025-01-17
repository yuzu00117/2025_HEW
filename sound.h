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
	NULL_SOUND,
	GAME_BGM,
	TITLE_BGM,
	STAGESELECT_BGM,
	RESULT_BGM,

	Select_Sound,
	Decision_Sound,
	Cancel_Sound,
	Pose_Sound,

	Anchor_Hit_Sound,
	Anchor_Hit_Miss_Sound,
	Anchor_Pulling_Sound,
	Anchor_Thorw_Sound,

	Player_Jump_Sound,
	Player_Soul_Colect_Sound,
	Player_Dameged_Sound,
	Player_Dead_Sound,

	Object_Rock_Fall_Sound,
	Object_Get_Treasure_Sound,
	Object_Wood_Fall_Sound,
	Object_Rock_Roll_Sound,
	Object_Get_Coin_Sound,

	Enemy_Dead_Sound,

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
CriBool app_atomex_stop_cue();

/**************************************************************************
 * 変数定義
 * Variable definitions
 **************************************************************************/

 /* キュー一覧 */
static AppCueListItem g_cue_list[] = {
CRI_CUESHEET_0_1_1BGM,					//1
CRI_CUESHEET_0_タイトルBGM,				//2
CRI_CUESHEET_0_BGM_STAGESELECT,			//3
CRI_CUESHEET_0_リザルトBGM,				//4
CRI_CUESHEET_0_選択音,					//5
CRI_CUESHEET_0_決定音,					//6
CRI_CUESHEET_0_キャンセル音,			//7
CRI_CUESHEET_0_一時停止音_メニュー音_,	//8
CRI_CUESHEET_0_アンカーヒットポイント,	//9
CRI_CUESHEET_0_アンカーポイントミス,	//10
CRI_CUESHEET_0_アンカーを引っ張る音,	//11
CRI_CUESHEET_0_アンカー投げる音,		//12
CRI_CUESHEET_0_ジャンプ,				//13
CRI_CUESHEET_0_ソウル吸収,				//14
CRI_CUESHEET_0_ダメージを受ける音,		//15
CRI_CUESHEET_0_やられた音,				//16
CRI_CUESHEET_0_岩が落ちる音,			//17
CRI_CUESHEET_0_宝を取る音,				//18
CRI_CUESHEET_0_木が倒れる音,			//19
CRI_CUESHEET_0_岩が転がる音,			//20
CRI_CUESHEET_0_コインを取る音,			//21
CRI_CUESHEET_0_敵を倒す音,				//22
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
