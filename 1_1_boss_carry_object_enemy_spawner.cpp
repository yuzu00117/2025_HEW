//-----------------------------------------------------------------------------------------------------
// #name boss_carry_object_enemy_spawner.cpp
// #description　ボスの上空をプカプカしてるエネミーオブジェクトを持ってて落とせるエネミーのスポナー
// #make 2025/02/01　永野義也
// #update 2025/02/01
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------



#include"texture.h"
#include"sprite.h"
#include"1_1_boss_carry_object_enemy_spawner.h"
#include"1-1_boss_field_block.h"
#include"world_box2d.h"
#include"create_filter.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"object_manager.h"



static ID3D11ShaderResourceView* g_Texture = NULL;//地面のテクスチャ
boss_carry_object_spawner::boss_carry_object_spawner(b2Vec2 position, b2Vec2 Size, Boss_Room_Level level,bool left)
{
    
    SetSize(Size);

    //大きさを補正

    b2Vec2 size;
    size.x = Size.x / BOX2D_SCALE_MANAGEMENT;
    size.y = Size.y / BOX2D_SCALE_MANAGEMENT;

    // ワールドのインスタンスを取得
    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();

    // スポナーのボディを作成
    b2BodyDef body;
    body.type = b2_staticBody;
    body.position.Set(position.x, position.y);
    body.fixedRotation = false;

    m_body = world->CreateBody(&body);

    // 形状の定義
    b2PolygonShape shape;
    shape.SetAsBox(size.x / (2.0f * BOX2D_SCALE_MANAGEMENT), size.y / (2.0f * BOX2D_SCALE_MANAGEMENT));

    // フィクスチャの設定
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;
    fixture.friction = 0.01f;
    fixture.restitution = 0.3f;
    fixture.isSensor = true;
    

    m_body->CreateFixture(&fixture);

    // 各種データの保存
    BossRoomLevel = level;

    left_flag = left;
  
}

boss_carry_object_spawner::~boss_carry_object_spawner()
{
    Finalize();
}

void boss_carry_object_spawner::Initialize()
{
    if (g_Texture == NULL)
    {
        g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//グラウンドのテクスチャ
    }
}

void boss_carry_object_spawner::Update()
{
    spawnTimer++;

    // 10秒ごとにエネミーを生成
    if (spawnTimer >= spawnIntervalFrames)
    {
        SpawnEnemy();
        spawnTimer = 0;
    }

 

    enemyList.erase(
        std::remove_if(enemyList.begin(), enemyList.end(), [](boss_carry_object_enemy* enemy) {
            if (enemy->IsDestroyed()) {
                delete enemy;  // メモリ解放
                return true;
            }
            return false;
            }),
        enemyList.end()
    );
    enemyList.shrink_to_fit();

}

void boss_carry_object_spawner::SpawnEnemy()
{
    Boss_1_1& boss = Boss_1_1::GetInstance();
    if (BossRoomLevel == boss.GetBossFieldLevel())
    {
        ObjectManager& objectManager = ObjectManager::GetInstance();
        objectManager.AddBossCarryObjectEnemy(m_body->GetPosition(), b2Vec2(3.0f, 3.0f), left_flag, 1.0, b2Vec2(4.0f, 4.0f), 1, 2);
    }
}

void boss_carry_object_spawner::Draw()
{
 


    if (m_body != nullptr)
    {
        // スケールをかけないとオブジェクトのサイズの表示が小さいから使う
        float scale = SCREEN_SCALE;

        // スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;

        b2Vec2 Pos = GetBody()->GetPosition();


        float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


        //draw
        DrawSprite(
            { draw_x,
              draw_y },
            GetBody()->GetAngle(),
            { GetSize().x * scale ,GetSize().y * scale }
        );
    }
}



void boss_carry_object_spawner::Finalize()
{
    for (auto& enemy : enemyList)
    {
        delete enemy;
        enemy = nullptr;  // ポインタをnullにする
    }
    enemyList.clear();
    enemyList.shrink_to_fit();

    if (g_Texture) {
        UnInitTexture(g_Texture);
    }
}