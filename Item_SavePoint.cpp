//-----------------------------------------------------------------------------------------------------
// #name Item_SavePoint.cpp
// #description セーブポイント
// #make 2025/2/04　王泳心
// #update 2025/2/04
// #comment 追加・修正予定
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

static ID3D11ShaderResourceView* g_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_get_save_point_effect = NULL;//セーブポイントを取得した時のエフェクト


ItemSavePoint::ItemSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
    :m_body_position(position), m_size(body_size), m_angle(angle), m_shape_polygon(shape_polygon), m_Alpha(Alpha)
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


    b2FixtureDef fixture;
    b2Fixture* p_fixture;


    //四角形の場合
   //-----------------------------------------------------------------------------
    if (shape_polygon)
    {
        b2PolygonShape shape;
        shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);

        fixture.shape = &shape;
        fixture.density = 1.0f;//密度
        fixture.friction = 0.3f;//摩擦
        fixture.restitution = 0.1f;//反発係数
        fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える

        p_fixture = m_body->CreateFixture(&fixture);

    }
    //円の場合
//-----------------------------------------------------------------------------
    else
    {
        b2CircleShape shape;
        shape.m_radius = size.x * 0.5f;

        fixture.shape = &shape;
        fixture.density = 1.0f;//密度
        fixture.friction = 0.3f;//摩擦
        fixture.restitution = 0.1f;//反発係数
        fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える

        p_fixture = m_body->CreateFixture(&fixture);
    }

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
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
        //体力がまだマックスじゃない
        AnchorSpirit::SetAnchorSpiritValueDirectly(100);    //アンカーをlevel２にセット
    }
    //プレイヤーのリスポン位置を更新する
    Player& player = Player::GetInstance();
    player.RegisterSavePoint(this);
    
    //今の豪快値を豪快UIに記録
    int value = Gokai_UI::GetNowGokaiCount();
    Gokai_UI::SetGokai_WhenRespawn(value);

    //初回通過時の効果音
    app_atomex_start(Player_Coin_Colect_Sound);


    effect_cnt = 1;
}


void ItemSavePoint::Initialize()
{
    if (g_Texture == NULL)
    {
        g_Texture = InitTexture(L"asset\\texture\\Item_texture\\SavePoint.png");
        g_get_save_point_effect = InitTexture(L"asset\\texture\\stage_1_1_object\\get_save_point_effect.png");//取得した時のエフェクト
    }

}



void ItemSavePoint::Draw()
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
        //ワールドのインスタンスを持ってくる
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
    body.fixedRotation = true;//回転を固定にする
    body.userData.pointer = (uintptr_t)this;


    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    m_body = world->CreateBody(&body);

    SetSize(m_size);//プレイヤー表示をするためにセットする


    b2Vec2 size;
    size.x = m_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
    size.y = m_size.y / BOX2D_SCALE_MANAGEMENT;


    b2FixtureDef fixture;
    b2Fixture* p_fixture;


    //四角形の場合
   //-----------------------------------------------------------------------------
    if (m_shape_polygon)
    {
        b2PolygonShape shape;
        shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);

        fixture.shape = &shape;
        fixture.density = 1.0f;//密度
        fixture.friction = 0.3f;//摩擦
        fixture.restitution = 0.1f;//反発係数
        fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える

        p_fixture = m_body->CreateFixture(&fixture);

    }
    //円の場合
//-----------------------------------------------------------------------------
    else
    {
        b2CircleShape shape;
        shape.m_radius = size.x * 0.5f;

        fixture.shape = &shape;
        fixture.density = 1.0f;//密度
        fixture.friction = 0.3f;//摩擦
        fixture.restitution = 0.1f;//反発係数
        fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える

        p_fixture = m_body->CreateFixture(&fixture);
    }

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
    ObjectData* data = new ObjectData{ collider_item };
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);


    data->Item_name = ITEM_SAVEPOINT;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);

}


