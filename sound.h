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
#include <cri_adx2le.h>
#include <unordered_map>
#include <vector>
#include "keyboard.h"
#include <thread>
#include <chrono>


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
	//BGM�@10��
	GAME_BGM,
	TITLE_BGM,//Title init	2/27�@�^�C�g��BGM���g�ύX
	STAGESELECT_BGM,//StageSelectScene inti
	RESULT_BGM,//RESULT_init
	BOSS1_BGM,
	STAGE1_BGM,//Game.cpp init
	POP_BGM,//Game.cpp init
	ADVENTURE_BGM,
	STAGE2_BGM,
	BOSS2_BGM,

	//UI�� 6��
	Select_Sound,
	Decision_Sound,
	Cancel_Sound,
	Pose_Sound,
	Result_Sound,
	Success_Sound,	//	2/27�ǉ��@�`���[�g���A���p������

	//�A���J�[���� 5��
	Anchor_Hit_Sound,//�Ȃ�₱��
	Anchor_Hit_Miss_Sound,//�Ȃ�
	Anchor_Pulling_Sound,//player.cpp connected
	Anchor_Thorw_Sound,//player.cpp crate_state
	Anchor_Mark_Sound, //anchor_point.cpp

	//�v���C���[SE 20��
	Player_Soul_Colect1_Sound,//playercpp update
	Player_Damege_Sound, //player.cpp Player_Damaged
	Player_Dead_Sound,//player_Stamina.cpp  EditPlayerStaminaValue
	Player_Coin_Colect_Sound,//Item_coni Update
	Player_Walk_Sound,// player.cpp Update
	Player_Attack_Sound,//player.cpp update CreateNormalAttack_State
	Player_Jewelry_Colect_Sound,//Item_Jewl.cpp update
	Player_GameOver_Sound,//�Q�[�����[�Ί�����ɓ���������
	Player_Soul_Colect2_Sound,//contactliast.h   // �v���[���[�ƃA�C�e�����Փ˂������𔻒�
	Player_Fall_Sound,//�����Ȃ�
	Player_Warp_Sound,//�����Ȃ�
	Player_Frame_Up_Sound,//player.cpp DrawAnchorLevel3Frame
	Player_Jump_Start_Sound,//player.cpp update �ύX�ς�
	Player_Jump_End_Sound,//					�ύX�ς�
	Player_Stock_Decrease_Sound,//		2/21�ǉ��@�c�@����
	Player_Buff_AnchorSpeedUp_Sound,//	2/21�ǉ��@�d�̃X�s�[�h�A�b�v
	Player_Buff_Invincible_Sound,//		2/21�ǉ��@�U������
	Player_Buff_SpeedUp_Sound,//		2/21�ǉ��@�v���C���[�̃X�s�[�h�A�b�v
	Player_Shield_Break_Sound,//		2/27�ǉ��@�v���C���[�̃o���A�j��
	Player_Respawn_Sound,//				2/27�ǉ��@���Ԓn�_������

	//��SE 8��
	Object_Rock_Fall_Sound,//static_to_dynamic_block.cpp  Update
	Object_Get_Treasure_Sound,
	Object_Wood_Fall_Sound,//wood �ύX�ς�
	Object_Rock_Roll_Sound,//rock.cpp  Pulling_rock
	Object_Pillar_Break_Sound,//boss_pillar.cpp  Destroy_Splitting
	Object_Pillar_Hit_Sound,
	Object_Geyser_Sound,// contact.list�@�@ �ύX�ς�
	Object_Geyser_Start_Sound,//			�ύX�ς�

	//�{�XSE 8��
	Boss_Charge_Attack_Sound,//update charge_attack_state
	Boss_Jump_Sound,//update jumpstate
	Boss_Attack_Wave_Sound,//update shock_wave_state
	Boss_Walk_Sound,//update walk_state
	Boss_Charge_Sound,//update charge_attack_state
	Boss_Core_Damege_Sound, //DestroyBossCore
	Boss_Damege_Sound,//boss.cpp CreateBossCore
	Boss_Stun_Sound,//Update�@down_state

	//�G�l�~�[SE 7��
	Enemy_Knock_Down1_Sound,//������
	Enemy_MiniGolem_Create_Sound,//boss.cpp  CreateMiniGolem
	Enemy_MiniGolem_Explosion_Sound,//boss.cpp  DestroyMiniGolemBody
	Enemy_Attack_Sound,//EnemyDyanamic.cpp attack
	Enemy_Knock_Down2_Sound,//EnemyDyanamic.cpp �@update
	Enemy_Shot_Down_Sound,//ContactList.h endcontact�@�C�ɓ����
	Enemy_Fly_Bomb_Sound
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


// ���y�̍Đ��i�Đ�ID��ۑ��j
CriBool app_atomex_start(Sound_Manager sound_name);

// ����̉����Đ������ǂ������m�F�i�Đ����I������ID���폜�j
CriBool app_atomex_is_playing(Sound_Manager sound_name);

// ���y�̒�~�i����̉��̂��ׂĂ̍Đ����~���A���X�g����폜�j
CriBool app_atomex_stop_cue(Sound_Manager sound_name);

// �v���C���[�S�̂��~�i���ׂẲ����Ǘ����X�g����폜�j
CriBool app_atomex_stop_player();

// �Đ����̉��̃��X�g���擾
std::vector<Sound_Manager> app_atomex_get_playing_sounds();

// ���[�v�ݒ��L����
void app_atomex_enable_loop();

// ���[�v�ݒ�𖳌����i1�񂾂��Đ��j
void app_atomex_disable_loop();

// �S�̂̉��ʂ𒲐�
void app_atomex_set_master_volume(float volume);

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
	CRI_CUESHEET_0_������,
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
	CRI_CUESHEET_0_�Ō���,
	CRI_CUESHEET_0_��Ύ擾,
	CRI_CUESHEET_0_���S��,
	CRI_CUESHEET_0_�\�E���z��2,
	CRI_CUESHEET_0_����,
	CRI_CUESHEET_0_���[�v��,
	CRI_CUESHEET_0_�t���[���A�b�v,
	CRI_CUESHEET_0_�v���C���[�̒��n��,
	CRI_CUESHEET_0_�v���C���[�̃W�����v��,
	CRI_CUESHEET_0_�c�@����,
	CRI_CUESHEET_0_�d�̃X�s�[�h�A�b�v,
	CRI_CUESHEET_0_�U������,
	CRI_CUESHEET_0_�ړ��X�s�[�h�A�b�v,
	CRI_CUESHEET_0_�o���A�j��,
	CRI_CUESHEET_0_���ԕ���,
	CRI_CUESHEET_0_�₪�����鉹,
	CRI_CUESHEET_0_�����鉹,
	CRI_CUESHEET_0_�؂��|��鉹,
	CRI_CUESHEET_0_�₪�]���鉹,
	CRI_CUESHEET_0_���|���ăo���o����,
	CRI_CUESHEET_0_���ƃ{�X���Ԃ��鉹,
	CRI_CUESHEET_0_�Ԍ���,
	CRI_CUESHEET_0_�Ԍ���o�n��,
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
	CRI_CUESHEET_0_�G�l�~�[���ĉ�,
	CRI_CUESHEET_0_���V�G����
};





static CriUint32 g_num_cue_items = sizeof(g_cue_list) / sizeof(AppCueListItem);


/***************************************************************************
 * �֐���`
 * Function definitions
 ***************************************************************************/

void CRIInitialize();
void CRIUpdate();
void CRIFinalize();

// �����ł͕ϐ��� `extern` �Ő錾����i��`���Ȃ��j
extern std::unordered_map<Sound_Manager, std::vector<CriAtomExPlaybackId>> g_playback_map;


#endif // SOUND_H
