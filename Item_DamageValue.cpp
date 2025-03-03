//-----------------------------------------------------------------------------------------------------
// #name Item_DamageValue.h
// #description ダメージ表記
// #make 2025/2/27　王泳心
// #update 2025/2/27
// #comment 追加・修正予定
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
    body.fixedRotation = true;//回転を固定にする
    body.userData.pointer = (uintptr_t)this;


    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    m_body = world->CreateBody(&body);

    SetSize(body_size);//プレイヤー表示をするためにセットする


    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
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
        pattern_ID.insert(pattern_ID.begin(), 10);  //プレイヤーのダメージ表記にしかない「-」を入れるため
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
            //ワールドのインスタンスを持ってくる
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
            // コライダーと位置情報の補正をするため
            float scale = SCREEN_SCALE;

            b2Vec2 screen_center;
            screen_center.x = SCREEN_CENTER_X;
            screen_center.y = SCREEN_CENTER_Y;


            // コライダーの位置の取得（アイテムーの位置）
            b2Vec2 position;
            position.x = m_body->GetPosition().x;
            position.y = m_body->GetPosition().y;

            //  iii
            // プレイヤー位置を考慮してスクロール補正を加える
            //取得したbodyのポジションに対してBox2dスケールの補正を加える
            float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x + (GetSize().x*(i+1)*scale)/2;
            float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

            // シェーダリソースを設定
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
