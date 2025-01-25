//-----------------------------------------------------------------------------------------------------
// #name 1-1_boss.h
// #description boss������@������
// #make 2025/01/07�@�@�i��`��
// #update 2025/01/21
// #comment �ǉ��E�C���\��
//          �E���ɂȂ�
//----------------------------------------------------------------------------------------------------
#ifndef BOSS_1_1_H
#define BOSS_1_1_H

#include"easing.h"
#include"include/box2d/box2d.h"



enum boss_state
{
	wait_state,//�҂����
	panic_state,//���ݏ��
	walk_state,//�������
	jump_state,//�W�����v���
	charge_attack_state,//�`���[�W�U����
	shock_wave_state,//�V���b�N�E�F�[�u�������Ă�����
	create_mini_golem_state,//�����ȃS�[�����𐶐����Ă�����

};

class Boss_1_1
{
public:
	Boss_1_1();
	~Boss_1_1();


	// �V���O���g���C���X�^���X�擾
	static Boss_1_1& GetInstance() {
		static Boss_1_1 instance;
		return instance;
	}



	void Initialize(b2Vec2 position, b2Vec2 bodysize,bool left);
	void Update();
	void UpdateCoolTime();
	void Draw();
	void debugDraw();//�U���͈͂�\�������肷��
	void Finalize();

	void CreateBossCore(b2Vec2 size);//�{�X�̎�_��I�o��������
	void DestroyBossCore(void);//�{�X�̃A���J�[�|�C���g�̃{�f�B����


	void BossDamaged(void);//�{�X�̔�e��������
	void BossDead(void);   //�{�X�̎��S����

	void CreateChargeAttack(b2Vec2 attack_size, bool left);//���ߍU���̐�������

	void CreateShockWave(b2Vec2 attack_size, bool left);//�Ռ��g�U���̐�������
	void ShockWaveUpdate(void);//�Ռ��g�U���̍X�V����

	void JumpUpdate(void);

	void DeleteAttackBody();//�U���̔�����폜���鏈���@���݂̂Ƃ���@�Ռ��g�U���Ƃ��ߍU���̍U������̍폜
	


	void CreateMiniGolem(b2Vec2 minigolem_size, bool left);//�~�j�S�[�����̍쐬
	void MiniGolemUpdate(void);//�~�j�S�[�����̍X�V����
	void DestroyMiniGolemBody(void);

	b2Body* GetOutSideBody(void);//Body���Ƃ�CPP����Ƃ��Ă���悤

	///-----------------------------------------------------------------------
	//�{�X�{��
	b2Vec2 GetBossDrawSize(void)
	{
		return boss_size;
	}
	void SetBossDrawSize(b2Vec2 size)
	{
		boss_size = size;
	}

	b2Body* GetBossBody(void)
	{
		return m_body;
	}

	void SetBossBody(b2Body* body)
	{
		m_body = body;
	}

	void SetBossSensorSize(b2Vec2 size)
	{
		boss_sensor_size = size;
	}
	b2Vec2 GetBossSensorSize(void)
	{
		return boss_sensor_size;
	}


	void SetAnchorPointBody(b2Body* body)
	{
		anchorpoint_body = body;
	}

	b2Body* GetAnchorPointBody(void)
	{
		return anchorpoint_body;
	}

	void SetAnchorPointSize(b2Vec2 size)
	{
		anchorpoint_size = size;
	}

	b2Vec2 GetAnchorPointSize(void)
	{
		return anchorpoint_size;
	}

	//-------------------------------------------------------------------------------------------
	//�U���̔���
	b2Vec2 GetAttackDrawSize(void)
	{
		return attack_size;
	}
	void SetAttackDrawSize(b2Vec2 size)
	{
		attack_size = size;
	}

	b2Body* GetAttackBody(void)
	{
		return m_attack_body;
	}

	void SetAttackBody(b2Body* body)
	{
		m_attack_body = body;
	}
	//-------------------------------------------------------------------------------------------
	//�~�j�S�[������
	b2Body* GetMiniGolemBody(int index)
	{
		return m_mini_golem_body[index];
	}

	void SetMiniGolemBody(b2Body* body,int index)
	{
		m_mini_golem_body[index] = body;
	}


	b2Vec2 GetMiniGolemDrawSize(void)
	{
		return mini_golem_size;
	}
	void SetMiniGolemDrawSize(b2Vec2 size)
	{
		mini_golem_size = size;
	}


	void SetDestroyMiniGolemBody(bool flag, b2Body* body)
	{
		destroy_mini_golem_flag = flag;
		destroy_mini_golem_body = body;
	}


	//--------------------------------------------------------------------------------------
	//

	int GetBossFieldLevel(void)//�{�X�̃t�B�[���h�̊Ǘ�
	{
		return boss_field_level;
	}

	//�v���C���[���߂��ɂ��邩�̊Ǘ�
	bool GetPlayerisNearbyFlag(void)
	{
		return Player_is_Nearby;
	}
	//�v���C���[���߂��ɂ��邩�̊Ǘ�
	void SetPlayerisNearbyFlag(bool flag)
	{
		Player_is_Nearby = flag;
	}
	//
	int GetPlayerNearbylocked(void)
	{
		return Player_is_Nearby_locked;
	}
	void SetPlayerNearbylocked(int cnt)
	{
		Player_is_Nearby_locked = cnt;
	}


	int GetBossHP(void)
	{
		return boss_hp;
	}

	void SetBossHP(int hp)
	{
		boss_hp = hp;
	}

	boss_state GetNowBossState(void)
	{
		return now_boss_state;
	}
	

	//-------------------------------------------------------------------------------------------
private:

	b2Vec2 Boss_size = b2Vec2(18, 24);

	int boss_hp=3;		   //boss��HP

	int boss_field_level=1;//�{�X�̏��̕�����Ǘ�����֐�


	b2Body* m_body;//�{�X�̃{�f�B
	b2Vec2 boss_size;//�`��Ŏg���{�X�̃T�C�Y

	b2Vec2 boss_sensor_size;//�{�X�̃Z���T�[�T�C�Y

	b2Body* m_attack_body;//�U���̔���
	b2Vec2 attack_size;//�U���̔���̃T�C�Y


	b2Body* anchorpoint_body;
	b2Vec2 anchorpoint_size;




	b2Body* m_mini_golem_body[2];//�{�f�B
	b2Vec2 mini_golem_size;
	b2Body* destroy_mini_golem_body;
	bool destroy_mini_golem_flag=false;

	



	float sheet_cnt;//�V�[�g�̊Ǘ��Ŏg���Ă���



	bool left_flag;//�v���C���[���ǂ���ɂ��邩�̕`��p

	bool old_left_flag;//�U������𔻕ʂ��邽��



	boss_state now_boss_state;//�{�X�̃X�e�[�g�Ǘ�

	//-------------------------------------------------------------------------------------------
	//�N�[���^�C���̊Ǘ�

	//�Ռ��g�U��
	static constexpr int Max_Shock_Wave_CoolTime = 600;//�Ռ��g�U���ő�N�[���^�C���@
	int					 Now_Shock_Wave_CoolTime = 0;  //���݂̃N�[���^�C��

	//�W�����v
	static constexpr int Max_Jump_CoolTime = 600;//�W�����v�̍ő�N�[���^�C���@
	int					 Now_Jump_CoolTime = 200;  //���݂̃N�[���^�C��

	//���␶��
	static constexpr int Max_Create_MiniGolem_CoolTime = 1800;//����̍ő�N�[���^�C���@
	int					 Now_Create_MiniGolem_CoolTime = 1500;  //���݂̃N�[���^�C��

	//�`���[�W�U��
	static constexpr int Max_Charge_Attack_CoolTime = 1500;//�`���[�W�U���̍ő�N�[���^�C���@
	int					 Now_Charge_Attack_CoolTime = 1500; //���݂̃N�[���^�C��

	//�������[�V����
	static constexpr int Max_Max_Walk_CoolTime = 120;//�������[�V�����̍ő�N�[���^�C���@
	int					 Now_Max_Walk_CoolTime = 0; //���݂̃N�[���^�C��





	//--------------------------------------------------------------------------------------------
	//�{�X�̊Ǘ�������@�ϐ�
	bool Player_is_Nearby = false;//�v���C���[���߂��ɂ��邩�̊Ǘ�
	int Player_is_Nearby_locked;//�{�f�B�������֌W�ňꎞ�Z���T�[�̔���𖳌��ɂ�����


	//----------------------------------------------------------------------------------------------
	//�Ռ��g�U��
	static constexpr int Max_Shock_Wave_Sheet = 98;			//�Ռ��g�U���̍ő�t���[��
	static constexpr int Shock_Wave_Start_Frame = 50;		//�Ռ��g�̔����t���[��
	static constexpr int Shock_Wave_End_Frame = 71;			//�Ռ��g�̏I���t���[��


	static constexpr float Shock_Wave_Speed = 1.5f;			//�Ռ��g�̔��ł����X�s�[�h

	bool Shock_Wave_Fly_flag=false;
	static constexpr int Shock_Wave_time_Frame = 180;
	int Now_Shock_Wave_time_Frame = 0;


	//-------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------
	//�~�j�S�[�����̐���
	static constexpr int Max_Create_Mini_Golem_Sheet = 98;	//�~�j�S�[�����̐�������ő�t���[��
	static constexpr int Create_Mini_Golem_Start_Frame = 46;

	bool Mini_golem_Create_flag=true;

	//-----------------------------------------------------------------------------------------------
	//���ߍU���̃t���[���B
	static constexpr int Max_Charge_Attack_Sheet = 200;		//���ߍU���̐�������ő�t���[��
	static constexpr int Charge_Attack_Start_Frame = 67;//���ߍU���̃��[�V�����̃{�f�B�̔����t���[��
	static constexpr int Charge_Attack_End_Frame = 102;//���ߍU���̃��[�V�����̃{�f�B�̔����t���[��

	//-----------------------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------------------------
	//�W�����v���[�V�����̍ő�t���[��
	static constexpr int Max_Jump_Sheet = 72;
	static constexpr int Jump_Attack_Start_Frame = 52;//�W�����v�ɂ���Ĕ�������U���̔����t���[��
	static constexpr int Jump_Attack_End_Frame = 56;//�W�����v�ɂ���Ĕ�������U���̏I���t���[��

	static constexpr int Jump_Start_Frame = 15;//�W�����v�̔����t���[��
	static constexpr int Jump_End_Frame = 20;///�W�����v�̏㏸�I���t���[��

	bool Jump_flag = false;
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------
	//���݂̃��[�V�����̍ő�t���[��
	static constexpr int Max_Panic_Sheet = 49;

	static constexpr int Max_Panic_Stun_Frame = 600;//���͂P�O�b
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------
	//�������[�V����
	static constexpr int Max_Walk_Sheet = 72;
	//-------------------------------------------------------------------------------------------


	static constexpr float boss_alpha = 3.0f;//�{�X�̃A���t�@�l
};



#endif // !1_1_BOSS_H