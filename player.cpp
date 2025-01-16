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
#include"anchor.h"
#include"anchor_spirit.h"
#include"scene.h"
#include"create_filter.h"
#include"Item_Manager.h"
#include"hit_stop.h"

//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_player_Texture=NULL;


//センサーの画像
ID3D11ShaderResourceView* g_player_sensor_Texture=NULL;

//staticメンバー変数の初期化
bool    Player::m_is_jumping = false;
bool    Player::m_jump_pressed = false;
bool     Player::m_direction = 1;



b2Body* player_body;


int g_anchor_frame_management_number = 0;

Player::Player()
{
    //ここではbodyを生成しない
}


Player::~Player()
{
 
}

void Player::Initialize(b2Vec2 position, b2Vec2 body_size, b2Vec2 sensor_size)
{
    if (m_body) {
        // ボディを削除
        Box2dWorld& box2d_world = Box2dWorld::GetInstance();
        b2World* world = box2d_world.GetBox2dWorldPointer();
        world->DestroyBody(m_body);
        m_body = nullptr;
    }

    //テクスチャのロード
    g_player_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");

    g_player_sensor_Texture= InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");


    b2BodyDef body;
    body.type = b2_dynamicBody;
    body.position.Set(position.x, position.y);
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
    size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
    size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;


    //センサーの設定用の
    b2Vec2 size_sensor;//命名すまん
    size_sensor.x = sensor_size.x / BOX2D_SCALE_MANAGEMENT;
    size_sensor.y = sensor_size.y / BOX2D_SCALE_MANAGEMENT;



    //プレイヤーの真ん中の長方形ボディ
   //-------------------------------------------
    b2PolygonShape rectangle_body;
    rectangle_body.SetAsBox(size.x * 0.5, size.y * 0.5f);

    b2FixtureDef fixture_rectangle_body;
    fixture_rectangle_body.shape = &rectangle_body;
    fixture_rectangle_body.density = 1.0f;//密度
    fixture_rectangle_body.friction = 0.001f;//摩擦
    fixture_rectangle_body.restitution = 0.1f;//反発係数
    fixture_rectangle_body.isSensor = false;//センサーかどうか、trueならあたり判定は消える
    fixture_rectangle_body.filter = createFilterExclude("Player_filter", {});


    //プレイヤーの下の円のコライダー
    //-------------------------------------------
    b2CircleShape circle_bottom;
    circle_bottom.m_p.Set(0.0f, size.y / 2);//下の方の円
    circle_bottom.m_radius = body_size.x / BOX2D_SCALE_MANAGEMENT * 0.5f;

    b2FixtureDef fixture_circle_bottom;
    fixture_circle_bottom.shape = &circle_bottom;
    fixture_circle_bottom.density = 1.3f;
    fixture_circle_bottom.friction = 1.0f;//摩擦
    fixture_circle_bottom.restitution = 0.0f;//反発係数
    fixture_circle_bottom.isSensor = false;//センサーかどうか、trueならあたり判定は消える
    fixture_circle_bottom.filter = createFilterExclude("Player_filter", {});



    //----------------------------------------------------

    //fixtureをbodyに登録
    //b2Fixture* upper_circle_fixture = m_body->CreateFixture(&fixture_circle_upper);
    b2Fixture* body_rectangle_fixture = m_body->CreateFixture(&fixture_rectangle_body);
    b2Fixture* bottom_circle_fixture = m_body->CreateFixture(&fixture_circle_bottom);

    // カスタムデータを作成して設定
    // プレイヤーに値を登録
    // プレーヤーにユーザーデータを登録
    ObjectData* playerdata_body = new ObjectData{ collider_player_body };
    ObjectData* playerdata_leg = new ObjectData{ collider_player_leg };

    body_rectangle_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata_body);
    bottom_circle_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(playerdata_leg);


    //--------------------------------------------------------------------------------------------------

    //プレイヤーのセンサーを新しくつくる

    b2PolygonShape shape_sensor;


    b2Vec2 vertices[4] = { b2Vec2(0.0f,0.0f) };

    // 反時計回りで頂点を設定
    vertices[0].Set(-size_sensor.x / 2, size_sensor.y / 2 / 3);  // 左下
    vertices[1].Set(size_sensor.x / 2, size_sensor.y / 2 / 3);   // 右下
    vertices[2].Set(size_sensor.x / 2, -size_sensor.y / 2);    // 右上
    vertices[3].Set(-size_sensor.x / 2, -size_sensor.y / 2);   // 左上


    shape_sensor.Set(vertices, 4);



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

void Player::Update()
{
    // プレイヤーの更新処理
    
    //センサーの画面サイズに応じた大きさの変動
    Player_sensor_size_change(AnchorSpirit::GetAnchorLevel());



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
            m_direction = 0;
        }

        //playerのスピード上昇
        //上がり方がわかりずらいかもしれない
        if (Keyboard_IsKeyDownTrigger(KK_Q))
        {
            m_speed = 0.75f;

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


    //ジャンプのバフ
    if (Keyboard_IsKeyDownTrigger(KK_Z))
    {
        m_jump_force = b2Vec2(0.0f, -0.40f * 1.5f);
    }


    //アンカーのレベルを手動で変えられるしょり　完成版ではけす
    if (Keyboard_IsKeyDown(KK_O) || (state.dpadUp))
    {
        AnchorSpirit::EditAnchorSpiritValue(50); //加算
    }

    if (Keyboard_IsKeyDown(KK_P) || (state.dpadDown))
    {
        AnchorSpirit::EditAnchorSpiritValue(-50); //加算
    }


    if (Keyboard_IsKeyDown(KK_L) || (state.dpadDown))
    {
        HitStop::StartHitStop(60);
    }

    //----------------------------------------------------------------------------------------------------------------------------------------------------

        //プレイヤーポジションCPPの関数にデータをセット
    PlayerPosition::SetPlayerPosition(m_body->GetPosition());


//ソウルアイテム回収処理
//----------------------------------------------------------------------------------------------------------------------------------------------------
    if (Keyboard_IsKeyDown(KK_B) || state.buttonB)
    {
        ItemManager& itemManager = ItemManager::GetInstance();
        itemManager.SetCollectSpirit(true);
    }




 //アンカーの処理
//----------------------------------------------------------------------------------------------------------------------------------------------------



    //オブジェクトに投げるアンカー処理の呼び出し
    if ((Keyboard_IsKeyDown(KK_T) || (state.rightTrigger)) && Anchor::GetAnchorState() == Nonexistent_state)//何も存在しない状態でボタン入力で移行する
    {
        if(AnchorPoint::GetTargetAnchorPointBody()->GetPosition()!=m_body->GetPosition())//現在プレイヤーを標準としていない場合でのしょり
        Anchor::SetAnchorState(Create_state);//作成状態に移行
    }


    //通常攻撃のアンカーの呼び出し
    if ((Keyboard_IsKeyDown(KK_N) || (state.buttonX)) && Anchor::GetAnchorState() == Nonexistent_state)//何も存在しない状態でボタン入力で移行する
    {
        Anchor::SetAnchorState(WaitCraateNormalAttack_state);
    }

 
    //プレーの向いている方向　swtich文的の仕様的に外でやる
    bool right = GetDirection();

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

        g_anchor_frame_management_number = 0;
        break;

    case Pulling_state://引っ張っている状態

        //呼ばれた回数でするかね　とりあえず2秒で
        if (g_anchor_frame_management_number > 100)
        {
            Anchor::DeleteRotateJoint();
            Anchor::PullingAnchor();
        }


        if ((state.rightTrigger) || (Keyboard_IsKeyDown(KK_G)))
        {
            g_anchor_frame_management_number = 200;
        }




        g_anchor_frame_management_number++;//アンカーが引っ張る

        break;

    case Deleting_state://削除している状態
        g_anchor_frame_management_number = 0;
        Anchor::DeleteAnchor();//アンカーを削除

        Anchor::SetAnchorState(Nonexistent_state);

        break;

    case WaitCraateNormalAttack_state:
        //通常攻撃の発生前の待ち状態

        g_anchor_frame_management_number++;
        if (45 < g_anchor_frame_management_number)
        {
            g_anchor_frame_management_number = 0;
            Anchor::SetAnchorState(CreateNormalAttack_state);//通常攻撃発生
        }

        break;
    case CreateNormalAttack_state:

        //通常攻撃の判定をつくる
        Anchor::CreateNormalAttack(b2Vec2(2.0f, 2.0f), right);//通常攻撃のボディをつくる

        Anchor::SetAnchorState(NowAttackngNormalAttack);
        break;
    case NowAttackngNormalAttack:
        //攻撃中
        g_anchor_frame_management_number++;
        if (g_anchor_frame_management_number < 15)
        {
            g_anchor_frame_management_number = 0;
            Anchor::SetAnchorState(DeletingNormaklAttack_state);
        }

        break;
    case DeletingNormaklAttack_state:
        //通常攻撃を削除する
        Anchor::DeleteNormalAttackAnchor();
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


void Player::Player_sensor_size_change(int anchor_level)
{
    if (anchor_level < 3)//アンカーレベルの１、２の時
    {
        if (GetSensorSize() == GetSensorSizeLev3())//センサーの大きさを取得して
        {
            b2Vec2 pos=GetPlayerBody()->GetPosition();
            Initialize(pos, b2Vec2(1, 2), GetSensorSizeLev1_2());
        }
    }

    if (anchor_level == 3)//アンカーレベルが３の時
    {
        if (GetSensorSize() == GetSensorSizeLev1_2())//大きさを取得して差分があれば
        {
            b2Vec2 pos = GetPlayerBody()->GetPosition();
            Initialize(pos, b2Vec2(1, 2), GetSensorSizeLev3());
        }
    }
}


void Player::Draw()
{
    if (m_body != nullptr) {
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
            { GetSize().x * scale,GetSize().y * scale }
        );

        //----------------------------------------------------------------------------------------
        //センサー描画


        //// シェーダリソースを設定
        //GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_sensor_Texture);

        //DrawSprite(
        //    { screen_center.x,
        //      screen_center.y },
        //    m_body->GetAngle(),
        //    { GetSensorSize().x * scale,GetSensorSize().y * scale }
        //);
        //float size_sensor = GetSensorSize().x * scale;
        //float size = GetSize().x * scale;

    }
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



    


