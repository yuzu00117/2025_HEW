#ifndef CHANGE_ENEMY_FILTER_AND_BODY_H
#define CHANGE_ENEMY_FILTER_AND_BODY_H
#include"include/box2d/box2d.h"
#include"sprite.h"


class change_enemy_filter_and_body
{
public:
	change_enemy_filter_and_body(b2Vec2 position, b2Vec2 size, b2Vec2 vectol,ID3D11ShaderResourceView* Textur,float speed,float angle);//�R�C���̃e�N�X�`�� );
	~change_enemy_filter_and_body();


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


	//�{�f�B���擾
	b2Body* GetBody(void)
	{
		return m_body;
	}

	//�{�f�B���Z�b�g
	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	//�T�C�Y���擾
	b2Vec2 GetSize(void)
	{
		return m_size;
	}
	//�T�C�Y���Z�b�g
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}

private:

	int id;

	b2Body* m_body;

	b2Vec2 m_size;

	ID3D11ShaderResourceView* g_Texture;


};

#endif // !CHANGE_ENEMY_FILTER_AND_BODY_H
