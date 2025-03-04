//-----------------------------------------------------------------------------------------------------
// #name ground.h
// #description �n��
// #make 2024/11/22�@�i��`��
// #update 2024/11/22 
// #comment �ǉ��E�C���\��
//          �E�ǂɂ߂荞�ނƃW�����v�ł���̂ł����̏C��
//----------------------------------------------------------------------------------------------------

#ifndef GROUND_H
#define GROUND_H

#include"include/box2d/box2d.h"
#include"texture.h"
#include"sprite.h"
#include <memory>  // �ǉ�



//Filed���p�����Ă���@Draw�Ƃ��̏������̂�Field�ł���Ă邼
class Ground
{
public:
	Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, ID3D11ShaderResourceView *texture,bool object_sensor);
	~Ground();

	void Initialize(void);
	void Update(void);
	void Draw(void);
	void Finalize(void);


	b2Body* GetBody(void)
	{
		return m_body;
	}

	void SetBody(b2Body* body)
	{
		m_body = body;
	}

	b2Vec2 GetSize(void)
	{
		return m_size;
	}

	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}

private:
	b2Body* m_body;
	b2Vec2 m_size;

	std::unique_ptr<class ObjectData> m_objectData;  // ���j�[�N�|�C���^�[�ɕύX
	
	ID3D11ShaderResourceView* g_texture;
	

};



#endif // !Ground_H
