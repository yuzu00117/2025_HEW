//-----------------------------------------------------------------------------------------------------
// #name wood.h
// #description �؂̃I�u�W�F�N�g�̃w�b�_�[
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//			�E����Ȃ�E�����ɓ|�ꂽ���Ƃ����������邩���ˁ[
//----------------------------------------------------------------------------------------------------


#ifndef WOOD_H
#define WOOD_H

#include"include/box2d/box2d.h"
#include"sound.h"
#include<list>

enum Wood_State
{
	Wood_Idle,
	Wood_Pulling,
	Wood_Falling,
	Wood_HitObject,
};


class wood
{
public:
	wood(b2Vec2 position,b2Vec2 Woodsize,b2Vec2 AnchorPoint_size,int need_level);
	~wood();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_wood(b2Vec2 pullingpower);


	// ID ���擾����
	int GetID() const {
		return id;
	}
	
	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetWoodSize() const
	{
		return m_Wood_size;
	}

	void SetWoodSize(b2Vec2 size) {
		m_Wood_size = size;
	}


	//body�̏K��
	b2Body* GetObjectWoodBody()
	{
		return Wood_body;
	}

	void SetObjectWoodBody(b2Body* body)
	{
		Wood_body = body;
	}	
	
	

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetStumpSize() const
	{
		return m_Stump_size;
	}

	void SetStumpSize(b2Vec2 size) {
		m_Stump_size = size;
	}
	//body�̏K��
	b2Body* GetObjectStumpBody()
	{
		return Stump_body;
	}

	void SetObjectStumpBody(b2Body* body)
	{
		Stump_body = body;
	}

	//�ؖ{�̂Ɛ؂芔�̊Ԃ̗n�ڃW���C���g�擾
	b2Joint* GetWoodStumpJoint() 
	{
		return m_wood_stump_joint; 
	}

	void	SetWoodStumpJoint(b2Joint* joint)
	{ 
		m_wood_stump_joint = joint; 
	}


	bool	GetIfPulling() {
		if (m_state == Wood_Pulling)return true;
		else return false;
	}
	void	SetIfPulling(bool flag) {
		if (flag)
		{
			SetState(Wood_Pulling);
		}
		else SetState(Wood_Idle);
	}

	//	�؂̏�Ԃ��Z�b�g
	void	SetState(Wood_State state);

	///-----------------------------------------------------------------------------
	//�A���J�[�|�C���g


	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetAnchorPointSize() const
	{
		return m_AnchorPoint_size;
	}

	void SetAnchorPointSize(b2Vec2 size) {
		m_AnchorPoint_size = size;
	}

	//body�̏K��
	b2Body* GetObjectAnchorPointBody()
	{
		return AnchorPoint_body;
	}

	void SetObjectAnchorPointBody(b2Body* body)
	{
		AnchorPoint_body = body;
	}

private:
	int id; // �e�C���X�^���X�ŗL�� ID

	b2Body* Wood_body;

	b2Vec2 m_Wood_size;


	b2Body* Stump_body;

	b2Vec2 m_Stump_size;

	b2Joint*	m_wood_stump_joint;

	bool	m_destory_joint = false;	//�؂芔�Ɩ{�̂̃W���C���g���������ǂ���

	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

	Wood_State	m_state = Wood_Idle;

	int		start_stop_sound_count = 120;	//�Q�[���J�n����؂��n�ʂ܂ŗ����鎞���炳�Ȃ����߂̃J�E���g�_�E��
	float	angle_when_pulling_start = 0;	//��������n�߂鎞�≹�炳�ꂽ����̖؂̉�]�p�x��ێ�

	//����
	//----------------------------------------
	Sound_Manager m_sound_FalledDown = Object_Wood_Fall_Sound;	//�|�ꂽ���̉�
};
#endif // !WOOD_H
