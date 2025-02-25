//-----------------------------------------------------------------------------------------------------
// #name Item_SavePoint.cpp
// #description �Z�[�u�|�C���g
// #make 2025/2/04�@���j�S
// #update 2025/2/04
// #comment �ǉ��E�C���\��
//          
//			
//----------------------------------------------------------------------------------------------------
#include "Item_SavePoint.h"
#include "world_box2d.h"
#include "collider_type.h"
#include "texture.h"
#include "sprite.h"
#include "player_position.h"
#include "anchor_spirit.h"
#include "player_stamina.h"
#include "player.h"
#include "sound.h"
#include "gokai.h"

static ID3D11ShaderResourceView* g_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_get_save_point_effect = NULL;//�Z�[�u�|�C���g���擾�������̃G�t�F�N�g


ItemSavePoint::ItemSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
    :m_body_position(position), m_size(body_size), m_angle(angle), m_shape_polygon(shape_polygon), m_Alpha(Alpha)
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


    data->Item_name = ITEM_SAVEPOINT;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);


}

void	ItemSavePoint::Update()
{

}

void    ItemSavePoint::Function()
{
    int level = AnchorSpirit::GetAnchorLevel();
    if (level < 2) {

        float stamina = PlayerStamina::GetPlayerStaminaValue();
        //�̗͂��܂��}�b�N�X����Ȃ�
        AnchorSpirit::SetAnchorSpiritValueDirectly(100);    //�A���J�[��level�Q�ɃZ�b�g
    }
    //�v���C���[�̃��X�|���ʒu���X�V����
    Player& player = Player::GetInstance();
    player.RegisterSavePoint(this);
    
    //���̍����l������UI�ɋL�^
    int value = Gokai_UI::GetNowGokaiCount();
    Gokai_UI::SetGokai_WhenRespawn(value);

    //����ʉߎ��̌��ʉ�
    app_atomex_start(Player_Coin_Colect_Sound);


    effect_cnt = 1;
}


void ItemSavePoint::Initialize()
{
    if (g_Texture == NULL)
    {
        g_Texture = InitTexture(L"asset\\texture\\Item_texture\\SavePoint.png");
        g_get_save_point_effect = InitTexture(L"asset\\texture\\stage_1_1_object\\get_save_point_effect.png");//�擾�������̃G�t�F�N�g
    }

}



void ItemSavePoint::Draw()
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
            { GetSize().x * scale * 1.5f,GetSize().y * scale * 1.5f },
            m_Alpha
        );



        if (effect_cnt != 0)
        {
            draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x - (GetSize().x * scale * 1.5f) *0.35f ;
            draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y + (GetSize().y * scale * 1.5f) * 0.1f;

            GetDeviceContext()->PSSetShaderResources(0, 1, &g_get_save_point_effect);

            DrawSplittingSprite(
                { draw_x,
                draw_y },
                0.0f,
                {GetSize().x * scale * 1.0f  ,GetSize().y * scale * 1.0f },
                6, 4,
               effect_cnt ,
                2.0f
            );

            effect_cnt += 0.3;


            if (36 < effect_cnt)
            {
                effect_cnt = 1;
            }

        }
    }
}


void ItemSavePoint::Finalize()
{

    if (GetBody() != nullptr)
    {
        //���[���h�̃C���X�^���X�������Ă���
        Box2dWorld& box2d_world = Box2dWorld::GetInstance();
        b2World* world = box2d_world.GetBox2dWorldPointer();
        world->DestroyBody(GetBody());
        SetBody(nullptr);
    }

    if (g_Texture) UnInitTexture(g_Texture);
    if (g_get_save_point_effect) UnInitTexture(g_get_save_point_effect);

}

ItemSavePoint::~ItemSavePoint()
{
}

void ItemSavePoint::CreateBody()
{
    b2BodyDef body;
    body.type = b2_staticBody;
    body.position.Set(m_body_position.x, m_body_position.y);
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


    //�l�p�`�̏ꍇ
   //-----------------------------------------------------------------------------
    if (m_shape_polygon)
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


    data->Item_name = ITEM_SAVEPOINT;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);

}


