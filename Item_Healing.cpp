//----------------------------------------------------------------------------------------------------
// #name Item_Healing.cpp
// #description     回復アイテムソースファイル
// #make 2025/02/25
// #update 2025/02/25 髙橋連
// #comment 追加・修正予定
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

static ID3D11ShaderResourceView* g_Healing_Texture = NULL;//回復アイテムのテクスチャ
static ID3D11ShaderResourceView* g_Effect_Texture = NULL;//エフェクトのテクスチャ

ItemHealing::ItemHealing(b2Vec2 position, b2Vec2 body_size, float angle)
    :m_position(position), m_size(body_size), m_angle(angle)
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

    b2Vec2 size;
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;

    //フィクスチャ=============================================================
    b2FixtureDef fixture;
    b2Fixture* p_fixture;

    b2CircleShape shape;
    shape.m_radius = size.x * 0.5f;

    fixture.shape = &shape;
    fixture.density = 1.0f;//密度
    fixture.friction = 0.3f;//摩擦
    fixture.restitution = 0.1f;//反発係数
    fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える

    p_fixture = m_body->CreateFixture(&fixture);

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
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

    b2CircleShape shape;
    shape.m_radius = size.x * 0.5f;

    fixture.shape = &shape;
    fixture.density = 1.0f;//密度
    fixture.friction = 0.3f;//摩擦
    fixture.restitution = 0.1f;//反発係数
    fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える

     p_fixture = m_body->CreateFixture(&fixture);


    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
    ObjectData* data = new ObjectData{ collider_item };
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);


    data->Item_name = ITEM_HEALING;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);
}

//アイテムがゲットされた時の処理
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

        //ボディの情報を消す
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
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Healing_Texture);

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
            3
        );

        if (60 < m_effect_start_cnt)
        {

        }

        m_effect_start_cnt++;
    }
    //エフェクトの呼び出し
    DrawEffect();
}

void ItemHealing::Finalize()
{
    if (GetBody() != nullptr)
    {
        //ワールドのインスタンスを持ってくる
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
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Effect_Texture);

        // コライダーと位置情報の補正をするため
        float scale = SCREEN_SCALE;

        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;


        // コライダーの位置の取得（アイテムーの位置）




        // プレイヤー位置を考慮してスクロール補正を加える
        //取得したbodyのポジションに対してBox2dスケールの補正を加える
        float draw_x = ((m_effect_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((m_effect_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


        //描画
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