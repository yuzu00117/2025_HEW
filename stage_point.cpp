//-----------------------------------------------------------------------------------------------------
// #name stage_point.cpp
// #description ステージセレクトをする際に使用する、ステージのポイントのあたり判定のボディを作ってる
// #make 2024/12/27　　永野義也
// #update 2024/12/27
// #comment 追加・修正予定
//                      ・特になし
//           
//----------------------------------------------------------------------------------------------------
#include "stage_point.h"
#include "texture.h"
#include "main.h"
#include"collider_type.h"
#include"stage_select_player.h"

constexpr float SCALE = 30.0f; // ピクセルからメートルへの変換スケール

//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_stage_select_stage_point_tutorial_Texture = NULL;
ID3D11ShaderResourceView* g_stage_select_stage_point_1_1_Texture = NULL;
ID3D11ShaderResourceView* g_stage_select_stage_point_unknow_Texture = NULL;
ID3D11ShaderResourceView* g_stage_select_stage_point_effect_Texture = NULL;




StagePoint::StagePoint()
    : m_body(nullptr), m_size(20.0f), m_world(nullptr) {}

StagePoint::~StagePoint() {
 
}

void StagePoint::Initialize(b2World* world, float x, float y, float size,int check_id) {
    m_world = world;
    m_size = size;

    // テクスチャをロード
    g_stage_select_stage_point_tutorial_Texture = InitTexture(L"asset\\texture\\stage_select_texture\\stage_point_tutorial.png");
    g_stage_select_stage_point_1_1_Texture=InitTexture(L"asset\\texture\\stage_select_texture\\stage_point_1_1.png");
    g_stage_select_stage_point_unknow_Texture= InitTexture(L"asset\\texture\\stage_select_texture\\stage_point_unknow.png");
    g_stage_select_stage_point_effect_Texture = InitTexture(L"asset\\texture\\stage_select_texture\\stage_point_effect.png");

    // Box2Dのボディ定義
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody; // 静的ボディ
    bodyDef.position.Set(x / SCALE, y / SCALE); // 初期位置
    m_body = m_world->CreateBody(&bodyDef);

    // ボディの形状とフィクスチャ
    b2PolygonShape shape;
    shape.SetAsBox(m_size / 2/SCALE, m_size  /2/ SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true; // 衝突判定専用
    b2Fixture* stage_point_fixture = m_body->CreateFixture(&fixtureDef);

    ObjectData* stage_point_data = new ObjectData{collider_stage_select_point };
    stage_point_data->id = check_id;
    stage_point_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(stage_point_data);

    id = check_id;

    
}

void StagePoint::Update() {
    // 必要であれば更新処理を追加
}

void StagePoint::Draw() {
    if (!m_body || !g_stage_select_stage_point_tutorial_Texture) return;

    b2Vec2 position = m_body->GetPosition();
    float x = position.x * SCALE; // メートルからピクセルに変換
    float y = position.y * SCALE;


 //---------------------------------------------------------------------------------------------
 // フィールドポイントのエフェクト



    float SCALE = 1;

    StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
    if (0 != m_player.GetTouchStageSelectNum())
    {
        if (id == m_player.GetTouchStageSelectNum())
        {
            SCALE = 1.5;
        }
    }

    int select_flag = 1;
    if (SCALE != 1)
    {
        select_flag = 2;
    }

    //テクスチャを描画
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_stage_point_effect_Texture);

   
    DrawDividedSprite(XMFLOAT2(x, y+30), 0.0f, XMFLOAT2(210*SCALE, 140*SCALE), 5, 3, effect_cnt / 4*select_flag);

    effect_cnt++;

    if (60 < effect_cnt)
    {
        effect_cnt = 0;
    }



    switch (id)
    {
    case 0:
        // テクスチャを描画
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_stage_point_unknow_Texture);
        break;
    case 1:
        // テクスチャを描画
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_stage_point_tutorial_Texture);
        break;
    case 2:
        // テクスチャを描画
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_stage_point_1_1_Texture);
        break;
    case 3:
        // テクスチャを描画
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_stage_point_unknow_Texture);
        break;
    case 4:
        // テクスチャを描画
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_stage_point_unknow_Texture);
        break;
    default:
        break;
    }

 

    DrawSpriteOld(
        XMFLOAT2(x, y),
        0.0f,
        XMFLOAT2(m_size*SCALE, m_size*SCALE)
    );


 

}

bool StagePoint::IsPlayerColliding(b2Body* playerBody) {
    if (!m_body || !playerBody) return false;

    for (b2ContactEdge* edge = playerBody->GetContactList(); edge; edge = edge->next) {
        if (edge->contact->IsTouching() &&
            (edge->contact->GetFixtureA()->GetBody() == m_body ||
                edge->contact->GetFixtureB()->GetBody() == m_body)) {
            return true;
        }
    }
    return false;
}

void StagePoint::Finalize() {
    

    
    if (m_world && m_body) {
        m_world->DestroyBody(m_body);
        m_body = nullptr;
    }
    
    if (g_stage_select_stage_point_tutorial_Texture) UnInitTexture(g_stage_select_stage_point_tutorial_Texture);
    if (g_stage_select_stage_point_1_1_Texture) UnInitTexture(g_stage_select_stage_point_1_1_Texture);
    if (g_stage_select_stage_point_unknow_Texture) UnInitTexture(g_stage_select_stage_point_unknow_Texture);
    if (g_stage_select_stage_point_effect_Texture) UnInitTexture(g_stage_select_stage_point_effect_Texture);

    
}