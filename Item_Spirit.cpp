//-----------------------------------------------------------------------------------------------------
// #name Item_Spirit.cpp
// #description		�\�E���i�G�����Ƃ��A�C�e���j
// #make 2024/12/28�@���j�S
// #update 2025/1/10
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------
#include "Item_Spirit.h"
#include "texture.h"
#include "sprite.h"
#include "player_position.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_stamina.h"
#include"create_filter.h"
#include"player.h"


static ID3D11ShaderResourceView* g_Texture = NULL;//�e�N�X�`��



ItemSpirit::ItemSpirit(b2Vec2 position, b2Vec2 body_size, float angle, float recovery, float Alpha)
    :m_size(body_size), m_Alpha(Alpha), m_recovery(recovery)
{
    b2BodyDef body;
    body.type = b2_dynamicBody;
    body.position.Set(position.x, position.y);
    body.angle = angle;
    body.fixedRotation = true;//��]���Œ�ɂ���
    body.userData.pointer = (uintptr_t)this;
    body.gravityScale = 0;



    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    m_body = world->CreateBody(&body);
    m_body->SetLinearDamping(100.0f); //�������x�𐧌�����i�l���傫���قǂ������j

    SetSize(body_size);//�v���C���[�\�������邽�߂ɃZ�b�g����


    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


    b2FixtureDef fixture;
    b2Fixture* p_fixture;

    //�l�p�`�����蔻��
   //-----------------------------------------------------------------------------
    b2PolygonShape shape;
    shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);

    fixture.shape = &shape;
    fixture.density = 1.0f;//���x
    fixture.friction = 0.0f;//���C
    fixture.restitution = 0.1f;//�����W��
    fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

    p_fixture = m_body->CreateFixture(&fixture);
    

    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* data = new ObjectData{ collider_item };
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);


    data->Item_name = ITEM_SPIRIT;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);


}

void	ItemSpirit::Update()
{
    if (m_body != nullptr)
    {
        //�v���C���[�ɉ������Ă�Ȃ�
        if (m_state == Spirit_Collecting)
        {
            b2Vec2 player_position = PlayerPosition::GetPlayerPosition();
            //  �v���C���[�֌������x�N�g��
            b2Vec2 vec;
            vec.x = player_position.x - m_body->GetPosition().x;
            vec.y = player_position.y - m_body->GetPosition().y;
            vec.Normalize();

            float speed = 0.5f;

            GetBody()->ApplyLinearImpulseToCenter(b2Vec2(vec.x * speed, vec.y * speed), true);

            //�\�E���̃T�C�Y�����X�Ɍ���
            if (m_size.x > 0.8f || m_size.y > m_size.x * 2)
            {
                m_size.x -= 0.005f;
                m_size.y -= 0.005f;
            }

        }
        //��Ԃ��㏸���̏ꍇ
        if (m_state == Spirit_Rising)
        {
            //��ɏ㏸
            GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -0.1f), true);
        }
        if (m_state == Spirit_Falling)
        {
            //���������̏I�_�ɒ�������
            if (m_body->GetPosition().y >= m_Falling_to_position.y)
            {
                SetState(Spirit_Idle);
            }
        }
        //�������\��Ȃ�{�f�B�[������
        if (m_state == Spirit_Destory)
        {
            //�{�f�B�̏�������
            b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
            world->DestroyBody(m_body);
            m_body = nullptr;
        }
    }

}


void ItemSpirit::SetState(SpiritState state)
{
    if (m_state == Spirit_Destory)
    {
        return;
    }

    if (m_body != nullptr)
    {
        m_state = state;
        switch (m_state)
        {
        case Spirit_Idle:
            m_body->SetGravityScale(0);
            break;
        case Spirit_Rising:
            m_body->SetGravityScale(0);
            //GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -0.1f), true);
            break;
        case Spirit_Falling:
            m_body->SetGravityScale(1);
            break;
        case Spirit_Collecting:
        {
            if (!m_CollidedObject.empty())
            {
                m_CollidedObject.clear();
            }
            if (m_state == Spirit_Destory)
            {
                return;
            }
            //  �����蔻����Z���T�[�ɕύX�A�t�B���^���Ȃ��ɕύX�i���ɂ������ł���j
            b2Fixture* fixture = m_body->GetFixtureList();
            b2Filter filter;
            filter.maskBits = 0xFFFF;
            fixture->SetFilterData(filter);
            fixture->SetSensor(true);

            //  ��ʂ̃T�C�Y���A���J�[�����x���R�̎��̃T�C�Y�Ƒz�肵�āA�v���C���[����߂����ቓ���ꍇ�ʒu����ʋ߂��܂ňړ�������
            b2Vec2 position = m_body->GetPosition();
            b2Vec2 player_position = PlayerPosition::GetPlayerPosition();
            b2Vec2 sensor_size = Player::GetInstance().GetSensorSizeLev3();


            //  �i +/- adjust�j�͂��łɉ�ʋ߂��ɂ���\�E������������
            float adjust = 1.5f;
            float SCREEN_LEFT = player_position.x - sensor_size.x / BOX2D_SCALE_MANAGEMENT / 2 - adjust;
            float SCREEN_RIGHT = player_position.x + sensor_size.x / BOX2D_SCALE_MANAGEMENT / 2 + adjust;

            if (position.x < SCREEN_LEFT)
            {
                position.x = SCREEN_LEFT;
            }
            else if (position.x > SCREEN_RIGHT)
            {
                position.x = SCREEN_RIGHT;
            }
            m_body->SetTransform(position, m_body->GetAngle());

        }
            break;
        }
    }

}

void    ItemSpirit::Function()
{
    PlayerStamina::EditPlayerStaminaValue(m_recovery);
}



void ItemSpirit::Initialize()
{
    if (g_Texture == NULL)
    {
        g_Texture = InitTexture(L"asset\\texture\\sample_texture\\tama.png");
    }

}



void ItemSpirit::Draw()
{
    if (m_body != nullptr)
    {
        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

        // �R���C�_�[�ƈʒu���̕␳�����邽��
        float scale = SCREEN_SCALE;

        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;


        // �R���C�_�[�̈ʒu�̎擾�i�A�C�e���[�̈ʒu�j
        b2Vec2 position;
        position.x = m_body->GetPosition().x;
        position.y = m_body->GetPosition().y;


        // �v���C���[�ʒu���l�����ăX�N���[���␳��������
        //�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
        float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


        //�`��
        DrawSprite(
            { draw_x,
              draw_y },
            m_body->GetAngle(),
            { GetSize().x * scale,GetSize().y * scale },
            m_Alpha
        );

    }
}


void ItemSpirit::Finalize()
{

    if (GetBody() != nullptr)
    {
        //���[���h�̃C���X�^���X�������Ă���
        Box2dWorld& box2d_world = Box2dWorld::GetInstance();
        b2World* world = box2d_world.GetBox2dWorldPointer();
        world->DestroyBody(GetBody());
        SetBody(nullptr);
    }
    if (g_Texture != nullptr)
    {
        UnInitTexture(g_Texture);
        g_Texture = NULL;
    }

    m_CollidedObject.clear();
}

ItemSpirit::~ItemSpirit()
{
}
