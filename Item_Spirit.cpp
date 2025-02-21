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
#include "anchor_spirit.h"
#include"create_filter.h"
#include"player.h"


static ID3D11ShaderResourceView* g_Texture = NULL;//テクスチャ

int count_anim_time = 0;



ItemSpirit::ItemSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, float Alpha)
    :m_size(body_size), m_Alpha(Alpha), m_type(type)
{

    if (g_Texture == NULL)
    {
        switch (m_type)
        {
        case Spirit_L:
            g_Texture = InitTexture(L"asset\\texture\\soul_texture\\EFF_Soul_L.png");
            break;
        case Spirit_M:
            g_Texture = InitTexture(L"asset\\texture\\soul_texture\\EFF_Soul_M.png");
            break;
        case Spirit_S:
            g_Texture = InitTexture(L"asset\\texture\\soul_texture\\EFF_Soul_S.png");
            break;
        }
    }

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

    Player& player = Player::GetInstance();
    float recovery = 0;
    switch (m_type)
    {
    case Spirit_L:
        recovery = 100;
        player.SetSoulGetEffectType(3);
        break;
    case Spirit_M:
        recovery = 50;
        player.SetSoulGetEffectType(2);
        break;
    case Spirit_S:
        recovery = 25;
        player.SetSoulGetEffectType(1);
        break;
    }


    AnchorSpirit::EditAnchorSpiritValue(recovery);
}



void ItemSpirit::Initialize()
{

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

        float sprit_scale = 1;

        switch (m_type)
        {
        case Spirit_L:
            sprit_scale = 4.0f;
            break;
        case Spirit_M:
            sprit_scale = 2.5f;
            break;
        case Spirit_S:
            sprit_scale = 1.2f;
            break;
        default:
            break;
        }


        DrawSplittingSprite(
            { draw_x,
            draw_y },
            m_body->GetAngle(),
            { GetSize().x * scale* sprit_scale * 0.75f,GetSize().y * scale * sprit_scale },
            6, 4, m_anim_id,2.0f
        );
        
        count_anim_time++;
        if (count_anim_time > 60)
        {
            m_anim_id++;
            m_anim_id = m_anim_id % 24;
        }


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
