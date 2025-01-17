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
#include "../../cridata/Public/HEW_sound_acf.h"

/* �f�[�^�f�B���N�g���ւ̃p�X */
#define PATH	"./cri/cridata/Public/"

/* �T���v���Ŏg�p����t�@�C���� */
#define ACF_FILE			"HEW_sound.acf"
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
 * �֐��錾
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
 * �ϐ���`
 * Variable definitions
 **************************************************************************/

 /* �L���[�ꗗ */
static AppCueListItem g_cue_list[] = {
CRI_CUESHEET_0_1_1BGM,					//1
CRI_CUESHEET_0_�^�C�g��BGM,				//2
CRI_CUESHEET_0_BGM_STAGESELECT,			//3
CRI_CUESHEET_0_���U���gBGM,				//4
CRI_CUESHEET_0_�I����,					//5
CRI_CUESHEET_0_���艹,					//6
CRI_CUESHEET_0_�L�����Z����,			//7
CRI_CUESHEET_0_�ꎞ��~��_���j���[��_,	//8
CRI_CUESHEET_0_�A���J�[�q�b�g�|�C���g,	//9
CRI_CUESHEET_0_�A���J�[�|�C���g�~�X,	//10
CRI_CUESHEET_0_�A���J�[���������鉹,	//11
CRI_CUESHEET_0_�A���J�[�����鉹,		//12
CRI_CUESHEET_0_�W�����v,				//13
CRI_CUESHEET_0_�\�E���z��,				//14
CRI_CUESHEET_0_�_���[�W���󂯂鉹,		//15
CRI_CUESHEET_0_���ꂽ��,				//16
CRI_CUESHEET_0_�₪�����鉹,			//17
CRI_CUESHEET_0_�����鉹,				//18
CRI_CUESHEET_0_�؂��|��鉹,			//19
CRI_CUESHEET_0_�₪�]���鉹,			//20
CRI_CUESHEET_0_�R�C������鉹,			//21
CRI_CUESHEET_0_�G��|����,				//22
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
