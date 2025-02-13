//-----------------------------------------------------------------------------------------------------
// #name Item_Spirit.cpp
// #description		ソウル（敵が落とすアイテム）
// #make 2024/12/28　王泳心
// #update 2025/1/10
// #comment 追加・修正予定
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


static ID3D11ShaderResourceView* g_Texture = NULL;//テクスチャ



ItemSpirit::ItemSpirit(b2Vec2 position, b2Vec2 body_size, float angle, float recovery, float Alpha)
    :m_size(body_size), m_Alpha(Alpha), m_recovery(recovery)
{
    b2BodyDef body;
    body.type = b2_dynamicBody;
    body.position.Set(position.x, position.y);
    body.angle = angle;
    body.fixedRotation = true;//回転を固定にする
    body.userData.pointer = (uintptr_t)this;
    body.gravityScale = 0;



    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    m_body = world->CreateBody(&body);
    m_body->SetLinearDamping(100.0f); //落下速度を制限する（値が大きいほどゆっくり）

    SetSize(body_size);//プレイヤー表示をするためにセットする


    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


    b2FixtureDef fixture;
    b2Fixture* p_fixture;

    //四角形当たり判定
   //-----------------------------------------------------------------------------
    b2PolygonShape shape;
    shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);

    fixture.shape = &shape;
    fixture.density = 1.0f;//密度
    fixture.friction = 0.0f;//摩擦
    fixture.restitution = 0.1f;//反発係数
    fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える

    p_fixture = m_body->CreateFixture(&fixture);
    

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
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
        //プレイヤーに回収されてるなら
        if (m_state == Spirit_Collecting)
        {
            b2Vec2 player_position = PlayerPosition::GetPlayerPosition();
            //  プレイヤーへ向かうベクトル
            b2Vec2 vec;
            vec.x = player_position.x - m_body->GetPosition().x;
            vec.y = player_position.y - m_body->GetPosition().y;
            vec.Normalize();

            float speed = 0.5f;

            GetBody()->ApplyLinearImpulseToCenter(b2Vec2(vec.x * speed, vec.y * speed), true);

            //ソウルのサイズが徐々に減る
            if (m_size.x > 0.8f || m_size.y > m_size.x * 2)
            {
                m_size.x -= 0.005f;
                m_size.y -= 0.005f;
            }

        }
        //消される予定ならボディーを消す
        if (m_state == Spirit_Destory)
        {
            //ボディの情報を消す
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
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

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


        //描画
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
        //ワールドのインスタンスを持ってくる
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

}

ItemSpirit::~ItemSpirit()
{
}
