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

	Anchor_Hit_Sound,//�Ȃ�₱��
	Anchor_Hit_Miss_Sound,//�Ȃ�
	Anchor_Pulling_Sound,//player.cpp connected
	Anchor_Thorw_Sound,//player.cpp crate_state
	Anchor_Mark_Sound, //anchor_point.cpp

	Player_Soul_Colect1_Sound,//playercpp update
	Player_Damege_Sound, //player.cpp Player_Damaged
	Player_Dead_Sound,//player_Stamina.cpp  EditPlayerStaminaValue
	Player_Coin_Colect_Sound,//Item_coni Update
	Player_Walk_Sound,// player.cpp Update
	Player_Jump_Start_Sound,//player.cpp update ��������
	Player_Jump_End_Sound,//�����Ȃ�
	Player_Attack_Sound,//player.cpp update CreateNormalAttack_State
	Player_Jewelry_Colect_Sound,//Item_Jewl.cpp update
	Player_GameOver_Sound,//�Q�[�����[�Ί�����ɓ���������
	Player_Soul_Colect2_Sound,//contactliast.h   // �v���[���[�ƃA�C�e�����Փ˂������𔻒�
	Player_Fall_Sound,//�����Ȃ�
	Player_Warp_Sound,//�����Ȃ�
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
	Boss_Stun_Sound,//Update�@down_state

	Enemy_Knock_Down1_Sound,
	Enemy_MiniGolem_Create_Sound,//boss.cpp  CreateMiniGolem
	Enemy_MiniGolem_Explosion_Sound,//boss.cpp  DestroyMiniGolemBody
	Enemy_Attack_Sound,
	Enemy_Knock_Down2_Sound,
	Enemy_Shot_Down_Sound

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
CriBool app_atomex_stop_cue(Sound_Manager sound_name);

/**************************************************************************
 * �ϐ���`
 * Variable definitions
 **************************************************************************/

 /* �L���[�ꗗ */
static AppCueListItem g_cue_list[] = {
	CRI_CUESHEET_0_1_1BGM,
	CRI_CUESHEET_0_�^�C�g��BGM,
	CRI_CUESHEET_0_BGM_STAGESELECT,
	CRI_CUESHEET_0_���U���gBGM,
	CRI_CUESHEET_0_BGM_�{�X,
	CRI_CUESHEET_0_BGM_�W�����O��,
	CRI_CUESHEET_0_BGM_�|�b�v,
	CRI_CUESHEET_0_BGM_�T��,
	CRI_CUESHEET_0_BGM_�W�����O��2,
	CRI_CUESHEET_0_BGM_�{�X2,
	CRI_CUESHEET_0_�I����,
	CRI_CUESHEET_0_���艹,
	CRI_CUESHEET_0_�L�����Z����,
	CRI_CUESHEET_0_�ꎞ��~��_���j���[��_,
	CRI_CUESHEET_0_���U���g���\,
	CRI_CUESHEET_0_�A���J�[�q�b�g�|�C���g,
	CRI_CUESHEET_0_�A���J�[�|�C���g�~�X,
	CRI_CUESHEET_0_�A���J�[���������鉹,
	CRI_CUESHEET_0_�A���J�[�����鉹,
	CRI_CUESHEET_0_�A���J�[�|�C���g�}�[�N�������̉�,
	CRI_CUESHEET_0_�\�E���z��1,
	CRI_CUESHEET_0_�_���[�W���󂯂鉹,
	CRI_CUESHEET_0_���ꂽ��,
	CRI_CUESHEET_0_�R�C���擾��,
	CRI_CUESHEET_0_�v���C���[�̑���,
	CRI_CUESHEET_0_�W�����v,
	CRI_CUESHEET_0_���n��,
	CRI_CUESHEET_0_�Ō���,
	CRI_CUESHEET_0_��Ύ擾,
	CRI_CUESHEET_0_���S��,
	CRI_CUESHEET_0_�\�E���z��2,
	CRI_CUESHEET_0_����,
	CRI_CUESHEET_0_���[�v��,
	CRI_CUESHEET_0_�t���[���A�b�v,
	CRI_CUESHEET_0_�₪�����鉹,
	CRI_CUESHEET_0_�����鉹,
	CRI_CUESHEET_0_�؂��|��鉹,
	CRI_CUESHEET_0_�₪�]���鉹,
	CRI_CUESHEET_0_�Ԍ���,
	CRI_CUESHEET_0_���|���ăo���o����,
	CRI_CUESHEET_0_���ƃ{�X���Ԃ��鉹,
	CRI_CUESHEET_0_�{�X���ߍU���Ռ���,
	CRI_CUESHEET_0_�{�X�W�����v���n��,
	CRI_CUESHEET_0_�{�X�Ռ��g������,
	CRI_CUESHEET_0_�{�X����,
	CRI_CUESHEET_0_�{�X���ߍU��,
	CRI_CUESHEET_0_�R�A�_���[�W��,
	CRI_CUESHEET_0_�{�X�ɑ΂���_���[�W,
	CRI_CUESHEET_0_�{�X�X�^����,
	CRI_CUESHEET_0_�G��|����,
	CRI_CUESHEET_0_���уS�[����������,
	CRI_CUESHEET_0_���уS�[����������,
	CRI_CUESHEET_0_�G�l�~�[�U��,
	CRI_CUESHEET_0_�G�l�~�[�_�E��,
	CRI_CUESHEET_0_�G�l�~�[���ĉ�
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
