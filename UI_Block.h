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


//�G�ꂽ�牽��\���������
enum Ui_Block_Type
{
	NULL_UI_TYPE,//�Ȃɂ��Ȃ����
	BOTTOM_A,
	BOTTOM_B

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

private:

	b2Body* m_body;

	b2Vec2 m_size;

	b2Vec2 m_sensor_size;

	int id;

	//�Ǘ��Ɏg���֐�
	bool m_flag = false;

	Ui_Block_Type m_ui_type;

	float angle;

};


#endif //!UI_BLOCK_H
