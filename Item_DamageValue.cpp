//-----------------------------------------------------------------------------------------------------
// #name Item_DamageValue.h
// #description �_���[�W�\�L
// #make 2025/2/27�@���j�S
// #update 2025/2/27
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------
#include "Item_DamageValue.h"
#include "world_box2d.h"
#include "texture.h"
#include "sprite.h"
#include "collider_type.h"
#include "player_position.h"



ItemDamageValue::ItemDamageValue(b2Vec2 position, b2Vec2 body_size, float angle, DamageOwnerType owner, int damage_value, float Alpha)
    :m_size(body_size), m_Alpha(Alpha), m_owner(owner), m_damage_value(damage_value)
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


    pattern_ID.push_back(m_damage_value / 1000);
    pattern_ID.push_back(m_damage_value / 100);
    pattern_ID.push_back(m_damage_value / 10 % 10);
    pattern_ID.push_back(m_damage_value % 10);
    while (pattern_ID.at(0) == 0)
    {
        pattern_ID.erase(pattern_ID.begin());
    }

    
    if (!m_enemy_damage_Texture) { m_enemy_damage_Texture = InitTexture(L"asset\\texture\\UI_damage_value\\UI_enemy_damage.png"); }
    if (!m_player_damage_Texture) { m_player_damage_Texture = InitTexture(L"asset\\texture\\UI_damage_value\\UI_player_damage.png"); }

    switch (m_owner)
    {
    case DamageOwnerType_enemy:
        m_texture_total_cols = 10;
        m_texture_total_rows = 1;
        break;
    case DamageOwnerType_player:
        m_texture_total_cols = 11;
        m_texture_total_rows = 1;
        pattern_ID.insert(pattern_ID.begin(), 10);  //�v���C���[�̃_���[�W�\�L�ɂ����Ȃ��u-�v�����邽��
        break;
    }
}



ItemDamageValue::~ItemDamageValue()
{
    if (m_enemy_damage_Texture) UnInitTexture(m_enemy_damage_Texture);
    if (m_player_damage_Texture) UnInitTexture(m_player_damage_Texture);
}



void ItemDamageValue::Initialize()
{
}

void ItemDamageValue::Update()
{
    if (m_body)
    {
        if (time_count > 0)
        {
            b2Vec2 current_position = m_body->GetPosition();
            m_body->SetTransform(b2Vec2{ current_position.x, current_position.y - 0.01f }, true);
            time_count--;
        }
        else
        {
            //���[���h�̃C���X�^���X�������Ă���
            Box2dWorld& box2d_world = Box2dWorld::GetInstance();
            b2World* world = box2d_world.GetBox2dWorldPointer();
            world->DestroyBody(GetBody());
            SetBody(nullptr);
        }
    }
}

void ItemDamageValue::Draw()
{
    if (m_body)
    {
        for (int i = 0; i < pattern_ID.size(); i++)
        {
            // �R���C�_�[�ƈʒu���̕␳�����邽��
            float scale = SCREEN_SCALE;

            b2Vec2 screen_center;
            screen_center.x = SCREEN_CENTER_X;
            screen_center.y = SCREEN_CENTER_Y;


            // �R���C�_�[�̈ʒu�̎擾�i�A�C�e���[�̈ʒu�j
            b2Vec2 position;
            position.x = m_body->GetPosition().x;
            position.y = m_body->GetPosition().y;

            //  iii
            // �v���C���[�ʒu���l�����ăX�N���[���␳��������
            //�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
            float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x + (GetSize().x*(i+1)*scale)/2;
            float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

            // �V�F�[�_���\�[�X��ݒ�
            switch (m_owner)
            {
            case DamageOwnerType_enemy:
                GetDeviceContext()->PSSetShaderResources(0, 1, &m_enemy_damage_Texture);
                break;
            case DamageOwnerType_player:
                GetDeviceContext()->PSSetShaderResources(0, 1, &m_player_damage_Texture);
                break;
            }

            DrawSplittingSprite(
                { draw_x,
                draw_y },
                0.0f,
                { GetSize().x * scale, GetSize().y * scale },
                m_texture_total_cols, m_texture_total_rows,
                pattern_ID.at(i),
                4.0f
            );

        }
    }

}

void ItemDamageValue::Finalize()
{
}
