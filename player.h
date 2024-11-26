//-----------------------------------------------------------------------------------------------------
// #name player.h
// #description �v���C���[
// #make 2024/11/19
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �E�R���X�g���N�^��body��fixture�����Initialize�ł���
//           
//----------------------------------------------------------------------------------------------------


#ifndef PLAYER_H
#define PLAYER_H

#include"world_box2d.h"
class Player
{
public:
	Player() {};

	Player(b2Vec2 position, b2Vec2 size,b2Vec2 sensor_size);
	~Player();


	// �V���O���g���̃C���X�^���X�擾 �@���̃N���X�ł͈�̃C���X�^���X�����F�߂Ȃ�
	static Player& GetInstance(b2Vec2 position = b2Vec2(0, 0), b2Vec2 size = b2Vec2(1, 1),b2Vec2 sensor_size=b2Vec2(30,20)) {
		static Player instance(position, size,sensor_size); // Initialized only once
		return instance;
	}
	

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	static b2Body* GetOutSidePlayerBody();

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const 
	{
		return m_p_size;
	}
	void SetSize(b2Vec2 size)
	{
		m_p_size = size;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSensorSize() const {return m_sensor_size;}
	void SetSensorSize(b2Vec2 sensor_size) {
		m_sensor_size = sensor_size;
	}



	//���W�����v�\�����R���g���[������֐�
	void SetJumpFlag(bool flag)
	{
		m_can_jump = flag;
	}

	bool GetCanJump(void)
	{
		return m_can_jump;
	}


	 b2Body*GetPlayerBody(void)
	{
		return m_body;
	}

	void SetPlayerBody(b2Body* player_body)
	{
		m_body = player_body;
	}



private:

	//�v���C���[��Body������
	b2Body* m_body;

	//�`��p��Body�̃T�C�Y�����Ă����@
	// ���R�Ƃ��Ă͂����̃T�C�Y��box2d�Ȃ��ŊǗ�����̂͂��邢����
	//Body����fixture�Ƃ��Ă��ăT�C�Y�m�F���ā[�@���̎��͎l�p������[�@���邢�@���Ă���������
	b2Vec2 m_p_size;

	//�Z���T�[�p�̃T�C�Y
	b2Vec2 m_sensor_size;


	//���W�����v�\�Ȃ̂��icontactlist.h�̕��ŃR���g���[�����Ă�j
	bool    m_can_jump = true;

	//�W�����v���鎞�̗́i�W�����v�ł��鍂���ɉe���j
	b2Vec2  m_jump_force = b2Vec2(0.0, -0.02);

	//���ړ��X�s�[�h
	float   m_speed = 10.0f;

	//�A���J�[���g�p����t���O
	bool m_is_use_anchor = false;
};

#endif // !PLAYER_H