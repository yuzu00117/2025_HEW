//-----------------------------------------------------------------------------------------------------
// #name player.h
// #description �v���C���[
// #make 2024/11/19
// #update 2024/11/20
// #comment �ǉ��E�C���\��
//          �E�R���X�g���N�^��body��fixture�����GetInstance�ł���
//           
//----------------------------------------------------------------------------------------------------


#ifndef PLAYER_H
#define PLAYER_H


#include"world_box2d.h"
class Player
{
public:
	Player();


	~Player();


	// �V���O���g���̃C���X�^���X�擾 �@���̃N���X�ł͈�̃C���X�^���X�����F�߂Ȃ�
	//static Player& GetInstance(b2Vec2 position = b2Vec2(1, 0), b2Vec2 size = b2Vec2(1, 2),b2Vec2 sensor_size=b2Vec2(40,30)) {
	//	static Player instance(position, size,sensor_size); // Initialized only once
	//	return instance;
	//}
	

		// �V���O���g���C���X�^���X�擾
	static Player& GetInstance() {
		static Player instance;
		return instance;
	}

	 // �v���C���[�̏�����
	void Initialize(b2Vec2 position, b2Vec2 body_size, b2Vec2 sensor_size);
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
		//���̏����͊G���オ������v���C���[�Ə��̊Ԃ̌��Ԃ𒲐����鏈��
		//size.y = size.y + 1;
		m_p_size = size;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSensorSize() const {return m_sensor_size;}
	void SetSensorSize(b2Vec2 sensor_size) {
		m_sensor_size = sensor_size;
	}


	//���W�����v�����ǂ������Z�b�g
	void SetIsJumping(bool flag)
	{
		m_is_jumping = flag;
	}
	//���W�����v�����ǂ������擾
	bool GetIsJumping(void)
	{
		return m_is_jumping;
	}



	//���̑��x���擾
	float	GetSpeed()
	{
		return m_speed;
	}

	//���̃v���C���[�̌������擾
	// �E�����F1    �������F0
	bool GetDirection() {
		return m_direction;
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


	//�W�����v�{�^�������ꂽ���ǂ����i�g���K�[����j
	static bool m_jump_pressed;
	//�W�����v�����ǂ���
	static bool    m_is_jumping;
	//�W�����v���鎞�̗́i�W�����v�ł��鍂���ɉe���j
	b2Vec2  m_jump_force = b2Vec2(0.0f, -0.40f);

	//���ړ��X�s�[�h
	float   m_speed = 0.04f;

	//�v���C���[�̌���
	// �E�����F1    �������F-1
	static bool		m_direction;

	//�A���J�[���g�p����t���O
	bool m_is_use_anchor = false;
};

#endif // !PLAYER_H