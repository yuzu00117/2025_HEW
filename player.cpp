//-----------------------------------------------------------------------------------------------------
// #name player.cpp
// #description プレイヤー
// #make 2024/11/19
// #update 2024/11/03
// #comment 追加・修正予定
//          ・コンストラクタでbodyとfixture作ってInitializeでつくる
//----------------------------------------------------------------------------------------------------




#include"include/box2d/box2d.h"
#include"player.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"collider_type.h"
#include"anchor_point.h"

//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_player_Texture=NULL;


//センサーの画像
ID3D11ShaderResourceView* g_player_sensor_Texture=NULL;



//プレーヤーのポインターをNULLに
Player *player=nullptr;

Player::Player(b2Vec2 position, b2Vec2 body_size,b2Vec2 sensor_size) :m_body(nullptr)
{

    b2BodyDef body;
    body.type = b2_dynamicBody;
    body.position.Set(position.x , position.y);
    body.angle = 0.0f;
    body.fixedRotation = true;//回転を固定にする
    body.userData.pointer = (uintptr_t)this;


    

    Box2dWorld& box2d_world = Box2dWorld::GetInstance();
    b2World* world = box2d_world.GetBox2dWorldPointer();



    m_body = world->CreateBody(&body);

   
    SetSize(body_size);//プレイヤー表示をするためにセットする
    SetSensorSize(sensor_size);//センサー表示をするためにセット


    
 
    b2Vec2 size;
    size.x = body_size.x/BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
    size.y = body_size.y/BOX2D_SCALE_MANAGEMENT;


    //センサーの設定用の
    b2Vec2 size_sensor;//命名すまん
    size_sensor.x=sensor_size.x / BOX2D_SCALE_MANAGEMENT;
    size_sensor.y=sensor_size.y / BOX2D_SCALE_MANAGEMENT;


    b2PolygonShape shape;
    shape.SetAsBox(size.x * 0.5 , size.y * 0.5f );

    

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;//密度
    fixture.friction = 0.05f;//摩擦
    fixture.restitution = 0.1f;//反発係数
    fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える

  

   
    b2Fixture* player_fixture =m_body->CreateFixture(&fixture);

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
    ObjectData* playerdata = new ObjectData{collider_player};
    player_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);

  
    //--------------------------------------------------------------------------------------------------
    
    //プレイヤーのセンサーを新しくつくる

    b2PolygonShape shape_sensor;
    shape_sensor.SetAsBox(size_sensor.x * 0.5, size_sensor.y * 0.5);



    b2FixtureDef fixture_sensor;
    fixture_sensor.shape = &shape_sensor;
    fixture_sensor.density = 0.0f;//密度
    fixture_sensor.friction = 0.0f;//摩擦
    fixture_sensor.restitution = 0.0f;//反発係数
    fixture_sensor.isSensor = true;//センサーかどうか、trueならあたり判定は消える
 
    b2Fixture* player_sensor_fixture = m_body->CreateFixture(&fixture_sensor);


    // カスタムデータを作成して設定
   // プレイヤーに値を登録
   // プレーヤーにユーザーデータを登録
    ObjectData* player_sensor_data = new ObjectData{ collider_player_sensor };
    player_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(player_sensor_data);

    //---------------------------------------------------------------------------------------------------
}


Player::~Player()
{
    ////テクスチャの削除
    //UnInitTexture(g_player_Texture);
}

void Player::Initialize()
{
  
    //テクスチャのロード
    g_player_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");

    g_player_sensor_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");


    

}

void Player::Update()
{
    // プレイヤーの更新処理


    //コントローラーの入力の受け取り
    ControllerState state = GetControllerInput();

    //右移動
    if (Keyboard_IsKeyDown(KK_RIGHT))
    {
        m_body->ApplyForceToCenter(b2Vec2(0.1, 0.0), true);
    }

    //左移動
    if (Keyboard_IsKeyDown(KK_LEFT))
    {
        m_body->ApplyForceToCenter(b2Vec2(-0.1, 0.0), true);
    }


    //コントローラーでの受け取り 横移動
    m_body ->ApplyForceToCenter(b2Vec2(state.leftStickX / 20000, 0.0), true);


    //ジャンプチェック
    if (m_can_jump && (Keyboard_IsKeyDown(KK_UP) || (state.buttonA)))
    {

        m_body->ApplyLinearImpulseToCenter(m_jump_force, true);

    }


    //スティックの値を受け取って正規化する
    float stick_x= state.rightStickX / 32768.0f;
    float stick_y= state.rightStickY / 32768.0f;

    //絶対値に変更する デットゾーンの審査に使うため　tool.cppに作った
    //デットゾーンをつくる x,yの値を足して一定以上経ったら　呼び出し
    if (0.98 < ReturnAbsoluteValue(stick_x) + ReturnAbsoluteValue(stick_y))
    {
        AnchorPoint::SelectAnchorPoint(stick_x, stick_y);
    }

    //プレイヤーポジションCPPの関数にデータをセット
    PlayerPostion::SetPlayerPostion(m_body->GetPosition());
}

void Player::Draw()
{

    // コライダーと位置情報の補正をするため
    float scale = SCREEN_SCALE;

    // スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
    b2Vec2 screen_center;
    screen_center.x = SCREEN_CENTER_X;
    screen_center.y = SCREEN_CENTER_Y;

    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_Texture);

    // コライダーの位置の取得（プレイヤーの位置）
    b2Vec2 player_position;
    player_position.x = m_body->GetPosition().x;
    player_position.y = m_body->GetPosition().y;


    //プレイヤーは画面中央に固定、まあ補正値はいれるんやけど
    //ほかのplayer以外の物体はプレイヤー分Positionを引いて
   // 描画位置を調整して、プレイヤーがスクリーン中央に表示されるようにする
    DrawSprite(
        { screen_center.x,
          screen_center.y },
        m_body->GetAngle(),
        {GetSize().x* scale,GetSize().y * scale }
    );

    //----------------------------------------------------------------------------------------
    //センサー描画


    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_sensor_Texture);

    DrawSprite(
        { screen_center.x,
          screen_center.y },
        m_body->GetAngle(),
        { GetSensorSize().x * scale,GetSensorSize().y * scale }
    );
    float size_sensor = GetSensorSize().x * scale;
    float size = GetSize().x * scale;
  
}

void Player::Finalize()
{
    if (m_body) {
        // ボディを削除
        Box2dWorld& box2d_world = Box2dWorld::GetInstance();
        b2World* world = box2d_world.GetBox2dWorldPointer();
        world->DestroyBody(m_body);
        m_body = nullptr;
    }
    if (g_player_Texture != nullptr)
    {
        UnInitTexture(g_player_Texture);
    }
}