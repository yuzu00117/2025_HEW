#include"player.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include"player_position.h"
#include"collider_type.h"
#include"anchor_point.h"
#include"anchor.h"
#include"anchor_spirit.h"
#include"create_filter.h"

//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_player_Texture=NULL;


//センサーの画像
ID3D11ShaderResourceView* g_player_sensor_Texture=NULL;

//staticメンバー変数の初期化
bool    Player::m_is_jumping = false;
bool    Player::m_jump_pressed = false;
int     Player::m_direction = 1;



b2Body* player_body;


int g_anchor_pulling_number = 0;

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

    player_body = m_body;//プレイヤーのボディをセット

   
    SetSize(body_size);//プレイヤー表示をするためにセットする
    SetSensorSize(sensor_size);//センサー表示をするためにセット


    
 
    b2Vec2 size;
    size.x = body_size.x/BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
    size.y = body_size.y/BOX2D_SCALE_MANAGEMENT;


    //センサーの設定用の
    b2Vec2 size_sensor;//命名すまん
    size_sensor.x=sensor_size.x / BOX2D_SCALE_MANAGEMENT;
    size_sensor.y=sensor_size.y / BOX2D_SCALE_MANAGEMENT;


    //プレイヤーの上の円のコライダー
//-------------------------------------------
    b2CircleShape circle_upper;
    circle_upper.m_p.Set(position.x, position.y);//上の方の円
    circle_upper.m_radius = body_size.x / BOX2D_SCALE_MANAGEMENT * 0.5f;

    b2FixtureDef fixture_circle_upper;
    fixture_circle_upper.shape = &circle_upper;
    fixture_circle_upper.density = 1.3f;
    fixture_circle_upper.friction = 3.0f;//摩擦
    fixture_circle_upper.restitution = 0.0f;//反発係数
    fixture_circle_upper.isSensor = false;//センサーかどうか、trueならあたり判定は消える
    fixture_circle_upper.filter = createFilterExclude("Player_filter",{});
  
 


    //プレイヤーの下の円のコライダー
    //-------------------------------------------
    b2CircleShape circle_bottom;
    circle_bottom.m_p.Set(position.x, position.y + 0.1f);//下の方の円
    circle_bottom.m_radius = body_size.x / BOX2D_SCALE_MANAGEMENT * 0.5f;

    b2FixtureDef fixture_circle_bottom;
    fixture_circle_bottom.shape = &circle_bottom;
    fixture_circle_bottom.density = 1.3f;
    fixture_circle_bottom.friction = 3.0f;//摩擦
    fixture_circle_bottom.restitution = 0.0f;//反発係数
    fixture_circle_bottom.isSensor = false;//センサーかどうか、trueならあたり判定は消える
    fixture_circle_bottom.filter = createFilterExclude("Player_filter", {});
   
  
    
    //----------------------------------------------------

    //fixtureをbodyに登録
    b2Fixture* upper_circle_fixture = m_body->CreateFixture(&fixture_circle_upper);
    b2Fixture* bottom_circle_fixture = m_body->CreateFixture(&fixture_circle_bottom);

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
    ObjectData* playerdata = new ObjectData{ collider_player };
    //player_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);
    upper_circle_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);
    bottom_circle_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata);

  
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

    //横移動
   //----------------------------------------------------------------------------------------------------------------------------------------------------

       //スティックの値を受け取って正規化する
    float left_stick_x = state.leftStickX / 40000.0f;
    float left_stick_y = state.leftStickY / 40000.0f;
    b2Vec2 vel = m_body->GetLinearVelocity();
    b2Vec2 max_velocity = { 1.8f , 1.2f };
    b2Vec2 player_position = { PlayerPosition::GetPlayerPosition().x,PlayerPosition::GetPlayerPosition().y };
    b2Vec2 player_point = m_body->GetWorldPoint(player_position);

    //絶対値に変更する デットゾーンの審査に使うため　tool.cppに作った
    //デットゾーンをつくる x,yの値を足して一定以上経ったら　呼び出し
    if (0.5f < ReturnAbsoluteValue(left_stick_x) || Keyboard_IsKeyDown(KK_RIGHT) || Keyboard_IsKeyDown(KK_LEFT))
    {
        // 今のスティック値を、正規化して単位ベクトルにする
        b2Vec2 stick = { left_stick_x,  left_stick_y };
        stick.Normalize();

        //状態によってスピードが調整される
       //----------------------------------------------------------
        float adjust_speed = 0.0f;
        //ジャンプしている時はスピードは半分下がる
        if (GetIsJumping())
        {
            adjust_speed = -(GetSpeed() / 2);
        }
        //----------------------------------------------------------
        //右移動
        if ((vel.x < max_velocity.x) && ((stick.x > 0) || (Keyboard_IsKeyDown(KK_RIGHT))))
        {
            m_body->ApplyLinearImpulse({ GetSpeed() + adjust_speed , 0.0f }, player_point, true);
            m_direction = 1;
        }
        //左移動
        if ((vel.x > -max_velocity.x) && ((stick.x < 0) || (Keyboard_IsKeyDown(KK_LEFT))))
        {
            m_body->ApplyLinearImpulse({ -(GetSpeed()) + adjust_speed , 0.0f }, player_point, true);
            m_direction = -1;
        }

    }


    //ジャンプ処理
//----------------------------------------------------------------------------------------------------------------------------------------------------

    //ジャンプチェック
    if (!m_is_jumping && !m_jump_pressed && ((Keyboard_IsKeyDown(KK_UP) || (state.buttonA))))
    {
        if (vel.y < max_velocity.y)
        {
            m_body->ApplyLinearImpulse(m_jump_force, { 0.0f,1.0f }, true);
        }
        // m_body->ApplyLinearImpulseToCenter(m_jump_force, true);
        m_is_jumping = true;
    }
    m_jump_pressed = (Keyboard_IsKeyDown(KK_UP) || (state.buttonA));



    //アンカーのレベルを手動で変えられるしょり　完成版ではけす
    if (Keyboard_IsKeyDown(KK_O) || (state.dpadUp))
    {
        AnchorSpirit::EditAnchorSpiritValue(50); //加算
    }

    if (Keyboard_IsKeyDown(KK_P) || (state.dpadDown))
    {
        AnchorSpirit::EditAnchorSpiritValue(-50); //加算
    }



    //----------------------------------------------------------------------------------------------------------------------------------------------------

    //プレイヤーポジションCPPの関数にデータをセット
    PlayerPosition::SetPlayerPosition(m_body->GetPosition());


 //アンカーの処理
//----------------------------------------------------------------------------------------------------------------------------------------------------


    if ((Keyboard_IsKeyDown(KK_T) || (state.rightTrigger)) && Anchor::GetAnchorState() == Nonexistent_state)//何も存在しない状態でボタン入力で移行する
    {
        if(AnchorPoint::GetTargetAnchorPointBody()->GetPosition()!=m_body->GetPosition())//現在プレイヤーを標準としていない場合でのしょり
        Anchor::SetAnchorState(Create_state);//作成状態に移行
    }

    switch (Anchor::GetAnchorState())
    {
    case Nonexistent_state://何もない状態
        //ここからの移行は上のボタンで管理
        break;
    case Create_state:
        Anchor::CreateAnchor(b2Vec2(2.0f, 2.0f));//ここの引数でアンカーの大きさの調整ができるよー
        Anchor::SetAnchorState(Throwing_state);//アンカーの状態を投げるている状態に移行
        break;
    case Throwing_state://錨が飛んでいる状態
        Anchor::ThrowAnchorToAP();//アンカーをターゲットとしたアンカーポイントに向かって投げる関数


        //ここはコンタクトリストないの接触判定から接触状態へと移行
        break;
    case Connected_state://物体がくっついた状態　ジョイントの作成

        Anchor::CreateRotateJoint();//回転ジョイントを作成
        AnchorSpirit::EditAnchorSpiritValue(-25);//アンカーを使ったらゲージを払う
        Anchor::SetAnchorState(Pulling_state);//引っ張り状態に移行
        break;

    case Pulling_state://引っ張っている状態

        //呼ばれた回数でするかね　とりあえず2秒で
        if (g_anchor_pulling_number > 100)
        {
            Anchor::DeleteRotateJoint();
            Anchor::PullingAnchor();
        }


        if ((state.rightTrigger) || (Keyboard_IsKeyDown(KK_G)))
        {
            g_anchor_pulling_number = 200;
        }

        g_anchor_pulling_number++;//アンカーが引っ張る

        break;

    case Deleting_state://削除している状態
        g_anchor_pulling_number = 0;
        Anchor::DeleteAnchor();//アンカーを削除

        Anchor::SetAnchorState(Nonexistent_state);

        break;

    default:
        break;
    }

 //---------------------------------------------------------------------------------------------------------------------------------------------------------------------  

    //スティックの値を受け取って正規化する
    float stick_x= state.rightStickX / 32768.0f;
    float stick_y= state.rightStickY / 32768.0f;

    //keybordでのアンカーポイントの設定 X軸
    if (Keyboard_IsKeyDown(KK_A))
    {
        stick_x = -1.0f;
    }
    if (Keyboard_IsKeyDown(KK_D))
    {
        stick_x = 1.0f;
    }

    //keybordでのアンカーポイントの設定　Ｙ軸
    if (Keyboard_IsKeyDown(KK_W))
    {
        stick_y = +1.0f;
    }
    if (Keyboard_IsKeyDown(KK_S))
    {
        stick_y = -1.0f;
    }
   


    //絶対値に変更する デットゾーンの審査に使うため　tool.cppに作った
    //デットゾーンをつくる x,yの値を足して一定以上経ったら　呼び出し
    if (0.98 < ReturnAbsoluteValue(stick_x) + ReturnAbsoluteValue(stick_y))
    {
        AnchorPoint::SelectAnchorPoint(stick_x, stick_y);
    }

    //プレイヤーポジションCPPの関数にデータをセット
    PlayerPosition::SetPlayerPosition(m_body->GetPosition());


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


//ボディを外部から取得するために作った関数
b2Body* Player::GetOutSidePlayerBody()
{
    return player_body;
}