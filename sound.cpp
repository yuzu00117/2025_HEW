//-----------------------------------------------------------------------------------------------------
// #name sound.cpp
// #description sound
// #make 2024/12/03�@�@�����F��
// #update 2024/12/03
// #comment 
//          
//----------------------------------------------------------------------------------------------------

 /**************************************************************************
  * �C���N���[�h
  * Header files
  **************************************************************************/
#include "sound.h"
#include "keyboard.h"

  /***************************************************************************
   * �֐���`
   * Function definitions
   ***************************************************************************/
   /* �A�v���P�[�V�����I�u�W�F�N�g�̒�` */
AppObj app_obj = { 0 };

void CRIInitialize(void) {


	/* �A�v���P�[�V�����̏����� */
	app_atomex_initialize(&app_obj);



}

void CRIUpdate(void)
{
	if (Keyboard_IsKeyDown(KK_D1))
	{
		app_atomex_start(&app_obj);
	}

	/* �A�v���P�[�V�����̍X�V */
	app_execute_main(&app_obj);

}

void CRIPlay(void)
{
	app_atomex_start(&app_obj);
}

void CRIFinalize(void)
{
	/* �A�v���P�[�V�����̏I�� */
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

	/* �J�[�\���̃��Z�b�g */
	app_obj->ui_cue_idnex = 0;
	/* ���擾�ȃv���C�o�b�NID(Voice�L���[�Đ����̂ݎ擾) */
	app_obj->playback_id = 0;


	/* �������A���P�[�^�̓o�^ */
	criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);

	/* ���C�u�����̏������i�ő�{�C�X���ύX�j */
	CriAtomExConfig_WASAPI lib_config;
	CriFsConfig fs_config;
	criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
	criFs_SetDefaultConfig(&fs_config);
	lib_config.atom_ex.max_virtual_voices = MAX_VIRTUAL_VOICE;
	lib_config.hca_mx.output_sampling_rate = SAMPLINGRATE_HCAMX;
	fs_config.num_loaders = MAX_CRIFS_LOADER;
	lib_config.atom_ex.fs_config = &fs_config;
	criAtomEx_Initialize_WASAPI(&lib_config, NULL, 0);

	/* D-Bas�̍쐬�i�ő�X�g���[�����͂����Ō��܂�܂��j */
	app_obj->dbas_id = criAtomDbas_Create(NULL, NULL, 0);

#if defined(USE_INGAME_PREVIEW)
	CriAtomExAsrBusAnalyzerConfig analyze_config;
	CriSint32 bus_no;
	/* �C���Q�[���v���r���[�p�̃��j�^���C�u������������ */
	criAtomExMonitor_Initialize(NULL, NULL, 0);
	/* ���x������@�\��ǉ� */
	criAtomExAsr_SetDefaultConfigForBusAnalyzer(&analyze_config);
	for (bus_no = 0; bus_no < 8; bus_no++) {
		criAtomExAsr_AttachBusAnalyzer(bus_no, &analyze_config);
	}
#endif

	/* ACF�t�@�C���̓ǂݍ��݂Ɠo�^ */
	criAtomEx_RegisterAcfFile(NULL, PATH ACF_FILE, NULL, 0);

	/* DSP�ݒ�̃A�^�b�` */
	criAtomEx_AttachDspBusSetting(CRI_TEST_ACF_DSPSETTING_MIXER, NULL, 0);

	/* �{�C�X�v�[���̍쐬�i�ő�{�C�X���ύX�^�ő�s�b�`�ύX�^�X�g���[���Đ��Ή��j */
	CriAtomExStandardVoicePoolConfig standard_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standard_vpool_config);
	standard_vpool_config.num_voices = MAX_VOICE;
	standard_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	standard_vpool_config.player_config.streaming_flag = CRI_TRUE;
	app_obj->standard_voice_pool = criAtomExVoicePool_AllocateStandardVoicePool(&standard_vpool_config, NULL, 0);

	/* HCA-MX�Đ��p�F�{�C�X�v�[���̍쐬 */
	CriAtomExHcaMxVoicePoolConfig hcamx_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForHcaMxVoicePool(&hcamx_vpool_config);
	hcamx_vpool_config.num_voices = MAX_VOICE;
	hcamx_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	hcamx_vpool_config.player_config.streaming_flag = CRI_TRUE;
	app_obj->hcamx_voice_pool = criAtomExVoicePool_AllocateHcaMxVoicePool(&hcamx_vpool_config, NULL, 0);

	/* ACB�t�@�C����ǂݍ��݁AACB�n���h�����쐬 */
	app_obj->acb_hn = criAtomExAcb_LoadAcbFile(NULL, PATH ACB_FILE, NULL, NULL, NULL, 0);

	/* �v���[���̍쐬 */
	app_obj->player = criAtomExPlayer_Create(NULL, NULL, 0);

	return CRI_TRUE;
}

CriBool app_atomex_finalize(AppObj* app_obj)
{
	/* DSP�̃f�^�b�` */
	criAtomEx_DetachDspBusSetting();

	/* �v���[���n���h���̔j�� */
	criAtomExPlayer_Destroy(app_obj->player);

	/* �{�C�X�v�[���̔j�� */
	criAtomExVoicePool_Free(app_obj->hcamx_voice_pool);
	criAtomExVoicePool_Free(app_obj->standard_voice_pool);

	/* ACB�n���h���̔j�� */
	criAtomExAcb_Release(app_obj->acb_hn);

	/* ACF�̓o�^���� */
	criAtomEx_UnregisterAcf();

#if defined(USE_INGAME_PREVIEW)
	/* �C���Q�[���v���r���[�֘A�@�\�̏I������ */
	CriSint32 bus_no;
	for (bus_no = 0; bus_no < 8; bus_no++) {
		criAtomExAsr_DetachBusAnalyzer(bus_no);
	}
	criAtomExMonitor_Finalize();
#endif

	/* D-BAS�̔j�� */
	criAtomDbas_Destroy(app_obj->dbas_id);

	/* ���C�u�����̏I�� */
	criAtomEx_Finalize_WASAPI();

	return CRI_TRUE;
}

CriBool app_execute_main(AppObj* app_obj)
{
	/* �T�[�o�����̎��s */
	criAtomEx_ExecuteMain();

#if defined(USE_INGAME_PREVIEW)
	/* �o�X��͏��̎擾 */
	CriAtomExAsrBusAnalyzerInfo analyze_info;
	CriSint32 bus_no;
	for (bus_no = 0; bus_no < 8; bus_no++) {
		criAtomExAsr_GetBusAnalyzerInfo(bus_no, &analyze_info);
	}
#endif

	return CRI_TRUE;
}

//1	���y�̍Đ�
static CriBool app_atomex_start(AppObj* app_obj)
{
	CriAtomExCueId start_cue_id = g_cue_list[5].id;

	/* �L���[ID�̎w�� */
	criAtomExPlayer_SetCueId(app_obj->player, app_obj->acb_hn, start_cue_id);

	/* MEMO: ����̉������s�b�`��ς��čĐ��������ꍇ�B      */
	/* (1) �v���[���Ƀs�b�`��ݒ�B                          */
	/* (2) �Đ��J�n�B                                        */
	/* (3) �v���[���̃s�b�`��߂��B                          */
	/* {                                                     */
	/*   criAtomExPlayer_SetPitch(player, pitch);          */
	/*   criAtomExPlayer_SetCueId(player, acb, cue_id);  */
	/*   criAtomExPlayer_Start(player, pitch);             */
	/*   criAtomExPlayer_SetPitch(player, 0.0f);           */
	/* {                                                     */
	/* �⑫: HCA-MX�R�[�f�b�N�̏ꍇ�̓s�b�`�ύX�͖����B      */

	/* �Đ��̊J�n */
	CriAtomExPlaybackId playback_id = criAtomExPlayer_Start(app_obj->player);

	return CRI_TRUE;
}

//2 ���y�̒�~
static CriBool app_atomex_stop_player(AppObj* app_obj)
{
	/* �v���[���̒�~ */
	criAtomExPlayer_Stop(app_obj->player);

	return CRI_TRUE;
}

//3 ����̉��̂ݒ�~
static CriBool app_atomex_stop_cue(AppObj* app_obj)
{
	/* ����̍Đ����̂ݒ�~ */
	criAtomExPlayback_Stop(app_obj->playback_id);

	return CRI_TRUE;
}
