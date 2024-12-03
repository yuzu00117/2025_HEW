//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 動的、静的エネミーの継承元、エネミークラスのcppファイル
// #make 2024/11/19
// #update 2024/11/22
// #comment 追加・修正予定
//          ・
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
    //bool型引数 dynamicがtrueなら動的、falseなら静的エネミー
    //ここ以外殆どプレイヤー同様
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
    body.fixedRotation = true;//回転を固定にする
    body.userData.pointer = (uintptr_t)this;

    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    m_body = world->CreateBody(&body);

    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;

    b2PolygonShape shape;
    shape.SetAsBox(size.x * 0.5, size.y * 0.5f);


    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;//密度
    fixture.friction = 1.0f;//摩擦
    fixture.restitution = 0.1f;//反発係数
    fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える

    m_body->CreateFixture(&fixture);
}

void Enemy::Finalize()
{
    
}