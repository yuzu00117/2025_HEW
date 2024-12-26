#include "stage_select_player.h"
#include "sprite.h"
#include"main.h"
#include"keyboard.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"collider_type.h"

//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_stage_select_player_Texture = NULL;

constexpr float SCALE = 30.0f; // ピクセルからメートルへの変換スケール

StageSelectPlayer::StageSelectPlayer()
    : m_body(nullptr), m_size(30.0f), m_world(nullptr) {}

StageSelectPlayer::~StageSelectPlayer() {
    Finalize();
}

void StageSelectPlayer::Initialize(b2World* world, float x, float y) {

    //画像のインクルード
    g_stage_select_player_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");
    
    
    m_world = world;

    // Box2Dのボディ定義
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / SCALE, y / SCALE); // 初期位置
    m_body = m_world->CreateBody(&bodyDef);

    // ボディの形状とフィクスチャ
    b2PolygonShape shape;
    shape.SetAsBox(m_size / 2.0f / SCALE, m_size / 2.0f / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    b2Fixture*stage_select_player_fixture=m_body->CreateFixture(&fixtureDef);

    ObjectData*stage_select_data= new ObjectData{ collider_stage_select_player };
    stage_select_player_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(stage_select_data);



}

void StageSelectPlayer::Update() {
    if (!m_body) return;


  


    //コントローラーの入力の受け取り
    ControllerState state = GetControllerInput();

    if (!m_body) return;

    // スティックの値を受け取って正規化する
    float left_stick_x = state.leftStickX / 40000.0f;
    float left_stick_y = state.leftStickY / 40000.0f;

    // スティックのデッドゾーンを設定
    const float deadZone = 0.2f; // デッドゾーンの範囲
    if (std::abs(left_stick_x) < deadZone) left_stick_x = 0.0f;
    if (std::abs(left_stick_y) < deadZone) left_stick_y = 0.0f;

    // 移動速度を設定
    const float maxSpeed = 10.0f; // 最大速度
    float velocityX = left_stick_x * maxSpeed;
    float velocityY = left_stick_y * maxSpeed;

    // プレイヤーの速度を設定
    m_body->SetLinearVelocity(b2Vec2(velocityX, -velocityY));

    

    // キー入力またはコントローラー入力でプレイヤーを動かす
    if (Keyboard_IsKeyDown(KK_LEFT)) {
        m_body->SetLinearVelocity(b2Vec2(-10.0f, 0.0f));
    }
    if (Keyboard_IsKeyDown(KK_RIGHT)) {
        m_body->SetLinearVelocity(b2Vec2(10.0f, 0.0f));
    }
    if (Keyboard_IsKeyDown(KK_UP)) {
        m_body->SetLinearVelocity(b2Vec2(0.0f, -10.0f));
    }
    if (Keyboard_IsKeyDown(KK_DOWN)) {
        m_body->SetLinearVelocity(b2Vec2(0.0f, 10.0f));
    }
}

void StageSelectPlayer::Draw() {
    if (!m_body) return;

    b2Vec2 position = m_body->GetPosition();
    float x = position.x * SCALE; // メートルからピクセルに変換
    float y = position.y * SCALE;

    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_player_Texture);

    DrawSpriteOld(
        XMFLOAT2(x, y),
        0.0f,
        XMFLOAT2(m_size, m_size)
    );
}

void StageSelectPlayer::Finalize() {
    if (m_world && m_body) {
        m_world->DestroyBody(m_body);
        m_body = nullptr;
    }
}