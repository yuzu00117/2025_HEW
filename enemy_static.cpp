//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 静的エネミーのcppファイル
// #make 2024/11/19
// #update 2025/02/10
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_static.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"contactlist.h"
#include"anchor_spirit.h"
#include"object_manager.h"
#include"Item_Manager.h"

static ID3D11ShaderResourceView* g_EnemyStatic_Texture = NULL;	//静的エネミーのテクスチャ
static ID3D11ShaderResourceView* g_EnemyStatic_Texture_Move;	  //静的エネミーの移動テクスチャ
static ID3D11ShaderResourceView* g_EnemyStatic_Texture_Attack;	  //静的エネミーの攻撃テクスチャ
static ID3D11ShaderResourceView* g_EnemyStatic_Texture_Destroyed;//静的エネミーの死亡テクスチャ
static ID3D11ShaderResourceView* g_EnemySensor_Texture = NULL;	  //エネミーのセンサーのテクスチャ

EnemyStatic::EnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_STATIC_LIFE, ENEMY_STATIC_DAMAGE, ENEMY_STATIC_SPIRIT_TYPE, ENEMY_STATIC_SCORE, true, false)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;							//静的なオブジェクトにするならture
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	SetBody(world->CreateBody(&body));//Bodyをワールドに固定


	SetSize(body_size);//表示用にサイズをセットしとく、表示のときにGetSizeを呼び出す



	b2Vec2 size;
	size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;                         //shapeには色々な型がある　サークルとかもあるよ
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//あたり判定を登録する4点　*0.5するのは

	b2FixtureDef fixture;
	fixture.shape = &shape;    //シャープをフィクスチャに登録する
	fixture.density = 0.1f;    //密度
	fixture.friction = 0.05f;  //摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

	//====================================================================================================
	//センサーの登録
	//====================================================================================================
	b2Vec2 size_sensor;
	size_sensor.x = body_size.x / BOX2D_SCALE_MANAGEMENT * 2;
	size_sensor.y = body_size.y / BOX2D_SCALE_MANAGEMENT;

	b2PolygonShape shape_sensor;
	shape_sensor.SetAsBox(size_sensor.x * 0.5, size_sensor.y * 0.5);
	m_size_sensor = b2Vec2(body_size.x * 2, body_size.y);

	b2FixtureDef fixture_sensor;
	fixture_sensor.shape = &shape_sensor;
	fixture_sensor.density = 0.0f;//密度
	fixture_sensor.friction = 0.0f;//摩擦
	fixture_sensor.restitution = 0.0f;//反発係数
	fixture_sensor.isSensor = true;//センサーかどうか、trueならあたり判定は消える
	//====================================================================================================

	b2Fixture* enemy_static_fixture = GetBody()->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する
	b2Fixture* enemy_sensor_fixture = GetBody()->CreateFixture(&fixture_sensor);


	// カスタムデータを作成して設定
	// 動的エネミーに値を登録
	// 動的エネミーにユーザーデータを登録
	m_object_data = std::make_unique<ObjectData>(collider_enemy_static);
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_object_data.get());
	m_sensor_data = std::make_unique<ObjectData>(collider_enemy_static_sensor);
	enemy_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_sensor_data.get());

	m_object_data->object_name = Object_Enemy_Static;
	int ID = m_object_data->GenerateID();
	m_object_data->id = ID;
	m_sensor_data->id = ID;
	SetID(ID);
}

void EnemyStatic::Initialize()
{
	if (g_EnemyStatic_Texture == NULL) {
		g_EnemyStatic_Texture = InitTexture(L"asset\\texture\\sample_texture\\enemy_1.png");//静的エネミーのテクスチャ
		g_EnemyStatic_Texture_Move = InitTexture(L"asset\\texture\\enemy_texture\\enemy_move.png");//静的エネミーの移動テクスチャ
		g_EnemyStatic_Texture_Attack = InitTexture(L"asset\\texture\\enemy_texture\\enemy_attack.png");//静的エネミーの攻撃テクスチャ
		g_EnemyStatic_Texture_Destroyed = InitTexture(L"asset\\texture\\enemy_texture\\enemy_destroyed.png");//静的エネミーの死亡テクスチャ
		g_EnemySensor_Texture = InitTexture(L"asset\\texture\\sample_texture\\xxx_enemy_sensor.png");//エネミーのセンサーのテクスチャ
	}
}

void EnemyStatic::Finalize()
{
	if (g_EnemyStatic_Texture) UnInitTexture(g_EnemyStatic_Texture);
	if (g_EnemyStatic_Texture_Move) UnInitTexture(g_EnemyStatic_Texture_Move);
	if (g_EnemyStatic_Texture_Attack) UnInitTexture(g_EnemyStatic_Texture_Attack);
	if (g_EnemyStatic_Texture_Destroyed) UnInitTexture(g_EnemyStatic_Texture_Destroyed);
	if (g_EnemySensor_Texture) UnInitTexture(g_EnemySensor_Texture);


	//ワールドに登録したbodyの削除
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	world->DestroyBody(GetBody());
	SetBody(nullptr);
}

void EnemyStatic::Update()
{
	if (GetUse())
	{
		switch (GetState())
		{
		case ENEMY_STATIC_STATE_IDLE:
			Idle();
			m_old_state = ENEMY_STATIC_STATE_IDLE;
			break;
		case ENEMY_STATIC_STATE_ATTACK:
			Attack();
			m_old_state = ENEMY_STATIC_STATE_ATTACK;
			break;
		default:
			if (GetInScreen())
			{
				SetState(ENEMY_STATIC_STATE_IDLE);

				SetDirectionBasedOnPlayer();
			}
			m_old_state = ENEMY_STATIC_STATE_NULL;
			break;
		}
	}
	else if (!GetUse())
	{
		//ソウルを落とす
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 2.0f,3.0f }, 0.0f, GetSpiritType(), false);



		b2Vec2 now_positon = GetBody()->GetPosition();
		b2Vec2 now_size = GetSize();
		b2Vec2 now_vec = GetBody()->GetLinearVelocity();

		item_manager.AddDamageValue(b2Vec2{ now_positon.x, now_positon.y - 0.2f }, b2Vec2{ 1.5f,1.5f }, 0.0f, DamageOwnerType_enemy, ENEMY_STATIC_LIFE);

		//ワールドに登録したbodyの削除
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());
		SetBody(nullptr);

		//オブジェクトマネージャー内のエネミー削除
		ObjectManager& object_manager = ObjectManager::GetInstance();

		if (object_manager.FindEnemyAttackByID(GetID()))
		{
			object_manager.FindEnemyAttackByID(GetID())->SetUse(false);
		}

		object_manager.DestroyEnemyStatic(GetID());

		object_manager.AddChangeEnemyFilterAndBody(now_positon, b2Vec2(now_size.x * 2, now_size.y * 2), b2Vec2_zero, g_EnemyStatic_Texture_Destroyed, 4, 4, now_vec);
	}
}

void EnemyStatic::Draw()
{
	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 position = GetBody()->GetPosition();

	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	switch (GetState())
	{
	case ENEMY_STATIC_STATE_IDLE:
		//貼るテクスチャを指定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyStatic_Texture_Move);

		DrawDividedSpritePlayer(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale * 2.0f ,GetSize().y * scale * 2.0f },
			5, 5, m_anim_id, 3.0, m_direction
		);
		m_anim_id++;
		m_anim_id = m_anim_id % 25;
		break;
	case ENEMY_STATIC_STATE_ATTACK:
		//貼るテクスチャを指定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyStatic_Texture_Attack);

		//draw
		DrawDividedSpritePlayer(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale * 3.0f ,GetSize().y * scale * 2.0f },
			5, 5, m_anim_id, 3.0, m_direction
		);
		if (m_attack_counter % 2 == 0)
		{
			m_anim_id++;
		}
		m_anim_id = m_anim_id % 25;
		break;
	default:
		//貼るテクスチャを指定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyStatic_Texture_Move);

		DrawDividedSpritePlayer(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale * 2.0f ,GetSize().y * scale * 2.0f },
			5, 5, 0, 3.0, m_direction
		);
		break;
	}


	//============================================================
	//テスト:センサー描画
	//============================================================
	/*GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemySensor_Texture);
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ m_size_sensor.x * scale , m_size_sensor.y * scale }
	);*/
}

void EnemyStatic::Idle()
{
	SetDirectionBasedOnPlayer();
	//攻撃クールタイム
	m_attack_cooltime_counter++;
}

//攻撃
void EnemyStatic::Attack()
{
	//フレームカウント
	m_attack_counter++;
	//攻撃中は停止
	GetBody()->SetType(b2_staticBody);

	//攻撃発生フレームになったら攻撃を生成
	if (m_attack_counter == m_attack_birth)
	{
		ObjectManager& object_manager = ObjectManager::GetInstance();
		b2Vec2 pos = GetBody()->GetPosition();
		if (GetDirection())
		{
			object_manager.AddEnemyAttack(b2Vec2(pos.x - GetSize().x / 4, pos.y), GetSize(), 0.0f, GetID());
		}
		else
		{
			object_manager.AddEnemyAttack(b2Vec2(pos.x + GetSize().x / 4, pos.y), GetSize(), 0.0f, GetID());
		}
	}

	//攻撃動作が全て終了したら、動くように戻して移動状態にする
	if (m_attack_counter > m_attack_finish)
	{
		m_attack_counter = 0;
		SetState(ENEMY_STATIC_STATE_NULL);
		m_anim_id = 0;
		m_attack_cooltime_counter = 0;
		return;
	}
}

//センサーとプレイヤーが触れた時の処理
void EnemyStatic::CollisionSensorPlayer()
{
	//エネミーが攻撃中なら何もしない
	if ((GetState() != ENEMY_STATE_ATTACK) && m_attack_cooltime_counter >= m_attack_cooltime)
	{
		SetDirectionBasedOnPlayer();
		m_anim_id = 0;
		//攻撃状態に移行
		SetState(ENEMY_STATIC_STATE_ATTACK);
	}
}

void EnemyStatic::SetDirectionBasedOnPlayer()
{
	//プレイヤー追従(簡易)
		//プレイヤーのポジション取得
	b2Vec2 player_position;
	player_position.x = PlayerPosition::GetPlayerPosition().x;
	player_position.y = PlayerPosition::GetPlayerPosition().y;

	//移動方向
	b2Vec2 enemy_vector;
	enemy_vector.x = player_position.x - GetBody()->GetPosition().x;
	enemy_vector.y = player_position.y - GetBody()->GetPosition().y;

	bool old_direction = GetDirection();

	//プレイヤーの方向を向く
	if (enemy_vector.x > 0)
	{
		SetDirection(false);
	}
	else
	{
		SetDirection(true);
	}
}
