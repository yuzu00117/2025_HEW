//-----------------------------------------------------------------------------------------------------
// #name Item_Barrier.cpp
// #description �o���A
// #make 2025/2/26�@���j�S
// #update 2025/2/26
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------
#include "Item_Barrier.h"
#include "world_box2d.h"
#include "collider_type.h"
#include "texture.h"
#include "sprite.h"
#include "player_position.h"


static ID3D11ShaderResourceView* g_Start_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Loop_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Break_Texture = NULL;//�A���J�[�̃e�N�X�`��


ItemBarrier::ItemBarrier(b2Vec2 position, b2Vec2 body_size, float angle, const b2Body* owner_body, float Alpha)
    :m_size(body_size), m_Alpha(Alpha), m_owner(owner_body)
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

    // �J�X�^���f�[�^���쐬���Đݒ�
    // �v���C���[�ɒl��o�^
    // �v���[���[�Ƀ��[�U�[�f�[�^��o�^
    ObjectData* data = new ObjectData{ collider_none };

    data->Item_name = ITEM_BARRIER;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);

    SetState(Barrier_Start);
    g_Start_Texture = InitTexture(L"asset\\texture\\Item_texture\\EFF_BarrierStart_6x8.png");
    g_Loop_Texture = InitTexture(L"asset\\texture\\Item_texture\\EFF_BarrierLoop_6x8.png");
    g_Break_Texture = InitTexture(L"asset\\texture\\Item_texture\\EFF_BarrierBrake_4x5.png");

    m_effect_cnt = -120.0f;
    

}

ItemBarrier::~ItemBarrier()
{
    if (g_Start_Texture) UnInitTexture(g_Start_Texture);
    if (g_Loop_Texture) UnInitTexture(g_Loop_Texture);
    if (g_Break_Texture) UnInitTexture(g_Break_Texture);
}

void ItemBarrier::Initialize()
{
}

void ItemBarrier::Update()
{
    if (m_body)
    {
        m_body->SetTransform(m_owner->GetPosition(), 0.0f);

        //�o���A����܂Ŏ��ԋ󂢂Ă���`�悳������
        if (m_effect_cnt < 0)
        {
            m_effect_cnt++;
            return;
        }

        switch (m_state)
        {
        case Barrier_Start:
            if (m_effect_cnt >= 48)
            {
                SetState(Barrier_Loop);
                break;
            }
             m_effect_cnt += 0.3f;
            break;
        case Barrier_Loop:
            if (m_effect_cnt >= 48)
            {
                m_effect_cnt = 0;
                break;
            }
            m_effect_cnt += 0.3f;
            break;
        case Barrier_Break:
            if (m_effect_cnt >= 20)
            {
                //���[���h�̃C���X�^���X�������Ă���
                Box2dWorld& box2d_world = Box2dWorld::GetInstance();
                b2World* world = box2d_world.GetBox2dWorldPointer();
                world->DestroyBody(GetBody());
                SetBody(nullptr);
                m_effect_cnt = 0;
            }
            m_effect_cnt += 0.3f;
            break;
        }
    }
}

void ItemBarrier::Draw()
{
    if (m_body)
    {
        //�o���A����܂Ŏ��ԋ󂢂Ă���`�悳������
        if (m_effect_cnt < 0)
        {
            return;
        }

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


        switch (m_state)
        {
        case Barrier_Start:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Start_Texture);
            break;
        case Barrier_Loop:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Loop_Texture);
            break;
        case Barrier_Break:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Break_Texture);
            break;
        }

        DrawSplittingSprite(
            { draw_x,
            draw_y },
            m_body->GetAngle(),
            { GetSize().x * scale,GetSize().y * scale },
            m_texture_total_cols, m_texture_total_rows,
            m_effect_cnt,
            4.0f
        );

    }
}

void ItemBarrier::Finalize()
{
}
