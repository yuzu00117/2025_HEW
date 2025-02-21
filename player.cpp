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
#include"sound.h"
#include"hit_stop.h"
#include"camera_shake.h"
#include"display.h"
#include<cmath>
#include"FixtureSizeCalculate.h"




//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_player_Texture=NULL;

//ジャンプのシート
ID3D11ShaderResourceView* g_player_jump_sheet = NULL;
//錨を投げるシート
ID3D11ShaderResourceView* g_player_throw_anchor_sheet = NULL;
//横移動のシート
ID3D11ShaderResourceView* g_player_walk_sheet = NULL;
//通常攻撃
ID3D11ShaderResourceView* g_player_normal_attack_sheet = NULL;
ID3D11ShaderResourceView* g_player_normal_attack_anchor_sheet = NULL;
//被弾モーション
ID3D11ShaderResourceView* g_player_damaged_sheet = NULL;


//歩く時のエフェクト
ID3D11ShaderResourceView* g_player_walk_effect = NULL;


//プレイヤーが死んだ時
ID3D11ShaderResourceView* g_player_dead_Texture = NULL;

//センサーの画像
ID3D11ShaderResourceView* g_player_sensor_Texture=NULL;

//レベル３で
ID3D11ShaderResourceView* g_anachor_level_3_Frame1_Texture = NULL;
ID3D11ShaderResourceView* g_anachor_level_3_Frame2_Texture = NULL;

static ID3D11ShaderResourceView* g_Anchor_Effect_S1 =   NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_S2=    NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_S3 =   NULL;//アンカーのエフェクト

static ID3D11ShaderResourceView* g_Anchor_Effect_M1 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_M2 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_M3 = NULL;//アンカーのエフェクト


static ID3D11ShaderResourceView* g_Anchor_Effect_L1 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_L2 = NULL;//アンカーのエフェクト
static ID3D11ShaderResourceView* g_Anchor_Effect_L3 = NULL;//アンカーのエフェクト



//アンカーレベルが変わった際に表示するエフェクト プレイヤーの背景として表示する
static ID3D11ShaderResourceView* g_Anchor_LevelUp_1to2_Effect = NULL;//アンカーがレベル１から２になった場合のエフェクト
static ID3D11ShaderResourceView* g_Anchor_LevelUp_2to3_Effect = NULL;//アンカーがレベル2から３になった場合のエフェクト
static ID3D11ShaderResourceView* g_Anchor_LevelDown_Effect = NULL;  //アンカーがレベルが下がったひと

//たまちゃん
static ID3D11ShaderResourceView* g_TamaChan_Lv1 = NULL;//たまちゃんLv1
static ID3D11ShaderResourceView* g_TamaChan_Lv2 = NULL;//たまちゃんLv2
static ID3D11ShaderResourceView* g_TamaChan_Lv3 = NULL;//たまちゃんLv3

//ソウルのゲットのやつ
static ID3D11ShaderResourceView* g_SoulGet_Lv1_Effect = NULL;//ソウルのゲット時に表示Lv1
static ID3D11ShaderResourceView* g_SoulGet_Lv2_Effect = NULL;//ソウルのゲット時に表示Lv2
static ID3D11ShaderResourceView* g_SoulGet_Lv3_Effect = NULL;//ソウルのゲット時に表示Lv3


//staticメンバー変数の初期化
float   Player::m_AnchorThrowing_SpeedUp = 1.0f;
bool    Player::m_is_jumping = false;
bool    Player::m_jump_pressed = false;
bool     Player::m_direction = 1;
b2Vec2  Player::m_jump_force = b2Vec2(0.0f, -0.43f);
float   Player::m_speed = 0.04f;

int Player::invincible_time = 0;

bool    CollectSpirit_pressed = false;

b2Body* player_body;


int g_anchor_frame_management_number = 0;
float g_KnockBack_total_time = 0.0f;        //  ノックバック（ベジエ2回合計）に使う時間
float g_KnockBack_elapce_time = 0.0f;       //　ノックバック（ベジエ2回合計）の経過時間
bool    g_damage_from_right = true;         //  ダメージはプレイヤーの右から来てるのかどうか
b2Vec2 g_Beziers_parameter_position[2][3];  //  ２回の2次ベジエのパラメータを入れるための変数（[0][0] => 1回目の始点　　　[1][2] => 2回目の終点）
b2Vec2 g_Beziers_prev_position;             // 前フレームでのposition
int g_Beziers_id = 0;                       //  1回のノックバックで2次ベジエ2回やるから、ベジエ何回目かのID

Player::Player()
{
    //ここではbodyを生成しない
}


Player::~Player()
{
 
}

void Player::Initialize(b2Vec2 position, b2Vec2 body_size, b2Vec2 sensor_size, bool respawning)
{
    if (respawning)
    {
        m_AnchorThrowing_SpeedUp = 1.0f;
        m_is_jumping = false;
        m_jump_pressed = false;
        m_direction = 1;
        m_jump_force = b2Vec2(0.0f, -0.40f);
        m_speed = 0.04f;
        invincible_time = 0;
    }

    if (m_body) {
        // ボディを削除
        Box2dWorld& box2d_world = Box2dWorld::GetInstance();
        b2World* world = box2d_world.GetBox2dWorldPointer();
        world->DestroyBody(m_body);
        m_body = nullptr;
    }

    m_body_position = position;
    m_body_size = body_size;
    m_initial_sensor_size = sensor_size;
    player_alpha = 3.0f;

    g_anchor_frame_management_number = 0;
    g_KnockBack_total_time = 0.0f;        //  ノックバック（ベジエ2回合計）に使う時間
    g_KnockBack_elapce_time = 0.0f;       //　ノックバック（ベジエ2回合計）の経過時間
    g_damage_from_right = true;         //  ダメージはプレイヤーの右から来てるのかどうか
    g_Beziers_prev_position = { 0.0f,0.0f };             // 前フレームでのposition
    g_Beziers_id = 0;                       //  1回のノックバックで2次ベジエ2回やるから、ベジエ何回目かのID


    if (g_player_Texture == NULL) {
        //テクスチャのロード

        g_player_dead_Texture=InitTexture(L"asset\\texture\\player_texture\\player_dead_sheet.png");

        g_player_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");

        g_player_jump_sheet = InitTexture(L"asset\\texture\\player_texture\\player_jump_sheet.png");

        g_player_damaged_sheet = InitTexture(L"asset\\texture\\player_texture\\player_damaged_sheet.png");

        g_player_throw_anchor_sheet = InitTexture(L"asset\\texture\\player_texture\\player_throw_anchor_sheet.png");

        g_player_normal_attack_anchor_sheet = InitTexture(L"asset\\texture\\player_texture\\player_normal_attack_anchor_sheet.png");

        g_player_normal_attack_sheet = InitTexture(L"asset\\texture\\player_texture\\player_normal_attack_sheet.png");

        g_player_walk_sheet = InitTexture(L"asset\\texture\\player_texture\\player_walk_sheet.png");

        g_player_sensor_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");

        g_player_walk_effect = InitTexture(L"asset\\texture\\player_texture\\player_walk_effect.png");


        g_anachor_level_3_Frame1_Texture = InitTexture(L"asset\\texture\\anchor_point\\soul_frame1.png");

        g_anachor_level_3_Frame2_Texture = InitTexture(L"asset\\texture\\anchor_point\\soul_frame2.png");

        g_Anchor_Effect_S1 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_S_1_3_2.png");
        g_Anchor_Effect_S2 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_S_2_3_2.png");
        g_Anchor_Effect_S3 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_S_3_4_2.png");


        g_Anchor_Effect_M1 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_M_1_5_2.png");
        g_Anchor_Effect_M2 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_M_2_5_2.png");
        g_Anchor_Effect_M3 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_M_3_4_2.png");

        g_Anchor_Effect_L1 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_L_1_4_2.png");
        g_Anchor_Effect_L2 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_L_2_5_2.png");
        g_Anchor_Effect_L3 = InitTexture(L"asset\\texture\\anchor_point\\Effect_Anchor_L_3_5_2.png");


        g_Anchor_LevelUp_1to2_Effect = InitTexture(L"asset\\texture\\anchor_point\\Anchor_Level_Up_1to2_Effect.png");
        g_Anchor_LevelUp_2to3_Effect = InitTexture(L"asset\\texture\\anchor_point\\Anchor_Level_Up_2to3_Effect.png");
        g_Anchor_LevelDown_Effect =    InitTexture(L"asset\\texture\\anchor_point\\Anchor_Level_Domn_Effect.png");


        g_TamaChan_Lv1 = InitTexture(L"asset\\texture\\anchor_point\\tama_Lv1.png");
        g_TamaChan_Lv2 = InitTexture(L"asset\\texture\\anchor_point\\tama_Lv2.png");
        g_TamaChan_Lv3 = InitTexture(L"asset\\texture\\anchor_point\\tama_Lv3.png");


        //ソウルのゲット時に表示
        g_SoulGet_Lv1_Effect = InitTexture(L"asset\\texture\\player_texture\\EFF_soulget_S_2x4.png");
        g_SoulGet_Lv2_Effect = InitTexture(L"asset\\texture\\player_texture\\EFF_soulget_M_2x3.png");
        g_SoulGet_Lv3_Effect = InitTexture(L"asset\\texture\\player_texture\\EFF_soulget_L_3x5.png");
    }


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
        SetSensorSize(b2Vec2(sensor_size.x * DISPLAY_RANGE_TO_SCALE, sensor_size.y * DISPLAY_RANGE_TO_SCALE));//センサー表示をするためにセット




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
        fixture_circle_bottom.friction = 2.5f;//摩擦
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
        fixture_sensor.filter = createFilterExclude("player_sensor_filter", {});



        b2Fixture* player_sensor_fixture = m_body->CreateFixture(&fixture_sensor);


        // カスタムデータを作成して設定
       // プレイヤーに値を登録
       // プレーヤーにユーザーデータを登録
        ObjectData* player_sensor_data = new ObjectData{ collider_player_sensor };
        player_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(player_sensor_data);

        //---------------------------------------------------------------------------------------------------------------------------

        //エフェクト用のセンサー　撃墜演出のね
        float sencor_x_Scale=0.6;

        float sensor_y_Scale_over = 0.8;
        float sencor_y_Scale_under =0.4;

        b2PolygonShape effect_sensor;


        b2Vec2 effect_vertices[4] = { b2Vec2(0.0f,0.0f) };

        // 反時計回りで頂点を設定
        effect_vertices[0].Set(-size_sensor.x / 2* sencor_x_Scale, size_sensor.y / 2 / 3 * sencor_y_Scale_under);  // 左下
        effect_vertices[1].Set(size_sensor.x / 2 * sencor_x_Scale, size_sensor.y / 2 / 3 * sencor_y_Scale_under);   // 右下
        effect_vertices[2].Set(size_sensor.x / 2 * sencor_x_Scale, -size_sensor.y / 2 * sensor_y_Scale_over);    // 右上
        effect_vertices[3].Set(-size_sensor.x / 2 * sencor_x_Scale, -size_sensor.y / 2 * sensor_y_Scale_over);   // 左上


        effect_sensor.Set(effect_vertices, 4);



        b2FixtureDef effect_fixture_sensor;
        effect_fixture_sensor.shape = &effect_sensor;
        effect_fixture_sensor.density = 0.0f;//密度
        effect_fixture_sensor.friction = 0.0f;//摩擦
        effect_fixture_sensor.restitution = 0.0f;//反発係数
        effect_fixture_sensor.isSensor = true;//センサーかどうか、trueならあたり判定は消える
        effect_fixture_sensor.filter = createFilterExclude("player_sensor_filter", {});


        b2Fixture* effect_player_sensor_fixture = m_body->CreateFixture(&effect_fixture_sensor);


        // カスタムデータを作成して設定
       // プレイヤーに値を登録
       // プレーヤーにユーザーデータを登録
        ObjectData* effect_player_sensor_data = new ObjectData{ collider_effect_sensor };
        effect_player_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(effect_player_sensor_data);

        //---------------------------------------------------------------------------------------------------

        sensor_flag = false;

        draw_state = player_nomal_state;
    
}

void Player::Update()
{
    // プレイヤーの更新処理

    //モーションのDrawカウントを加算
    draw_cnt++;
    

    // Sensorの変更フラグの管理とエフェクトのトリガー
    int current_level = AnchorSpirit::GetAnchorLevel();

    if (old_anchor_Lev != current_level)
    {
        // レベルの変化を詳細にチェック
        switch (old_anchor_Lev)
        {
        case 1: // 過去がレベル1だった
            if (current_level == 2)
            {
                Anchor_level_up_down_sheet_type = 1;
            }
            else if (current_level == 3)
            {
                Anchor_level_up_down_sheet_type = 2;
            }
            break;

        case 2:
            if (current_level == 1)
            {
                Anchor_level_up_down_sheet_type = 3;
            }
            else if (current_level == 3)
            {
                Anchor_level_up_down_sheet_type = 2;
            }
            break;

        case 3:
            if (current_level == 1)
            {
                Anchor_level_up_down_sheet_type = 3;
            }
            else if (current_level == 2)
            {
                Anchor_level_up_down_sheet_type = 3;
            }
            break;

        default:
            // 予期しないレベルのとき
            break;
        }

        // エフェクトシートを更新
        Now_Anchor_level_up_Down_effect_sheet = 1;
    }


    //Sensorの変更フラグの管理
    if (old_anchor_Lev != AnchorSpirit::GetAnchorLevel())
    {
        if ((old_anchor_Lev == 1 || old_anchor_Lev == 2)&& (AnchorSpirit::GetAnchorLevel()==1|| AnchorSpirit::GetAnchorLevel() == 2))
        {
        }
        else
        {
            sensor_flag = false;
        }  
    }
    old_anchor_Lev = AnchorSpirit::GetAnchorLevel();
    //センサーの画面サイズに応じた大きさの変動
    Player_sensor_size_change(AnchorSpirit::GetAnchorLevel());



    

    //コントローラーの入力の受け取り
    ControllerState state = GetControllerInput();


    //無敵時間の処理
    Invincible_time_update();
    KnockBack_Update();

    

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
         // プレイヤーが歩行中かどうかのフラグ
        // 右移動
        if ((vel.x < max_velocity.x) && ((stick.x > 0) || (Keyboard_IsKeyDown(KK_RIGHT))))
        {
            
            m_body->ApplyLinearImpulseToCenter({ GetSpeed() + adjust_speed , 0.0f }, true);
        

            // 使用中は左右反転できないようにする
            if (Anchor::GetAnchorState() == Nonexistent_state)
            {
                m_direction = 1;
            }

            // 歩行状態に変更
            if (draw_state == player_nomal_state)
            {
                draw_state = player_walk_state;
            }

            // 歩行音を再生中でなければ再生する
            if (draw_state == player_walk_state && !app_atomex_is_playing(Player_Walk_Sound))
            {
                app_atomex_start(Player_Walk_Sound);
            }
        }

        // 左移動
        if ((vel.x > -max_velocity.x) && ((stick.x < 0) || (Keyboard_IsKeyDown(KK_LEFT))))
        {
          
             m_body->ApplyLinearImpulseToCenter({ -(GetSpeed()) + adjust_speed, 0.0f }, true);
          
         

            // 使用中は左右反転できないようにする
            if (Anchor::GetAnchorState() == Nonexistent_state)
            {
                m_direction = 0;
            }

            // 歩行状態に変更
            if (draw_state == player_nomal_state)
            {
                draw_state = player_walk_state;
            }

            // 歩行音を再生中でなければ再生する
            if (draw_state == player_walk_state && !app_atomex_is_playing(Player_Walk_Sound))
            {
                app_atomex_start(Player_Walk_Sound);
            }
        }

        // 停止した場合、歩行音を停止
        if (!(Keyboard_IsKeyDown(KK_RIGHT) || Keyboard_IsKeyDown(KK_LEFT) || stick.x != 0))
        {
            if (draw_state == player_walk_state)
            {
                app_atomex_stop_cue(Player_Walk_Sound);
            }

            draw_state = player_nomal_state;
        }


  
       

      

        //宝石による移動速度アップした時滑るので、プレイヤーの足の摩擦力を上げたのと以下の処理を一応
        if (!(stick.x < 0 || Keyboard_IsKeyDown(KK_LEFT) || stick.x>0 || Keyboard_IsKeyDown(KK_RIGHT))||draw_state == player_nomal_state)
        {
            m_body->SetLinearVelocity({ 0.0f, 0.0f });
            draw_state = player_nomal_state;
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
    if ((!m_is_jumping && !m_jump_pressed && ((Keyboard_IsKeyDown(KK_UP) || (state.buttonA))))&&Anchor::GetAnchorState()==Nonexistent_state)
    {
        if (vel.y < max_velocity.y)
        {
            m_body->ApplyLinearImpulse(m_jump_force, { 0.0f,1.0f }, true);
            draw_state = player_jumping_state;
            draw_cnt = 0;
            app_atomex_start(Player_Jump_Start_Sound);
        }
        // m_body->ApplyLinearImpulseToCenter(m_jump_force, true);
        m_is_jumping = true;

        VibrationController::StartVibration(0.5,0.5,6);
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


//宝石使う処理(テスト用)
//----------------------------------------------------------------------------------------------------------------------------------------------------
    if (!CollectSpirit_pressed && (Keyboard_IsKeyDownTrigger(KK_J)))
    {
        ItemManager& itemManager = ItemManager::GetInstance();
        itemManager.UseAllJewel();

        app_atomex_start(Player_Soul_Colect1_Sound);
    }
    CollectSpirit_pressed = (Keyboard_IsKeyDownTrigger(KK_B));



 //アンカーの処理
//----------------------------------------------------------------------------------------------------------------------------------------------------



    //オブジェクトに投げるアンカー処理の呼び出し
    if ((Keyboard_IsKeyDown(KK_T) || (state.rightTrigger)) && Anchor::GetAnchorState() == Nonexistent_state)//何も存在しない状態でボタン入力で移行する
    {
        if (AnchorPoint::GetTargetAnchorPointBody() != nullptr)
        {
          
            if (false == AnchorPoint::AnchorPointListCheck())
            {
                return;
            }
      

            if (AnchorPoint::GetTargetAnchorPointBody()->GetPosition() != m_body->GetPosition())//現在プレイヤーを標準としていない場合でのしょり
            {
               Anchor::SetAnchorState(Create_wait_draw_cnt_state);//作成状態に移行
            }
        }
    }


    ////通常攻撃のアンカーの呼び出し
    //if ((Keyboard_IsKeyDown(KK_N) || (state.buttonX)) && Anchor::GetAnchorState() == Nonexistent_state)//何も存在しない状態でボタン入力で移行する
    //{
    //    Anchor::SetAnchorState(WaitCreateNormalAttackDraw_cnt_state);
    //}

 
    //プレーの向いている方向　swtich文的の仕様的に外でやる
    bool right = GetDirection();

    switch (Anchor::GetAnchorState())
    {
    case Nonexistent_state://何もない状態
        //ここからの移行は上のボタンで管理
        break;
    case Create_wait_draw_cnt_state:
        draw_cnt = 0;
        draw_state = player_throw_anchor_state;
        Anchor::SetAnchorState(Create_wait_state);

        break;
    case Create_wait_state:
       

        if (30 < draw_cnt)
        {
            Anchor::SetAnchorState(Create_state);
           
        }
        
        break;
    

    case Create_state:
        Anchor::CreateAnchor(b2Vec2(2.0f, 2.0f));//ここの引数でアンカーの大きさの調整ができるよー
        
        app_atomex_start(Anchor_Thorw_Sound);
        Anchor::SetAnchorState(Throwing_state);

        if (AnchorPoint::GetTargetAnchorPointBody()->GetPosition().x < m_body->GetPosition().x)
        {
            m_direction = 0;
        }
        else
        {
            m_direction = 1;
        }
    
        StartAnchorEffect();
        break;

    case Throwing_state://錨が飛んでいる状態
        Anchor::ThrowAnchorToAP(m_AnchorThrowing_SpeedUp);//アンカーをターゲットとしたアンカーポイントに向かって投げる関数
        g_anchor_frame_management_number++;

        if (180 < g_anchor_frame_management_number)
        {
            g_anchor_frame_management_number = 0;
            Anchor::SetAnchorState(Pulling_state);
        }

      
     
       

        //ここはコンタクトリストないの接触判定から接触状態へと移行
        break;
    case Connected_state://物体がくっついた状態　ジョイントの作成

        Anchor::CreateRotateJoint();//回転ジョイントを作成
        AnchorSpirit::EditAnchorSpiritValue(-25);//アンカーを使ったらゲージを払う
        Anchor::SetAnchorState(Pulling_state);//引っ張り状態に移行

       
        app_atomex_start(Anchor_Pulling_Sound);
        g_anchor_frame_management_number = 0;
        break;

    case Pulling_state://引っ張っている状態

        g_anchor_frame_management_number++;
        //呼ばれた回数でするかね　とりあえず2秒で
      
        if (g_anchor_frame_management_number > 60)
        {
            Anchor::PullingAnchor();
            Anchor::DeleteRotateJoint();
        }


       
           
        


        if (g_anchor_frame_management_number > 120)
        {
            g_anchor_frame_management_number = 0;
            Anchor::SetAnchorState(Deleting_state);
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
        AnchorPoint::OutsideSensor(AnchorPoint::GetTargetAnchorPointBody());

        break;
    case WaitCreateNormalAttackDraw_cnt_state:
        draw_cnt = 0;
        draw_state = player_normal_attack_state;
        Anchor::SetAnchorState(WaitCraateNormalAttack_state);//通常攻撃発生
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
        Anchor::CreateNormalAttack(b2Vec2(3.0f, 3.0f), right);//通常攻撃のボディをつくる
        app_atomex_start(Player_Attack_Sound);

   


        Anchor::SetAnchorState(NowAttackngNormalAttack);

      
        break;
    case NowAttackngNormalAttack:
        //攻撃中
        g_anchor_frame_management_number++;
        if (g_anchor_frame_management_number > 8)
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

    if (Keyboard_IsKeyDown(KK_M))
    {
        draw_state = player_dameged_state;
        Player_Damaged(-50,120,nullptr);

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

void Player::Player_Damaged(int Change_to_HP,int invincibletime, const b2Body* attack_body)
{
    // HPを減らす
    PlayerStamina::EditPlayerStaminaValue(Change_to_HP);//HPに加算減算する　今回は減算

    //無敵時間を付与
    invincible_time = invincibletime;
    g_KnockBack_total_time = invincible_time * 0.27f;  //ノックバックのする時間（無敵時間と比例してる）
    if (attack_body != nullptr)
    {
        if (GetOutSidePlayerBody()->GetPosition().x < attack_body->GetPosition().x)
        {
            g_damage_from_right = true;    //ダメージはプレイやーの右から来てるかどうかをセット
        }
        else
        {
            g_damage_from_right = false;    //ダメージはプレイやーの右から来てるかどうかをセット
        }
    }
    

    //被弾の音
    app_atomex_start(Player_Damege_Sound);

    // フィルターを変更
    updateFixtureFilter("Player_filter", {"object_filter","enemy_filter","MiniGolem_filter","Boss_filter"});

}

void Player::Invincible_time_update(void)
{
    if (invincible_time != 0)
    {
        invincible_time--;

        if (invincible_time % 5 == 0)
        {
            if (player_alpha == 3.0f)
            {
                player_alpha = 0.5;
            }
            else
            {
                player_alpha = 3.0f;
            }
        }


        if (invincible_time <= 0)
        {
            //プレイヤーの無的状態を解除
            updateFixtureFilter("Player_filter", {});
            player_alpha = 3.0f;
        }
    }

}

void    Player::KnockBack_Update()
{
    //ノックバックが終わってないのなら更新処理に入る
    if (g_KnockBack_elapce_time < g_KnockBack_total_time)
    {
        //2回のベジエの時間や距離などの比例
        float   Beziers_total_ratio = 5.0f;
        float   Beziers_A_ratio = 3.0f;
        float   Beziers_B_ratio = 2.0f;

        //ノックバック開始直後ならパラメータの初期設定を行う
        if (g_Beziers_id == 0 && g_KnockBack_elapce_time == 0.0f)
        {
            float distance_x = 50.0f;                   //ベジエ二回全部で移動したｘ座標距離
            float max_bounce_height = 30.0f;            //最大バウンド高さ

            float distance_x_A = distance_x / Beziers_total_ratio * Beziers_A_ratio;    //一回目のベジエの時の始点と終点の距離
            float distance_x_B = distance_x / Beziers_total_ratio * Beziers_B_ratio;    //二回目のベジエの時の始点と終点の距離

            //左からの攻撃だったら、右の方に飛ばされる
            if (!g_damage_from_right)
            {
                distance_x_A = -distance_x_A;
                distance_x_B = -distance_x_B;
            }

            //ノックバック前と後と曲線を制御してる座標を設定しておく
            g_Beziers_parameter_position[0][0] = g_Beziers_prev_position = GetOutSidePlayerBody()->GetPosition();
            g_Beziers_parameter_position[0][1] = { g_Beziers_parameter_position[0][0].x - distance_x_A / 2, g_Beziers_parameter_position[0][0].y - max_bounce_height };
            g_Beziers_parameter_position[0][2] = { g_Beziers_parameter_position[0][0].x - distance_x_A, g_Beziers_parameter_position[0][0].y };
            g_Beziers_parameter_position[1][0] = g_Beziers_parameter_position[0][2];
            g_Beziers_parameter_position[1][1] = { g_Beziers_parameter_position[1][0].x - distance_x_B / 2, g_Beziers_parameter_position[1][0].y - max_bounce_height / 2 };
            g_Beziers_parameter_position[1][2] = { g_Beziers_parameter_position[1][0].x - distance_x_B, g_Beziers_parameter_position[1][0].y };
        }
        
        //3次ベジエ計算
        float time_elapced_ratio = 0.0f;
        switch (g_Beziers_id)
        {
        case 0:
            time_elapced_ratio = g_KnockBack_elapce_time / (g_KnockBack_total_time / Beziers_total_ratio * Beziers_A_ratio);
            break;
        case 1:
            time_elapced_ratio = g_KnockBack_elapce_time / (g_KnockBack_total_time / Beziers_total_ratio * Beziers_B_ratio);
            break;
        }
        float time_remained_ratio = 1 - time_elapced_ratio;
        float parameter_time_ratio[3];
        parameter_time_ratio[0] = time_remained_ratio * time_remained_ratio;
        parameter_time_ratio[1] = (2 * time_elapced_ratio) * (time_remained_ratio);
        parameter_time_ratio[2] = time_elapced_ratio * time_elapced_ratio;

        b2Vec2 new_position = { 0.0f,0.0f };
        for (int i = 0; i < 3; i++)
        {
            new_position.x += parameter_time_ratio[i] * g_Beziers_parameter_position[g_Beziers_id][i].x;
            new_position.y += parameter_time_ratio[i] * g_Beziers_parameter_position[g_Beziers_id][i].y;
        }

        b2Vec2  velocity = new_position - g_Beziers_prev_position;

        GetOutSidePlayerBody()->SetLinearVelocity(velocity);
        

        g_Beziers_prev_position = new_position;

        //タイムの更新
        g_KnockBack_elapce_time++;
        switch (g_Beziers_id)
        {
        case 0:
            if (g_KnockBack_elapce_time >= g_KnockBack_total_time / Beziers_total_ratio * Beziers_A_ratio)
            {
                g_KnockBack_elapce_time = 1.0f;
                g_Beziers_id++;
            }
            break;
        case 1:
            if (g_KnockBack_elapce_time >= g_KnockBack_total_time / Beziers_total_ratio * Beziers_B_ratio)
            {
                g_KnockBack_elapce_time = g_KnockBack_total_time = 0.0f;
                g_Beziers_id = 0;
            }
            break;
        }

    }

}


void Player::updateFixtureFilter(const std::string& category, const std::vector<std::string>& includeMasks) {
    // ボディの最初のフィクスチャを取得
    b2Fixture* fixture = GetOutSidePlayerBody()->GetFixtureList();

  

    // フィクスチャが存在しない場合は早期リターン
    if (!fixture) {
        return;
    }

    // 新しいフィルターを作成
    b2Filter newFilter = createFilterExclude(category, includeMasks);

    // すべてのフィクスチャに対してフィルターを更新
    while (fixture) {

        if (fixture->IsSensor()==false)//センサーじゃなかったらフィルターを変更する
        {
            fixture->SetFilterData(newFilter);   
        }
        fixture = fixture->GetNext(); // 次のフィクスチャに移動
    }
}


void Player::Player_sensor_size_change(int anchor_level)
{
    if (anchor_level < 3)//アンカーレベルの１、２の時
    {
        if(sensor_flag==false)
        {
            b2Vec2 pos=GetPlayerBody()->GetPosition();
            Initialize(pos, b2Vec2(1, 2), GetSensorSizeLev1_2());

            SetSensorSize(GetSensorSizeLev1_2());
            sensor_flag = true;
        }
    }

    if (anchor_level == 3)//アンカーレベルが３の時
    {
        if(sensor_flag==false)
        {
            b2Vec2 pos = GetPlayerBody()->GetPosition();
            Initialize(pos, b2Vec2(1, 2), GetSensorSizeLev3());
            SetSensorSize(GetSensorSizeLev3());
            sensor_flag = true;
            Anchor_level3_Frame_Sheet_cnt = 0;
        }
    }
}

void Player::StartAnchorEffect()
{
    int rand=GetRandomInt(1, 3);

    int anchorlevel=AnchorSpirit::GetAnchorLevel();

    Anchor_effect_sheet = 0;
    switch (anchorlevel)
    {
    //アンカーレベル１
    case 1:
        switch (rand)
        {
        case 1:
            Anchor_Effect_Type = 1;
            Max_Anchor_effect_sheet = 6;

            break;
        case 2:
            Anchor_Effect_Type = 2;
            Max_Anchor_effect_sheet = 6;
            break;
        case 3:
            Anchor_Effect_Type = 3;
            Max_Anchor_effect_sheet = 8;
            break;
            

        default:
            break;
        }
        break;
    //アンカーレベル2
    case 2:
        switch (rand)
        {
        case 1:
            Anchor_Effect_Type = 4;
            Max_Anchor_effect_sheet = 10;

            break;
        case 2:
            Anchor_Effect_Type = 5;
            Max_Anchor_effect_sheet = 10;
            break;
        case 3:
            Anchor_Effect_Type = 6;
            Max_Anchor_effect_sheet = 8;
            break;


        default:
            break;
        }
        break;
    //アンカーレベル3
    case 3:
        switch (rand)
        {
        case 1:
            Anchor_Effect_Type = 7;
            Max_Anchor_effect_sheet = 8;

            break;
        case 2:
            Anchor_Effect_Type = 8;
            Max_Anchor_effect_sheet = 10;
            break;
        case 3:
            Anchor_Effect_Type = 9;
            Max_Anchor_effect_sheet = 10;
            break;


        default:
            break;
        }
        break;

    default:
        break;
    }
}

void Player::DrawAnchorEffect()
{
    if (Anchor_effect_sheet < Max_Anchor_effect_sheet)
    {

       
        float scale = SCREEN_SCALE;

        // スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;

        int player_scale_x = 3.0f;
        int player_scale_y = 2.0f;

        // コライダーの位置の取得（プレイヤーの位置）
        b2Vec2 player_position;
        player_position.x = m_body->GetPosition().x;
        player_position.y = m_body->GetPosition().y;
        switch (Anchor_Effect_Type)
        {
        case 1:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_S1);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                3, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;
        case 2:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_S2);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                3, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;
        case 3:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_S3);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                4, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;
        case 4:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_M1);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                5, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;
        case 5:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_M2);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                4, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;
        case 6:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_M3);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                4, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;
        case 7:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_L1);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                4, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;
        case 8:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_L2);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                5, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;

        case 9:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_Effect_L3);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                5, 2, Anchor_effect_sheet / 4, player_alpha, m_direction
            );
            break;
        default:
            break;
        }


        Anchor_effect_sheet+=0.5;
    }
}


void Player::Draw()
{
    if (m_body != nullptr) {


        //レベルが変わった時にエフェクトを発生させる
        DrawAnchorLevelUpDownEffect();

        // コライダーと位置情報の補正をするため
        float scale = SCREEN_SCALE;

        // スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;

        

        // コライダーの位置の取得（プレイヤーの位置）
        b2Vec2 player_position;
        player_position.x = m_body->GetPosition().x;
        player_position.y = m_body->GetPosition().y;


        //描画との大きさのずれがあるから
        float player_scale_x = 3.0f;
        float player_scale_y = 1.5f;


        //プレイヤーは画面中央に固定、まあ補正値はいれるんやけど
        //ほかのplayer以外の物体はプレイヤー分Positionを引いて
       // 描画位置を調整して、プレイヤーがスクリーン中央に

       //あたり判定の描画
        //GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_Texture);
        //DrawSprite({ screen_center.x,
        //      screen_center.y },
        //    m_body->GetAngle(),
        //    { GetSize().x * scale,GetSize().y * scale } );

      
       
   
       
        int left = 1;

        switch (draw_state)
        {
        case null:
            break;
        case player_nomal_state:

            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_jump_sheet);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                5, 5, 0, player_alpha, m_direction

            );

           
            break;
        case player_jumping_state:

            

            if (30 < draw_cnt)
            {
                if (m_is_jumping)
                {
                    draw_cnt = 30;
                }
                
            }

            if (50 < draw_cnt)
            {
                draw_cnt = 0;
                draw_state = player_nomal_state;
            }


            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_jump_sheet);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                5, 5, draw_cnt/2, player_alpha, m_direction

            );

           


            break;
        case player_throw_anchor_state:


           

            if (64<draw_cnt)
            {
                draw_cnt = 0;
                draw_state = player_nomal_state;
            }
            if (Anchor::GetAnchorState() != Nonexistent_state)
            {
                if (40<draw_cnt)
                {
                    draw_cnt = 40;
                }
            }

            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_throw_anchor_sheet);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                4, 4, draw_cnt / 4, player_alpha, m_direction

            );

            break;
        case player_dameged_state:
           
            if (24 < draw_cnt)
            {
                draw_cnt = 0;
                draw_state = player_nomal_state;
            }

            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_damaged_sheet);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                3, 3, draw_cnt / 4, player_alpha, m_direction

            );

            break;
        case player_walk_state:

            

            if (ReturnAbsoluteValue(m_body->GetLinearVelocity().x)<0.1)//横の移動量の絶対値が0.1未満の時にノーマルに戻る
            {
                draw_cnt = 0;
                draw_state = player_nomal_state;
            }

          

            // **10フレームごとにプレイヤーの座標を記録**
            CreateDustEffect(m_body->GetPosition());
           

            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_walk_sheet);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                3, 6, draw_cnt/2, player_alpha, m_direction

            );

         

            dustFrameCnt++; // フレームをカウント




            
            break;

        case player_normal_attack_state:
           

           

            if (50 < draw_cnt)
            {
                draw_state = player_nomal_state;
                draw_cnt = 0;
            }


            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_normal_attack_sheet);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                5, 5, draw_cnt / 3, 3.0, m_direction

            );

            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_normal_attack_anchor_sheet);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x ,GetSize().y * scale * player_scale_y },
                5, 5, draw_cnt / 3, player_alpha, m_direction

            );
            break;

        case player_dead_state:

            draw_cnt++;

            if (126 < draw_cnt)
            {
                draw_cnt = 126;
            }

            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_dead_Texture);

            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y },
                m_body->GetAngle(),
                { GetSize().x * scale * player_scale_x*1.7f ,GetSize().y * scale * player_scale_y },
                8, 8, draw_cnt / 2, 3.0, m_direction

            );

            break;
           
        default:
            break;
        }


        // **土煙エフェクトの更新**
        UpdateDustEffect();
        // **土煙を描画**
        DrawDustEffect();
      
        //アンカーを投げる時にエフェクトを発生させる
        DrawAnchorEffect();


       

        //----------------------------------------------------------------------------------------
        //センサー描画

        b2Vec2 Pos = GetPlayerBody()->GetPosition();


        float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

       
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_sensor_Texture);

        DrawSprite(
            { draw_x,
              draw_y },
            m_body->GetAngle(),
            { GetSensorSize().x * scale,GetSensorSize().y * scale }
        );
       

        //エフェクトレベル３になった時のフレーム
        DrawAnchorLevel3Frame();


        //たまちゃん
        DrawTamaChan();

        //ソウルをゲットした時のエフェクト
        DrawSoulGetEffect();
    }
}


void Player::DrawSoulGetEffect()
{
    if (soul_get_effect_sheet_cnt != 0)
    {
        // コライダーと位置情報の補正をするため
        float scale = SCREEN_SCALE;

        // スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;



        // コライダーの位置の取得（プレイヤーの位置）
        b2Vec2 player_position;
        player_position.x = m_body->GetPosition().x;
        player_position.y = m_body->GetPosition().y;

        b2Vec2 Pos = GetPlayerBody()->GetPosition();


        float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y+20;

        float effect_scale = 0.0;

        soul_get_effect_sheet_cnt += 0.2;

        switch (soul_get_effect_type)
        {
        case 1:
            effect_scale = 6.0f;
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_SoulGet_Lv1_Effect);

            DrawDividedSpritePlayer(
                { draw_x,
                  draw_y },
                m_body->GetAngle(),
                { GetSize().x * scale * effect_scale ,GetSize().x * scale * effect_scale },
                4, 2, soul_get_effect_sheet_cnt , 1.0, m_direction
            );

            if (7 <= soul_get_effect_sheet_cnt)
            {
                soul_get_effect_sheet_cnt = 0;
            }

            break;
        case 2:
            effect_scale = 8.0f;
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_SoulGet_Lv2_Effect);

            DrawDividedSpritePlayer(
                { draw_x,
                  draw_y },
                m_body->GetAngle(),
                { GetSize().x * scale * effect_scale ,GetSize().x * scale * effect_scale },
                3, 2, soul_get_effect_sheet_cnt , 1.0, m_direction

            );
            if (5 <= soul_get_effect_sheet_cnt)
            {
                soul_get_effect_sheet_cnt = 0;
            }
            break;
        case 3:
            effect_scale = 10.0f;
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_SoulGet_Lv3_Effect);

            DrawDividedSpritePlayer(
                { draw_x,
                  draw_y },
                m_body->GetAngle(),
                { GetSize().x * scale * effect_scale ,GetSize().x * scale * effect_scale },
                5, 3, soul_get_effect_sheet_cnt , 1.0, m_direction

            );
            if (14 <= soul_get_effect_sheet_cnt)
            {
                soul_get_effect_sheet_cnt = 0;
            }
            break;
        default:
            break;
        }

      



    }



}


void Player::UpdateDustEffect() {
    // **土煙の寿命を減らし、30フレーム経過したものを削除**
    for (auto it = dustEffects.begin(); it != dustEffects.end();) {
        it->lifeTime--;
        if (it->lifeTime <= 0) {
            it = dustEffects.erase(it); // 30フレーム経過で削除
        }
        else {
            ++it;
        }
    }
}

// **プレイヤーが走った際に土煙を記録**
void Player::CreateDustEffect(b2Vec2 playerPos) {
    if (dustFrameCnt % 10 == 0) { // 10フレームごとに記録
        dustEffects.emplace_back(playerPos);
    }
}

// **土煙を描画**
void Player::DrawDustEffect() {
    // コライダーと位置情報の補正をするため
    float scale = SCREEN_SCALE;

    // スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
    b2Vec2 screen_center;
    screen_center.x = SCREEN_CENTER_X;
    screen_center.y = SCREEN_CENTER_Y;
    if (dustEffects.size() > 1) {
        for (size_t i = 0; i < dustEffects.size() - 1; i++) { // 最後の要素はスキップ
            const auto& dust = dustEffects[i];

            float dust_draw_x = ((dust.pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
            float dust_draw_y = ((dust.pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_walk_effect);



            DrawDividedSpritePlayer(
                { dust_draw_x, dust_draw_y + 45 },
                0.0,
                { Walk_effect_size.x * scale, Walk_effect_size.y * scale },
                6, 4, draw_cnt, player_alpha, 1
            );
        }
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
        UnInitTexture(g_player_dead_Texture);
        UnInitTexture(g_player_Texture);
        UnInitTexture(g_player_jump_sheet);
        UnInitTexture(g_player_throw_anchor_sheet);
        UnInitTexture(g_player_walk_sheet);
        UnInitTexture(g_player_normal_attack_sheet);
        UnInitTexture(g_player_normal_attack_anchor_sheet);
        UnInitTexture(g_player_damaged_sheet);
        UnInitTexture(g_player_walk_effect);
        UnInitTexture(g_player_sensor_Texture);
        UnInitTexture(g_anachor_level_3_Frame1_Texture);
        UnInitTexture(g_anachor_level_3_Frame2_Texture);
        UnInitTexture(g_Anchor_Effect_S1);
        UnInitTexture(g_Anchor_Effect_S2);
        UnInitTexture(g_Anchor_Effect_S3);
        UnInitTexture(g_Anchor_Effect_M1);
        UnInitTexture(g_Anchor_Effect_M2);
        UnInitTexture(g_Anchor_Effect_M3);
        UnInitTexture(g_Anchor_Effect_L1);
        UnInitTexture(g_Anchor_Effect_L2);
        UnInitTexture(g_Anchor_Effect_L3);
        UnInitTexture(g_Anchor_LevelUp_1to2_Effect);
        UnInitTexture(g_Anchor_LevelUp_2to3_Effect);
        UnInitTexture(g_Anchor_LevelDown_Effect);
        UnInitTexture(g_TamaChan_Lv1);
        UnInitTexture(g_TamaChan_Lv2);
        UnInitTexture(g_TamaChan_Lv3);

        // ソウルゲット時のエフェクト解放
        UnInitTexture(g_SoulGet_Lv1_Effect);
        UnInitTexture(g_SoulGet_Lv2_Effect);
        UnInitTexture(g_SoulGet_Lv3_Effect);

        g_player_dead_Texture = NULL;
        g_player_Texture = NULL;
        g_player_jump_sheet = NULL;
        g_player_throw_anchor_sheet = NULL;
        g_player_walk_sheet = NULL;
        g_player_normal_attack_sheet = NULL;
        g_player_normal_attack_anchor_sheet = NULL;
        g_player_damaged_sheet = NULL;
        g_player_sensor_Texture = NULL;
        g_anachor_level_3_Frame1_Texture = NULL;
        g_anachor_level_3_Frame2_Texture = NULL;
        g_Anchor_Effect_S1 = NULL;
        g_Anchor_Effect_S2 = NULL;
        g_Anchor_Effect_S3 = NULL;
        g_Anchor_Effect_M1 = NULL;
        g_Anchor_Effect_M2 = NULL;
        g_Anchor_Effect_M3 = NULL;
        g_Anchor_Effect_L1 = NULL;
        g_Anchor_Effect_L2 = NULL;
        g_Anchor_Effect_L3 = NULL;

        g_Anchor_LevelUp_1to2_Effect = NULL;
        g_Anchor_LevelUp_2to3_Effect = NULL;
        g_Anchor_LevelDown_Effect = NULL;
        g_TamaChan_Lv1 = NULL;
        g_TamaChan_Lv2 = NULL;
        g_TamaChan_Lv3 = NULL;



        g_SoulGet_Lv1_Effect = NULL;
        g_SoulGet_Lv2_Effect = NULL;
        g_SoulGet_Lv3_Effect = NULL;
    }

}




//ボディを外部から取得するために作った関数
b2Body* Player::GetOutSidePlayerBody()
{
    return player_body;
}




    


void Player::DrawAnchorLevel3Frame()
{
    if (AnchorSpirit::GetAnchorLevel() == 3)
    {

        if (Anchor_level3_Frame_Sheet_cnt < 50)
        {

            //サウンドを再生
            if (Anchor_level3_Frame_Sheet_cnt == 0)
            {
                app_atomex_start(Player_Frame_Up_Sound);
            }

            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_anachor_level_3_Frame1_Texture);
            DrawDividedSprite(
                { SCREEN_WIDTH / 2,
                 SCREEN_HEIGHT / 2 },
                0,
                { 1280 ,720 }
                , 10, 5, Anchor_level3_Frame_Sheet_cnt, 0.5f
            );

            //// シェーダリソースを設定
            //GetDeviceContext()->PSSetShaderResources(0, 1, &g_anachor_level_3_Frame1_Texture);
            //DrawDividedSprite(
            //    { SCREEN_WIDTH / 2,
            //     SCREEN_HEIGHT / 2 },
            //    0,
            //    { 1280 ,720 }
            //    , 10, 5, Anchor_level3_Frame_Sheet_cnt, 0.5f
            //);


            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_anachor_level_3_Frame2_Texture);
            DrawDividedSprite(
                { SCREEN_WIDTH / 2,
                 SCREEN_HEIGHT / 2 },
                0,
                { 1280 ,720 }
                , 10, 5, Anchor_level3_Frame_Sheet_cnt, 0.5f
            );

            //フレームのカウントを加算
            Anchor_level3_Frame_Sheet_cnt += 0.5;
        }

      
    }
 
}

void Player::DrawAnchorLevelUpDownEffect()
{
    if (Now_Anchor_level_up_Down_effect_sheet != 0)
    {
        // コライダーと位置情報の補正をするため
        float scale = SCREEN_SCALE;

        // スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
        b2Vec2 screen_center;
        screen_center.x = SCREEN_CENTER_X;
        screen_center.y = SCREEN_CENTER_Y;

      

        float player_pos_y = 20;//少しプレイヤーの下の方に座標を調整している

        float effect_alpha = 1.0f;//アルファ値の管理

        float effect_size = 3.0f;
     
        switch (Anchor_level_up_down_sheet_type)
        {
        case 1:
            Max_Anchor_level_up_Down_effect_sheet = 5;
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_LevelUp_1to2_Effect);
            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y+ player_pos_y },
                m_body->GetAngle(),
                { GetSize().x * scale*2 * effect_size ,GetSize().y * scale* effect_size },
                5, 1, Now_Anchor_level_up_Down_effect_sheet, effect_alpha, m_direction
            );
            break;
        case 2:
            Max_Anchor_level_up_Down_effect_sheet = 6;
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_LevelUp_2to3_Effect);
            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y + player_pos_y },
                m_body->GetAngle(),
                { GetSize().x * scale*2* effect_size*1.2f  ,GetSize().y * scale* effect_size*1.2f },
                3, 2, Now_Anchor_level_up_Down_effect_sheet, effect_alpha, m_direction
            );

            break;
        case 3:
            Max_Anchor_level_up_Down_effect_sheet = 12;
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Anchor_LevelDown_Effect);
            DrawDividedSpritePlayer(
                { screen_center.x,
                  screen_center.y + player_pos_y },
                m_body->GetAngle(),
                { GetSize().x * scale*2 * effect_size ,GetSize().y * scale* effect_size },
                6, 2, Now_Anchor_level_up_Down_effect_sheet, effect_alpha, m_direction
            );

            break;
        default:
            break;
        }

        Now_Anchor_level_up_Down_effect_sheet += 0.3;
        if (Now_Anchor_level_up_Down_effect_sheet > Max_Anchor_level_up_Down_effect_sheet)
        {
            Now_Anchor_level_up_Down_effect_sheet = 0;
        }
     
       
    }
}

void Player::DrawTamaChan()
{
    // コライダーと位置情報の補正をするため
    float scale = SCREEN_SCALE;

    // スクリーン中央位置 (16m x 9m の解像度で、中央は x = 8, y = 4.5 と仮定)
    b2Vec2 screen_center;
    screen_center.x = SCREEN_CENTER_X;
    screen_center.y = SCREEN_CENTER_Y;

    // コライダーの位置の取得（プレイヤーの位置）
    b2Vec2 player_pos = GetPlayerBody()->GetPosition();

    // たまちゃんの現在の位置を取得
    static b2Vec2 tamachan_pos = player_pos;

    // プレイヤーの位置に向かって徐々に移動する
    float follow_speed = 0.03f; // 追従速度（0.0f〜1.0fの範囲で調整）

    // プレイヤーの向きを取得
    bool direction = GetDirection(); // true: 右向き, false: 左向き

    // 向きに応じてたまちゃんの位置を反転
    if (!direction) {
        // たまちゃんの位置を反転
        tamachan_pos.x += (player_pos.x - tamachan_pos.x + 0.5) * follow_speed;
        tamachan_pos.y += (player_pos.y - tamachan_pos.y) - 1 * follow_speed;
        // たまちゃんの位置がプレイヤーを越したら
        if (tamachan_pos.x > player_pos.x) {
            // たまちゃんの向きを反転
            is_left = false;
		}

    }
    else
    {   // たまちゃんの位置を反転
        tamachan_pos.x += (player_pos.x - tamachan_pos.x - 0.5) * follow_speed;
        tamachan_pos.y += (player_pos.y - tamachan_pos.y) - 1 * follow_speed;
        // たまちゃんの位置がプレイヤーを越したら
        if (tamachan_pos.x < player_pos.x) {
            // たまちゃんの向きを反転
            is_left = true;
        }
    }

    // たまちゃんの描画位置を計算
    float draw_x = ((tamachan_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
    float draw_y = ((tamachan_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

    b2Vec2 tamachan_size;

    switch (AnchorSpirit::GetAnchorLevel())
    {
    case 1:
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_TamaChan_Lv1);
        tamachan_size = { 75.f,75.f };
        break;
    case 2:
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_TamaChan_Lv2);
        tamachan_size = { 125.f,125.f };
        break;
    case 3:
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_TamaChan_Lv3);
        tamachan_size = { 175.f,175.f };
        break;
    default:
        break;
    }

   

    // スプライトを描画
    DrawDividedSpritePlayer(
        { draw_x, draw_y -100 },        // 描画位置
        0,                              // 回転角度
        { tamachan_size.x, tamachan_size.y },                   // サイズ
        6, 6,                           // 分割数
        TamaChanSheetCnt,               // シートカウント
        3.0f,                           // スケール
        is_left                         // 左向きかどうか
        
    );

    TamaChanSheetCnt += 0.5;
}

int Player::GetAnchorFrameManagement()
{
    return g_anchor_frame_management_number;
}