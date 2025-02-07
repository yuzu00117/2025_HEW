//-----------------------------------------------------------------------------------------------------
// #name Item_Jewel.h
// #description     ��΃A�C�e��
// #make 2025/1/24�@���j�S
// #update 2025/1/24
// #comment �ǉ��E�C���\��
//                  �A���J�[�������C�N���ꂽ��Ԃ̕�΂̌��ʂɃA���J�[�����鑬�x�A�b�v��ǉ�����\��
//                  ���ʂ̔{���܂���������
//----------------------------------------------------------------------------------------------------

#include "Item_Jewel.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"renderer.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"player.h"
#include"player_stamina.h"
#include"Item_Manager.h"
#include"object_manager.h"
#include"sound.h"
#include"player_UI.h"
#include"easing.h"


ItemJewel::ItemJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool shape_polygon, float Alpha)
    :m_size(body_size), m_Alpha(Alpha), m_type(type)
{
    b2BodyDef body;
    body.type = b2_staticBody;
    body.position.Set(position.x, position.y);
    body.angle = angle;
    body.fixedRotation = true;//��]���Œ�ɂ���
    body.userData.pointer = (uintptr_t)this;


    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    m_body = world->CreateBody(&body);

    SetSize(body_size);//�v���C���[�\�������邽�߂ɃZ�b�g����


    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


    b2FixtureDef fixture;
    b2Fixture* p_fixture;


    //�l�p�`�̏ꍇ
   //-----------------------------------------------------------------------------
    if (shape_polygon)
    {
        b2PolygonShape shape;
        shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);

        fixture.shape = &shape;
        fixture.density = 1.0f;//���x
        fixture.friction = 0.3f;//���C
        fixture.restitution = 0.1f;//�����W��
        fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

        p_fixture = m_body->CreateFixture(&fixture);

    }
    //�~�̏ꍇ
//-----------------------------------------------------------------------------
    else
    {
        b2CircleShape shape;
        shape.m_radius = size.x * 0.5f;

        fixture.shape = &shape;
        fixture.density = 1.0f;//���x
        fixture.friction = 0.3f;//���C
        fixture.restitution = 0.1f;//�����W��
        fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

        p_fixture = m_body->CreateFixture(&fixture);
    }

    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* data = new ObjectData{ collider_item };
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);


    data->Item_name = ITEM_JEWEL;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);


}

void	ItemJewel::Update()
{
    if (m_destory && m_body != nullptr)
    {
        app_atomex_start(Player_Coin_Colect_Sound);
        SetIfCollecting(false);
        //�{�f�B�̏�������
        b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
        world->DestroyBody(m_body);
        m_body = nullptr;
        
        return;
    }

    if (m_collecting)
    {
        auto ring_position = player_UI::GetRingPosition();
        ring_position.y += 170.0f;


        //�C�[�W���O
        //=====================================================================================================================================
        float collecting_time = Ease::InQuint(m_collecting_time);

        b2Vec2	new_position;	//start����end��t�ŕۊǂ���retv�����
        new_position.x = (1 - collecting_time) * m_position_collecting_start.x + collecting_time * ring_position.x;
        new_position.y = (1 - collecting_time) * m_position_collecting_start.y+ collecting_time * ring_position.y;

        m_body->SetTransform(new_position, 0.0f);
        m_position_while_collecting = new_position;

        m_collecting_time += 1.0f / 60.0f;

        //=====================================================================================================================================
        if (m_position_while_collecting.x <= ring_position.x)
        {
            SetDestory(true);
            player_UI::SetJewelCollected(m_type, true);
        }

    }

}

void ItemJewel::SetIfCollecting(bool flag)
{
    m_collecting = flag;

    if (m_collecting)
    {
        float scale = SCREEN_SCALE;

        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;


        // �R���C�_�[�̈ʒu�̎擾�i�A�C�e���[�̈ʒu�j
        b2Vec2 position;
        position.x = m_body->GetPosition().x;
        position.y = m_body->GetPosition().y;

        m_position_collecting_start.x = SCREEN_WIDTH / 2;
        m_position_collecting_start.y = (PlayerPosition::GetPlayerPosition().y * BOX2D_SCALE_MANAGEMENT * scale + screen_center.y)- SCREEN_HEIGHT / 2;

        m_position_while_collecting = m_position_collecting_start;

        app_atomex_start(Player_Coin_Colect_Sound);


    }
}

void    ItemJewel::Function()
{
    Player& player = Player::GetInstance();
    ObjectManager& object_manager = ObjectManager::GetInstance();
    float speed;
    float speed_change_value;
    b2Vec2 jump_force;
    b2Vec2 jump_force_change_value;

    switch (m_type)
    {
    case BLUE:
        //�v���C���[�̈ړ����x��1.5�{
        speed = player.GetSpeed();
        speed_change_value = speed * 4.0f - speed;
        player.SetSpeed(speed_change_value);
        //�v���C���[�̃W�����v�͂�1.5�{
        jump_force = player.GetJumpForce();
        jump_force_change_value = b2Vec2{ 0.0f, jump_force.y * 1.2f - jump_force.y };
        player.SetJumpForce(jump_force_change_value);
        break;
    case RED:
        //�I�u�W�F���������鎞�̗͂�{���Ŋ|����
        object_manager.SetPullingPower_With_Multiple(b2Vec2{ 1.5f,1.5f });
        break;
    case YELLOW:
        PlayerStamina::SetAvoidDamageOnce(true);
        break;
    }

}


void ItemJewel::Initialize()
{
    switch (m_type)
    {
    case BLUE:
        g_Texture = InitTexture(L"asset\\texture\\Item_texture\\item_blue_jewel.png");
        break;
    case RED:
        g_Texture = InitTexture(L"asset\\texture\\Item_texture\\item_red_jewel.png");
        break;
    case YELLOW:
        g_Texture = InitTexture(L"asset\\texture\\Item_texture\\item_yellow_jewel.png");
        break;
    }
}



void ItemJewel::Draw()
{
    if (m_body != nullptr)
    {
        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


        // �R���C�_�[�ƈʒu���̕␳�����邽��
        float scale = SCREEN_SCALE;

        if (m_collecting)
        {
            //�`��
            DrawSpriteOld(
                { m_position_while_collecting.x,
                  m_position_while_collecting.y },
                m_body->GetAngle(),
                { GetSize().x * scale * 1.7f,GetSize().y * scale * 1.7f },
                m_Alpha
            );

            return;
        }

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
            { GetSize().x * scale * 1.5f,GetSize().y * scale * 1.5f },
            m_Alpha
        );

    }
}


void ItemJewel::Finalize()
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
    }
}

ItemJewel::~ItemJewel()
{
}
