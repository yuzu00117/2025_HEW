#ifndef BOSS_1_1_H
#define BOSS_1_1_H

#include"easing.h"
#include"include/box2d/box2d.h"



enum boss_state
{
	wait_state,//�҂����
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




	void Initialize(b2Vec2 position, b2Vec2 bodysize);
	void Update();
	void Draw();
	void debugDraw();
	void Finalize();


	b2Vec2 GetBossSize(void)
	{
		return boss_size;
	}
	void SetBossSize(b2Vec2 size)
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




private:

	b2Body *m_body;

	b2Vec2 boss_size;//�`��Ŏg���{�X�̃T�C�Y

	float sheet_cnt;//�V�[�g�̊Ǘ��Ŏg���Ă���

	bool  display_shake_flag=true;

	

	boss_state now_boss_state;


	static constexpr int Max_Shock_Wave_Sheet =98;			//�Ռ��g�U���̍ő�t���[��

	static constexpr int Max_Create_Mini_Golem_Sheet = 98;	//�~�j�S�[�����̐�������ő�t���[��

	static constexpr int Max_Charge_Attack_Sheet = 200;		//���ߍU���̐�������ő�t���[��

	static constexpr int Max_Walk_Sheet = 72;
};



#endif // !1_1_BOSS_H
