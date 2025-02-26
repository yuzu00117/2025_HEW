//-----------------------------------------------------------------------------------------------------
// #name Item_Barrier.cpp
// #description バリア
// #make 2025/2/26　王泳心
// #update 2025/2/26
// #comment 追加・修正予定
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


static ID3D11ShaderResourceView* g_Start_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Loop_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Break_Texture = NULL;//アンカーのテクスチャ


ItemBarrier::ItemBarrier(b2Vec2 position, b2Vec2 body_size, float angle, const b2Body* owner_body, float Alpha)
    :m_size(body_size), m_Alpha(Alpha), m_owner(owner_body)
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

        //バリア張るまで時間空いてから描画させたい
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
                //ワールドのインスタンスを持ってくる
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
        //バリア張るまで時間空いてから描画させたい
        if (m_effect_cnt < 0)
        {
            return;
        }

        // コライダーと位置情報の補正をするため
        float scale = SCREEN_SCALE;

        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;


        // コライダーの位置の取得（アイテムーの位置）
        b2Vec2 position;
        position.x = m_body->GetPosition().x;
        position.y = m_body->GetPosition().y;


        // プレイヤー位置を考慮してスクロール補正を加える
        //取得したbodyのポジションに対してBox2dスケールの補正を加える
        float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


        switch (m_state)
        {
        case Barrier_Start:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Start_Texture);
            break;
        case Barrier_Loop:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Loop_Texture);
            break;
        case Barrier_Break:
            // シェーダリソースを設定
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
