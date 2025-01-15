//-----------------------------------------------------------------------------------------------------
// #name sound.h
// #description sound
// #make 2024/12/03�@�@�����F��
// #update 2024/12/03
// #comment 
//          
//----------------------------------------------------------------------------------------------------
#ifndef SOUND_H
#define SOUND_H


 /**************************************************************************
  * �C���N���[�h
  * Header files
  **************************************************************************/
#include <cri_adx2le.h>
#include <CriSmpFramework.h>

  /* �C���Q�[���v���r���[��L���ɂ���ꍇ�A��`���� */
#define USE_INGAME_PREVIEW

/**************************************************************************
 * �萔�}�N����`
 * Constant macro definitions
 **************************************************************************/
 /* ACF/ACB�̃}�N����`�w�b�_ */
#include "../../cridata/Public/CueSheet_0.h"
#include "../../cridata/Public/Test_acf.h"

/* �f�[�^�f�B���N�g���ւ̃p�X */
#define PATH	"./cridata/Public/"

/* �T���v���Ŏg�p����t�@�C���� */
#define ACF_FILE			"Test.acf"
#define ACB_FILE			"CueSheet_0.acb"
//#define AWB_FILE			"Basic.awb"

/* �ő�{�C�X���𑝂₷���߂̊֘A�p�����[�^ */
#define MAX_VOICE			(24)
#define MAX_VIRTUAL_VOICE	(64)		/* �������葽��(�ʏ�{�C�X�{HCA-MX�{�C�X�{��) */
#define MAX_CRIFS_LOADER	(64)		/* �������葽��(�ʏ�{�C�X�{HCA-MX�{�C�X�{��) */

/* �ő�T���v�����O���[�g�i�s�b�`�ύX�܂ށj */
#define MAX_SAMPLING_RATE	(48000*2)

/* HCA-MX�R�[�f�b�N�̃T���v�����O���[�g */
#define SAMPLINGRATE_HCAMX		(32000)


#define X_POS_OFFSET_FOR_MAP_DRAW	(20)
#define PITCH_CHANGE_VALUE			(-200.0f)

/**************************************************************************
 * �^��`
 * Type definitions
 **************************************************************************/

 /* �L���[���X�g�A�C�e�� */
typedef struct AppCueListItemTag {
	CriAtomExCueId id;
} AppCueListItem;

/* �A�v���P�[�V�����\���� */
typedef struct AppTag {
	CriAtomExPlayerHn		player;		/* �Đ��v���[�� */
	CriAtomExVoicePoolHn	standard_voice_pool;	/* �{�C�X�v�[��(ADX/HCA�R�[�f�b�N�p) */
	CriAtomExVoicePoolHn	hcamx_voice_pool;		/* �{�C�X�v�[��(HCA-MX�p) */
	CriAtomExAcbHn			acb_hn;		/* ACB�n���h��(�����f�[�^) */
	CriAtomDbasId			dbas_id;	/* D-BAS�̍쐬*/

	CriAtomExPlaybackId		playback_id;	/* Voice�L���[�̃v���C�o�b�NID(�Đ��J�n���ɕێ�����) */

	CriSint32 ui_cue_idnex;				/* ���[�U�I�𒆂̃L���[   */

} AppObj;



/***************************************************************************
 * �֐��錾
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
 * �ϐ���`
 * Variable definitions
 **************************************************************************/

 /* �L���[�ꗗ */
static AppCueListItem g_cue_list[] = {
 CRI_CUESHEET_0_GUN1_HIGH,
 CRI_CUESHEET_0_HELI_LOOP,
 CRI_CUESHEET_0_KALIMBA_G,
 CRI_CUESHEET_0_LEVELB,
 CRI_CUESHEET_0_LEVELC,
 CRI_CUESHEET_0_�Ȃ�ł��傤�H,
};
static CriUint32 g_num_cue_items = sizeof(g_cue_list) / sizeof(AppCueListItem);


/***************************************************************************
 * �֐���`
 * Function definitions
 ***************************************************************************/

void CRIInitialize();
void CRIUpdate();
void CRIFinalize();

#endif // SOUND_H
