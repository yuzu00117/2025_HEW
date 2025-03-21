//-----------------------------------------------------------------------------------------------------
// #name Item_Coin.cpp
// #description     coinアイテム
// #make 2024/12/28　永野義也
// #update 2024/12/28
// #comment 追加・修正予定

//----------------------------------------------------------------------------------------------------

#include"Item_Coin.h"
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
#include"game.h"

static ID3D11ShaderResourceView* g_Texture = NULL;//コインのテクスチャ
static ID3D11ShaderResourceView* g_coin_effect = NULL;//コインのテクスチャ


ItemCoin::ItemCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
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
        fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える

        p_fixture = m_body->CreateFixture(&fixture);
    }

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // ユニークポインターを使って ObjectData を作成
    m_objectData = std::make_unique<ObjectData>(collider_item);
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());



    m_objectData->Item_name = ITEM_COIN;
    int ID = m_objectData->GenerateID();
    m_objectData->id = ID;
    SetID(ID);


}

void	ItemCoin::Update()
{
    if (m_destory && m_body != nullptr)
    {
        if (!m_body) return;

        for (b2Fixture* fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
            if (!fixture) continue;

            // UserData 取得


            // 無効なポインタならスキップ
            if (!fixture->GetUserData().pointer) {
                continue;
            }


            // ObjectData を削除す
            fixture->GetUserData().pointer = 0;  // ポインタのクリア
        }
        coin_effect_sheet_cnt = 1;
        coin_effect_pos = m_body->GetPosition();

        //ボディの情報を消す
        b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
        world->DestroyBody(m_body);
        m_body = nullptr;

        Item_Coin_UI::SetDrawCount(30);
        Item_Coin_UI::SetNowCoinCount(Item_Coin_UI::GetNowCoinCount()+1);
        app_atomex_start(Player_Coin_Colect_Sound);


    }
}

void    ItemCoin::Function()
{

}


void ItemCoin::Initialize()
{
    m_destory = false;
    coin_effect_sheet_cnt = 0;
    coin_effect_start_cnt = 0;

    Game& game = Game::GetInstance();
    if (game.GetCurrentGameState() == GAME_STATE_PAUSE_RESPAWN_INITIAL)
    {
        m_registered_to_save_point = false;
    }

    if (g_Texture == NULL)
    {
        g_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_coin.png");
        g_coin_effect = InitTexture(L"asset\\texture\\sample_texture\\coin_effect.png");
    }
}



void ItemCoin::Draw()
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
            { GetSize().x * scale*1.5f,GetSize().y * scale*1.5f },
            m_Alpha
        );

        if (60 < coin_effect_start_cnt)
        {
          
        }

        coin_effect_start_cnt++;
    }
    //エフェクトの呼び出し
    DrawEffect();
}

void ItemCoin::DrawEffect()
{
  
    if (coin_effect_sheet_cnt!=0)
    {
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_coin_effect);

        // コライダーと位置情報の補正をするため
        float scale = SCREEN_SCALE;

        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;


        // コライダーの位置の取得（アイテムーの位置）




        // プレイヤー位置を考慮してスクロール補正を加える
        //取得したbodyのポジションに対してBox2dスケールの補正を加える
        float draw_x = ((coin_effect_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((coin_effect_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


        //描画
        DrawSplittingSprite(
            { draw_x,
             draw_y },
            0.0,
            { GetSize().x * scale * 2.5f,GetSize().y * scale * 2.5f },
            4, 3,
            coin_effect_sheet_cnt / 4,
            3.0
        );


        coin_effect_sheet_cnt++;

        if (48 <= coin_effect_sheet_cnt)
        {
            coin_effect_sheet_cnt = 0;
            coin_effect_start_cnt = 0;
        }

    }
       
    

}


void ItemCoin::Finalize()
{

    if (GetBody() != nullptr)
    {

        if (!m_body) return;

        for (b2Fixture* fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
            if (!fixture) continue;

            // UserData 取得


            // 無効なポインタならスキップ
            if (!fixture->GetUserData().pointer) {
                continue;
            }




            // ObjectData を削除す
            fixture->GetUserData().pointer = 0;  // ポインタのクリア
        }
        //ワールドのインスタンスを持ってくる
        Box2dWorld& box2d_world = Box2dWorld::GetInstance();
        b2World* world = box2d_world.GetBox2dWorldPointer();
        world->DestroyBody(GetBody());
        SetBody(nullptr);
    }

    if (g_Texture) UnInitTexture(g_Texture);
    if (g_coin_effect) UnInitTexture(g_coin_effect);

}

ItemCoin::~ItemCoin()
{
    Finalize();
}

void ItemCoin::CreateBody()
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
        fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える

        p_fixture = m_body->CreateFixture(&fixture);
    }

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
    ObjectData* data = new ObjectData{ collider_item };
    p_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);


    data->Item_name = ITEM_COIN;
    int ID = data->GenerateID();
    data->id = ID;
    SetID(ID);

}

