//----------------------------------------------------------------------------------------------------
// #name�@UI_Block.h
// #description �Z���T�[�ɐڐG������v���C���[�̉E���UI��\������
// #make 2025/02/09
// #update 2025/02/09
// #comment �ǉ��E�C���\��  UI�̎�ނ����񂪂񑫂�����
//         
//          
//----------------------------------------------------------------------------------------------------

#ifndef UI_BLOCK_H
#define UI_BLOCK_H

#include"include/box2d/box2d.h"
#include"Video.h"


//�G�ꂽ�牽��\���������
enum Ui_Block_Type
{
	NULL_UI_TYPE,//�Ȃɂ��Ȃ����
	ARROW,
	BUTTON_A,
	BUTTON_B,

	VIDEO_BUTTON_A,
	VIDEO_BUTTON_LEFT_STICK,
	VIDEO_BUTTON_RIGHT_STICK,
	VIDEO_BUTTON_ZR,

};



class UI_block
{
public:
	UI_block(b2Vec2 Position, b2Vec2 block_size,b2Vec2 Sensor_size,b2Vec2 Sensor_Position, Ui_Block_Type type,float texture_angle);
	~UI_block();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();


	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const
	{
		return m_size;
	}

	void SetSize(b2Vec2 size) {
		m_size = size;
	}


	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSensorSize() const
	{
		return m_sensor_size;
	}

	void SetSensorSize(b2Vec2 size) {
		m_sensor_size = size;
	}

	//body�̏K��
	b2Body* GetBody()
	{
		return m_body;
	}

	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	bool GetFlag(void)
	{
		return m_flag;
	}


	void SetFlag(bool flag)
	{
		m_flag = flag;
	}


	bool	GetIfVideo() { return m_is_video; }

	//���̓���̏�Ԃ��擾�i��������UI�����悶��Ȃ��ꍇ�͓���I��������Ă�����Ԃ��Ԃ��Ă���j
	Video_State GetVideoState() { 
		if (m_is_video) { return m_video.GetState(); } 
		return Video_Finished;
	}
	//���̓���̏�Ԃ��Z�b�g�i��������UI�����悶��Ȃ��ꍇ�̓Z�b�g����Ȃ��j
	void	SetVideoState(Video_State state)
	{
		if (m_is_video) { m_video.SetState(state); }
	}

private:

	b2Body* m_body;

	b2Vec2 m_size;

	b2Vec2 m_sensor_size;

	int id;

	//�Ǘ��Ɏg���֐�
	bool m_flag = false;

	Ui_Block_Type m_ui_type;

	float angle;

	Video m_video;	//����C���X�^���X
	bool	m_is_video;	//����UI�͓��悩�ǂ���

};


#endif //!UI_BLOCK_H
