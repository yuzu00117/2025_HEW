//----------------------------------------------------------------------------------------------------
// #name�@texture_blokc.h
// #description �w�i�ɕ`�悷��e�N�X�`��
// #make 2025/02/09
// #update 2025/02/09
// #comment �ǉ��E�C���\��  
//         
//          
//----------------------------------------------------------------------------------------------------

#ifndef TEXTURE_BLOCK_H
#define TEXTURE_BLOCK_H

#include"include/box2d/box2d.h"
#include"sprite.h"
#include"texture.h"
#include <memory>  // �ǉ�




class Texture_block
{
public:
	Texture_block(b2Vec2 Position, b2Vec2 block_size, float texture_angle, ID3D11ShaderResourceView* texture);
	~Texture_block();


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

	ID3D11ShaderResourceView* m_texture;

	std::unique_ptr<class ObjectData> m_objectData;  // ���j�[�N�|�C���^�[�ɕύX

	float angle;

};


#endif //!UI_BLOCK_H