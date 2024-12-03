#include"include/box2d/box2d.h"
#include"test_object_wood.h"
#include"world_box2d.h"
#include"collider_type.h"

test_wood::test_wood(b2Vec2 Postion)
{

	b2Vec2 anchorpoint_size = b2Vec2(1.0f/BOX2D_SCALE_MANAGEMENT, 1.0f / BOX2D_SCALE_MANAGEMENT);//�A���J�[�|�C���g�̃T�C�Y
	b2Vec2 MiddleWood_size = b2Vec2(1.0f / BOX2D_SCALE_MANAGEMENT, 5.0f / BOX2D_SCALE_MANAGEMENT);//�؂̐^��
	b2Vec2 UnderWood_size = b2Vec2(1.0f / BOX2D_SCALE_MANAGEMENT, 1.0f / BOX2D_SCALE_MANAGEMENT);//�؂̉�
	
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	b2Fixture* object_fixture;
	ObjectData* objectdata;
//----------------------------------------------------------------------------//
//��ڂ̃{�f�B������

	b2BodyDef body_anchor_point;//�A���J�[�|�C���g��
	body_anchor_point.type = b2_dynamicBody;
	body_anchor_point.position.Set(Postion.x, Postion.y - (MiddleWood_size.y * 0.5 + anchorpoint_size.y * 0.5));
	body_anchor_point.fixedRotation = false;

	b2Body* m_anchorpoint_body = world->CreateBody(&body_anchor_point);

	SetObjectBody(m_anchorpoint_body, 0);

	b2PolygonShape anchor_point_shape;
	anchor_point_shape.SetAsBox(anchorpoint_size.x * 0.5, anchorpoint_size.y * 0.5);

	b2FixtureDef anchorpoint_fixture;

	anchorpoint_fixture.shape = &anchor_point_shape;
	anchorpoint_fixture.density = 1.0f;
	anchorpoint_fixture.friction = 0.05f;//���C
	anchorpoint_fixture.restitution = 0.0f;//�����W��
	anchorpoint_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	object_fixture = m_anchorpoint_body->CreateFixture(&anchorpoint_fixture);


};

test_wood::~test_wood()
{
}


void test_wood::Initialize()
{

}

void test_wood::Update()
{

}

void test_wood::Draw()
{

}

void test_wood::Finalize()
{

}