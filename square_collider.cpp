//-----------------------------------------------------------------------------------------------------
// #name square_collider.cpp
// #description �l�p�̂����蔻��
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//          �E�����Ŏl�p�̂����蔻��̃R���X�g���N�^���Ă�
// �@�@�@�@ �E�����łقƂ�ǂ̏������Ǘ����Ă�
//          �E��������Ȃ��C����������Ƃ��낢�����邯�ǃf�t�H���g�͂���Ƃ��āA����̌p�������ō�낤���Ȃ��Ďv���Ă�i�����W���Ƃ��ˁj
// �@�@�@�@ �E�Ƃ��������\�ȏ������Ă���̂ŁA���╪�������Č���ꂽ��@������ĂȂ�@����܂�ו������Ă���肸�炭�˂��Ďv�������ǁ@������Ȃ�ʂɂ�����
//          �E�X�P�[������̏����͂������傢��肭�ł���C�������
//          
//----------------------------------------------------------------------------------------------------

#include"square_collider.h"
#include"include/box2d/box2d.h"
#include"sprite.h"
#include"texture.h"
#include"box2d_world.h"




//�R���X�g���N�V����
SquareCollider::SquareCollider(XMFLOAT2 p, XMFLOAT2 s, float angle, bool bFixed)
{
	//�C���X�^���X���擾
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();

	//���[���h�ɓo�^
	this->m_p_world = box2d_world.GetBox2dWorldPointer();

	//��������T�C�Y����
	//���̍ۂɉ��Z�̕��ׂ������邽�߂Ɂ@�T�C�Y��/100����
	//�`��̂Ƃ��Ɂ��P�O�O���Ē�������
	m_p_size.x = s.x/100;
	m_p_size.y = s.y/100;

	////�R���C�_�[�̃^�C�v���Z�b�g  �����l�Ƃ��Ēn�ʂ��Z�b�g
	m_p_collider_type = collider_ground;

	////�e�N�X�`�����Z�b�g�����Ⴄ�I
	//SetColliderTexture(m_p_collider_type);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef dBody;
	dBody.type = bFixed ? b2_staticBody : b2_dynamicBody;
	dBody.position.Set(p.x, p.y);
	dBody.angle = angle;
	dBody.userData.pointer = (uintptr_t)this;

	
	

	m_p_body = box2d_world.GetBox2dWorldPointer()->CreateBody(&dBody);
	
	
	// Define another box shape for our dynamic body.
	b2PolygonShape shape;
	shape.SetAsBox(s.x * 0.5f, s.y * 0.5f);

	if (!bFixed) {
		// Define the dynamic body fixture.
		b2FixtureDef fixture;
		fixture.shape = &shape;

		// Set the box density to be non-zero, so it will be dynamic.
		fixture.density = 1.0f;


		//������I���ɂ���ƃZ���T�[�ɂȂ�

		/*fixture.isSensor = true;*/


		// Override the default friction.
		fixture.friction = 0.3f;

		// Add the shape to the body.
		m_p_body->CreateFixture(&fixture);
	}
	else {

		b2FixtureDef fixture;

		m_p_body->CreateFixture(&shape, 0);
	}

	
}
//void SquareCollider::SetColliderTexture(ColliderType collider_type)
//{
//	//�R���C�_�[�̃^�C�v�ɂ���ăe�N�X�`����ύX�@�@�����܂Ŏ����A�����Ƀe�N�X�`���킽���ā@��񎝂����������Ǘ����₷����������
//	switch (collider_type)
//	{
//	case collider_enemy:
//		m_p_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
//		break;
//	case collider_player:
//		m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_bule.png");
//		break;
//	case collider_anchor:
//		m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
//		break;
//	case collider_ground:
//		m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");
//		break;
//	default:
//		m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_black.png");
//		break;
//	}
//}







