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
#include "../../cridata/Public/Test_acf.h"

/* データディレクトリへのパス */
#define PATH	"./cridata/Public/"

/* サンプルで使用するファイル名 */
#define ACF_FILE			"Test.acf"
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



/***************************************************************************
 * 関数宣言
 * Function declarations
 ***************************************************************************/
static void* user_alloc_func(void* obj, CriUint32 size);
static void user_free_func(void* obj, void* ptr);

static CriBool app_atomex_initialize(AppObj* app_obj);
static CriBool app_atomex_finalize(AppObj* app_obj);
static CriBool app_execute_main(AppObj* app_obj);

static CriBool app_atomex_start(AppObj* app_obj);
static CriBool app_atomex_stop_player(AppObj* app_obj);
static CriBool app_atomex_stop_cue(AppObj* app_obj);

/**************************************************************************
 * 変数定義
 * Variable definitions
 **************************************************************************/

 /* キュー一覧 */
static AppCueListItem g_cue_list[] = {
 CRI_CUESHEET_0_GUN1_HIGH,
 CRI_CUESHEET_0_HELI_LOOP,
 CRI_CUESHEET_0_KALIMBA_G,
 CRI_CUESHEET_0_LEVELB,
 CRI_CUESHEET_0_LEVELC,
 CRI_CUESHEET_0_なんでしょう？,
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
