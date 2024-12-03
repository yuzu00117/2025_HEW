//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description ���I�A�ÓI�G�l�~�[�̌p�����A�G�l�~�[�N���X��cpp�t�@�C��
// #make 2024/11/19
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>

Enemy::Enemy(b2Vec2 body_size, b2Vec2 position, bool dynamic)
    : m_size(body_size), m_body(nullptr)
{
    //bool�^���� dynamic��true�Ȃ瓮�I�Afalse�Ȃ�ÓI�G�l�~�[
    //�����ȊO�w�ǃv���C���[���l
    b2BodyDef body;
    if (dynamic)
    {
        body.type = b2_dynamicBody;
    }
    else
    {
        body.type = b2_staticBody;
    }
    body.position.Set(position.x, position.y);
    body.angle = 0.0f;
    body.fixedRotation = true;//��]���Œ�ɂ���
    body.userData.pointer = (uintptr_t)this;

    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    m_body = world->CreateBody(&body);

    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;

    b2PolygonShape shape;
    shape.SetAsBox(size.x * 0.5, size.y * 0.5f);


    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;//���x
    fixture.friction = 1.0f;//���C
    fixture.restitution = 0.1f;//�����W��
    fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

    m_body->CreateFixture(&fixture);
}

void Enemy::Finalize()
{
    
}