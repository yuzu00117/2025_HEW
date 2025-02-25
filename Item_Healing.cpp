//----------------------------------------------------------------------------------------------------
// #name Item_Healing.cpp
// #description     �񕜃A�C�e���\�[�X�t�@�C��
// #make 2025/02/25
// #update 2025/02/25 �����A
// #comment �ǉ��E�C���\��
//----------------------------------------------------------------------------------------------------

#include"Item_Healing.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"renderer.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"player.h"
#include"Item_Manager.h"
#include"Item_Coin_UI.h"
#include"sound.h"

static ID3D11ShaderResourceView* g_Healing_Texture = NULL;//�񕜃A�C�e���̃e�N�X�`��
static ID3D11ShaderResourceView* g_Effect_Texture = NULL;//�G�t�F�N�g�̃e�N�X�`��

ItemHealing::ItemHealing(b2Vec2 position, b2Vec2 body_size, float angle)
    :m_position(position), m_size(body_size), m_angle(angle)
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

    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;

    //�t�B�N�X�`��=============================================================
    b2FixtureDef fixture;
    b2Fixture* p_fixture;

    b2CircleShape shape;
    shape.m_radius = size.x * 0.5f;

    fixture.shape = &shape;
    fixture.density = 1.0f;//���x
    fixture.friction = 0.3f;//���C
    fixture.restitution = 0.1f;//�����W��
    fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

    p_fixture = m_body->CreateFixture(&fixture);

    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* data = new ObjectData{ collider_item };
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

    data->Item_name = ITEM_HEALING;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);
}

void ItemHealing::CreateBody()
{
    if (m_destory)
    {
        return;
    }
    b2BodyDef body;
    body.type = b2_staticBody;
    body.position.Set(m_position.x, m_position.y);
    body.angle = m_angle;
    body.fixedRotation = true;//��]���Œ�ɂ���
    body.userData.pointer = (uintptr_t)this;


    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    m_body = world->CreateBody(&body);

    SetSize(m_size);//�v���C���[�\�������邽�߂ɃZ�b�g����


    b2Vec2 size;
    size.x = m_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
    size.y = m_size.y / BOX2D_SCALE_MANAGEMENT;


    b2FixtureDef fixture;
    b2Fixture* p_fixture;

    b2CircleShape shape;
    shape.m_radius = size.x * 0.5f;

    fixture.shape = &shape;
    fixture.density = 1.0f;//���x
    fixture.friction = 0.3f;//���C
    fixture.restitution = 0.1f;//�����W��
    fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

     p_fixture = m_body->CreateFixture(&fixture);


    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* data = new ObjectData{ collider_item };
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);


    data->Item_name = ITEM_HEALING;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);
}

//�A�C�e�����Q�b�g���ꂽ���̏���
void ItemHealing::Function()
{
    Player& player = Player::GetInstance();
    player.Palyer_Healed(m_heal_hp);
}

void ItemHealing::Initialize()
{
    if (!g_Healing_Texture)
    {
        g_Healing_Texture=InitTexture(L"asset\\texture\\sample_texture\\sample_rock.png");
        g_Effect_Texture= InitTexture(L"asset\\texture\\sample_texture\\coin_effect.png");
    }
}

void ItemHealing::Update()
{
    if (m_destory && m_body != nullptr)
    {

        m_effect_sheet_cnt = 1;
        m_effect_pos = m_body->GetPosition();

        //�{�f�B�̏�������
        b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
        world->DestroyBody(m_body);
        m_body = nullptr;

        /*Item_Coin_UI::SetDrawCount(30);
        Item_Coin_UI::SetNowCoinCount(Item_Coin_UI::GetNowCoinCount() + 1);*/
        //app_atomex_start(Player_Coin_Colect_Sound);
    }
}

void ItemHealing::Draw()
{
    if (m_body != nullptr)
    {
        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Healing_Texture);

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
            { GetSize().x * scale * 1.5f,GetSize().y * scale * 1.5f },
            3
        );

        if (60 < m_effect_start_cnt)
        {

        }

        m_effect_start_cnt++;
    }
    //�G�t�F�N�g�̌Ăяo��
    DrawEffect();
}

void ItemHealing::Finalize()
{
    if (GetBody() != nullptr)
    {
        //���[���h�̃C���X�^���X�������Ă���
        Box2dWorld& box2d_world = Box2dWorld::GetInstance();
        b2World* world = box2d_world.GetBox2dWorldPointer();
        world->DestroyBody(GetBody());
        SetBody(nullptr);
    }

    if (g_Healing_Texture)
    {
        UnInitTexture(g_Healing_Texture);
        UnInitTexture(g_Effect_Texture);
    }
}

void ItemHealing::DrawEffect()
{
    if (m_effect_sheet_cnt != 0)
    {
        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Effect_Texture);

        // �R���C�_�[�ƈʒu���̕␳�����邽��
        float scale = SCREEN_SCALE;

        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;


        // �R���C�_�[�̈ʒu�̎擾�i�A�C�e���[�̈ʒu�j




        // �v���C���[�ʒu���l�����ăX�N���[���␳��������
        //�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
        float draw_x = ((m_effect_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((m_effect_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


        //�`��
        DrawSplittingSprite(
            { draw_x,
             draw_y },
            0.0,
            { GetSize().x * scale * 2.5f,GetSize().y * scale * 2.5f },
            4, 3,
            m_effect_sheet_cnt / 4,
            3.0
        );


        m_effect_sheet_cnt++;

        if (48 <= m_effect_sheet_cnt)
        {
            m_effect_sheet_cnt = 0;
            m_effect_start_cnt = 0;
        }

    }
}