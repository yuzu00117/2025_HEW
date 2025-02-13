
//-------------------------------------------------------------------------------------------
// #name�@break_block.h
// #description �I�u�W�F�N�g�ƐڐG���������u���b�N
// #make 2025/02/06
// #update 2025/02/06
// #comment �ǉ��E�C���\��  �����Ȋ���������
//         
//          
//----------------------------------------------------------------------------------------------------
#ifndef BREAK_BLOCK_H
#define BREAK_BLOCK_H

#include"include/box2d/box2d.h"
#include"sprite.h"
#include"texture.h"


class Break_Block
{
public:
	Break_Block(b2Vec2 Position, b2Vec2 block_size,int divisions_x,int divisions_y,float angle,ID3D11ShaderResourceView* g_Texture);
	~Break_Block();


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


	

	ID3D11ShaderResourceView* Texture;

	float angle;

};


#endif //!UI_BLOCK_H
