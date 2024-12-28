//-----------------------------------------------------------------------------------------------------
// #name sound.cpp
// #description sound
// #make 2024/12/03　　今村友哉
// #update 2024/12/03
// #comment 
//          
//----------------------------------------------------------------------------------------------------

 /**************************************************************************
  * インクルード
  * Header files
  **************************************************************************/
#include "sound.h"
#include "keyboard.h"

  /***************************************************************************
   * 関数定義
   * Function definitions
   ***************************************************************************/
   /* アプリケーションオブジェクトの定義 */
AppObj app_obj = { 0 };

void CRIInitialize(void) {


	/* アプリケーションの初期化 */
	app_atomex_initialize(&app_obj);



}

void CRIUpdate(void)
{
	if (Keyboard_IsKeyDown(KK_D1))
	{
		app_atomex_start(&app_obj);
	}

	/* アプリケーションの更新 */
	app_execute_main(&app_obj);

}

void CRIPlay(void)
{
	app_atomex_start(&app_obj);
}

void CRIFinalize(void)
{
	/* アプリケーションの終了 */
	app_atomex_finalize(&app_obj);

}

void* user_alloc_func(void* obj, CriUint32 size)
{
	void* ptr;
	ptr = malloc(size);
	return ptr;
}

void user_free_func(void* obj, void* ptr)
{
	free(ptr);
}


CriBool app_atomex_initialize(AppObj* app_obj)
{

	/* カーソルのリセット */
	app_obj->ui_cue_idnex = 0;
	/* 未取得なプレイバックID(Voiceキュー再生時のみ取得) */
	app_obj->playback_id = 0;


	/* メモリアロケータの登録 */
	criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);

	/* ライブラリの初期化（最大ボイス数変更） */
	CriAtomExConfig_WASAPI lib_config;
	CriFsConfig fs_config;
	criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
	criFs_SetDefaultConfig(&fs_config);
	lib_config.atom_ex.max_virtual_voices = MAX_VIRTUAL_VOICE;
	lib_config.hca_mx.output_sampling_rate = SAMPLINGRATE_HCAMX;
	fs_config.num_loaders = MAX_CRIFS_LOADER;
	lib_config.atom_ex.fs_config = &fs_config;
	criAtomEx_Initialize_WASAPI(&lib_config, NULL, 0);

	/* D-Basの作成（最大ストリーム数はここで決まります） */
	app_obj->dbas_id = criAtomDbas_Create(NULL, NULL, 0);

#if defined(USE_INGAME_PREVIEW)
	CriAtomExAsrBusAnalyzerConfig analyze_config;
	CriSint32 bus_no;
	/* インゲームプレビュー用のモニタライブラリを初期化 */
	criAtomExMonitor_Initialize(NULL, NULL, 0);
	/* レベル測定機能を追加 */
	criAtomExAsr_SetDefaultConfigForBusAnalyzer(&analyze_config);
	for (bus_no = 0; bus_no < 8; bus_no++) {
		criAtomExAsr_AttachBusAnalyzer(bus_no, &analyze_config);
	}
#endif

	/* ACFファイルの読み込みと登録 */
	criAtomEx_RegisterAcfFile(NULL, PATH ACF_FILE, NULL, 0);

	/* DSP設定のアタッチ */
	criAtomEx_AttachDspBusSetting(CRI_TEST_ACF_DSPSETTING_MIXER, NULL, 0);

	/* ボイスプールの作成（最大ボイス数変更／最大ピッチ変更／ストリーム再生対応） */
	CriAtomExStandardVoicePoolConfig standard_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standard_vpool_config);
	standard_vpool_config.num_voices = MAX_VOICE;
	standard_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	standard_vpool_config.player_config.streaming_flag = CRI_TRUE;
	app_obj->standard_voice_pool = criAtomExVoicePool_AllocateStandardVoicePool(&standard_vpool_config, NULL, 0);

	/* HCA-MX再生用：ボイスプールの作成 */
	CriAtomExHcaMxVoicePoolConfig hcamx_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForHcaMxVoicePool(&hcamx_vpool_config);
	hcamx_vpool_config.num_voices = MAX_VOICE;
	hcamx_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	hcamx_vpool_config.player_config.streaming_flag = CRI_TRUE;
	app_obj->hcamx_voice_pool = criAtomExVoicePool_AllocateHcaMxVoicePool(&hcamx_vpool_config, NULL, 0);

	/* ACBファイルを読み込み、ACBハンドルを作成 */
	app_obj->acb_hn = criAtomExAcb_LoadAcbFile(NULL, PATH ACB_FILE, NULL, NULL, NULL, 0);

	/* プレーヤの作成 */
	app_obj->player = criAtomExPlayer_Create(NULL, NULL, 0);

	return CRI_TRUE;
}

CriBool app_atomex_finalize(AppObj* app_obj)
{
	/* DSPのデタッチ */
	criAtomEx_DetachDspBusSetting();

	/* プレーヤハンドルの破棄 */
	criAtomExPlayer_Destroy(app_obj->player);

	/* ボイスプールの破棄 */
	criAtomExVoicePool_Free(app_obj->hcamx_voice_pool);
	criAtomExVoicePool_Free(app_obj->standard_voice_pool);

	/* ACBハンドルの破棄 */
	criAtomExAcb_Release(app_obj->acb_hn);

	/* ACFの登録解除 */
	criAtomEx_UnregisterAcf();

#if defined(USE_INGAME_PREVIEW)
	/* インゲームプレビュー関連機能の終了処理 */
	CriSint32 bus_no;
	for (bus_no = 0; bus_no < 8; bus_no++) {
		criAtomExAsr_DetachBusAnalyzer(bus_no);
	}
	criAtomExMonitor_Finalize();
#endif

	/* D-BASの破棄 */
	criAtomDbas_Destroy(app_obj->dbas_id);

	/* ライブラリの終了 */
	criAtomEx_Finalize_WASAPI();

	return CRI_TRUE;
}

CriBool app_execute_main(AppObj* app_obj)
{
	/* サーバ処理の実行 */
	criAtomEx_ExecuteMain();

#if defined(USE_INGAME_PREVIEW)
	/* バス解析情報の取得 */
	CriAtomExAsrBusAnalyzerInfo analyze_info;
	CriSint32 bus_no;
	for (bus_no = 0; bus_no < 8; bus_no++) {
		criAtomExAsr_GetBusAnalyzerInfo(bus_no, &analyze_info);
	}
#endif

	return CRI_TRUE;
}

//1	音楽の再生
static CriBool app_atomex_start(AppObj* app_obj)
{
	CriAtomExCueId start_cue_id = g_cue_list[5].id;

	/* キューIDの指定 */
	criAtomExPlayer_SetCueId(app_obj->player, app_obj->acb_hn, start_cue_id);

	/* MEMO: 特定の音だけピッチを変えて再生したい場合。      */
	/* (1) プレーヤにピッチを設定。                          */
	/* (2) 再生開始。                                        */
	/* (3) プレーヤのピッチを戻す。                          */
	/* {                                                     */
	/*   criAtomExPlayer_SetPitch(player, pitch);          */
	/*   criAtomExPlayer_SetCueId(player, acb, cue_id);  */
	/*   criAtomExPlayer_Start(player, pitch);             */
	/*   criAtomExPlayer_SetPitch(player, 0.0f);           */
	/* {                                                     */
	/* 補足: HCA-MXコーデックの場合はピッチ変更は無効。      */

	/* 再生の開始 */
	CriAtomExPlaybackId playback_id = criAtomExPlayer_Start(app_obj->player);

	return CRI_TRUE;
}

//2 音楽の停止
static CriBool app_atomex_stop_player(AppObj* app_obj)
{
	/* プレーヤの停止 */
	criAtomExPlayer_Stop(app_obj->player);

	return CRI_TRUE;
}

//3 特定の音のみ停止
static CriBool app_atomex_stop_cue(AppObj* app_obj)
{
	/* 特定の再生音のみ停止 */
	criAtomExPlayback_Stop(app_obj->playback_id);

	return CRI_TRUE;
}
