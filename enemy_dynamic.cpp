//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のcppファイル
// #make 2024/11/20
// #update 2025/02/05
// #comment 追加・修正予定
//          ・ステータス調整
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_dynamic.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"player_stamina.h"
#include"contactlist.h"
#include"anchor_spirit.h"
#include"Item_Manager.h"
#include"create_filter.h"


static ID3D11ShaderResourceView* g_EnemyDynamic_Texture;		  //動的エネミーのテクスチャ
static ID3D11ShaderResourceView* g_EnemyDynamic_Texture_Move;	  //動的エネミーの移動テクスチャ
static ID3D11ShaderResourceView* g_EnemyDynamic_Texture_Attack;	  //動的エネミーの攻撃テクスチャ
static ID3D11ShaderResourceView* g_EnemyDynamic_Texture_Destroyed;//動的エネミーの死亡テクスチャ
static ID3D11ShaderResourceView* g_EnemySensor_Texture = NULL;	  //エネミーのセンサーのテクスチャ

EnemyDynamic::EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_DYNAMIC_LIFE, ENEMY_DYNAMIC_DAMAGE, ENEMY_DYNAMIC_SOULGAGE, ENEMY_DYNAMIC_SCORE, true, false)
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

	b2CircleShape shape2;
	shape2.m_radius = size.x / 2;

	b2FixtureDef fixture2;
	fixture2.shape = &shape2;
	fixture2.density = 1.0f;
	fixture2.friction = 0.001f;//摩擦
	fixture2.restitution = 0.0f;//反発係数
	fixture2.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	fixture2.filter=createFilterExclude("enemy_filter", {});

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

	//Bodyにフィクスチャを登録する
	b2Fixture* enemy_static_fixture = GetBody()->CreateFixture(&fixture2);
	b2Fixture* enemy_sensor_fixture = GetBody()->CreateFixture(&fixture_sensor);

	// カスタムデータを作成して設定
	// 動的エネミーに値を登録
	// 動的エネミーにユーザーデータを登録
	ObjectData* data = new ObjectData{ collider_enemy_dynamic };
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	ObjectData* sensor_data = new ObjectData{ collider_enemy_sensor };
	enemy_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(sensor_data);

	//ID登録(センサーのデータにもエネミーと同じIDを入れる)
	data->object_name = Object_Enemy_Dynamic;
	int ID = data->GenerateID();
	data->id = ID;
	sensor_data->id = ID;
	//sensor_data2->id = ID;
	SetID(ID);

	m_state = ENEMY_STATE_NULL;
	m_move_force = body_size.x * body_size.y;
	m_sensor_move_size = (2 * body_size.x);
}

void EnemyDynamic::Initialize()
{
	if (g_EnemyDynamic_Texture == NULL) {
		g_EnemyDynamic_Texture = InitTexture(L"asset\\texture\\sample_texture\\enemy_1.png");//動的エネミーのテクスチャ
		g_EnemyDynamic_Texture_Move=InitTexture(L"asset\\texture\\enemy_texture\\enemy_move.png");//動的エネミーの移動テクスチャ
		g_EnemyDynamic_Texture_Attack = InitTexture(L"asset\\texture\\enemy_texture\\enemy_attack.png");//動的エネミーの攻撃テクスチャ
		g_EnemyDynamic_Texture_Destroyed = InitTexture(L"asset\\texture\\enemy_texture\\enemy_destroyed.png");//動的エネミーの死亡テクスチャ
		g_EnemySensor_Texture = InitTexture(L"asset\\texture\\sample_texture\\xxx_enemy_sensor.png");//エネミーのセンサーのテクスチャ
	}
}

void EnemyDynamic::Finalize()
{
	UnInitTexture(g_EnemyDynamic_Texture);
	UnInitTexture(g_EnemyDynamic_Texture_Move);
	UnInitTexture(g_EnemyDynamic_Texture_Attack);
	UnInitTexture(g_EnemyDynamic_Texture_Destroyed);
	UnInitTexture(g_EnemySensor_Texture);

	//ワールドに登録したbodyの削除
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	world->DestroyBody(GetBody());
	SetBody(nullptr);
}

void EnemyDynamic::Update()
{
	if (GetUse())
	{
		switch (GetState())
		{
		case ENEMY_STATE_MOVE:
			Move();
			m_old_state = ENEMY_STATE_MOVE;
			break;
		case ENEMY_STATE_ATTACK:
			Attack();
			m_old_state = ENEMY_STATE_ATTACK;
			break;
		default:
			if (GetInScreen())
			{
				SetState(ENEMY_STATE_MOVE);

				SetDirectionBasedOnPlayer();
			}
			m_old_state = ENEMY_STATE_NULL;
			break;
		}
	}
	else if (!GetUse())
	{
		//ソウルを落とす
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 1.0f,2.0f }, 0.0f, GetSoulgage());

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

		object_manager.DestroyEnemyDynamic(GetID());
	}
}

void EnemyDynamic::Draw()
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
	case ENEMY_STATE_MOVE:
		//貼るテクスチャを指定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyDynamic_Texture_Move);

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
	case ENEMY_STATE_ATTACK:
		//貼るテクスチャを指定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyDynamic_Texture_Attack);

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
	case ENEMY_STATE_DESTROYED:
		//貼るテクスチャを指定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyDynamic_Texture_Destroyed);

		DrawDividedSpritePlayer(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale * 2.0f ,GetSize().y * scale * 2.0f },
			4, 4, m_anim_id, 3.0, m_direction
		);
		m_anim_id++;
		break;
	default:
		//貼るテクスチャを指定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyDynamic_Texture_Move);

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

//移動
void EnemyDynamic::Move()
{
	//移動量取得
	b2Vec2 liner_velocity = GetBody()->GetLinearVelocity();

	//着地判定(y軸移動が無ければ着地中)
	if (liner_velocity.y != 0.0)
	{
		m_is_ground = false;
	}
	else
	{
		m_is_ground = true;
	}

	
	//崖で反転
	if ((m_ground_cnt == m_sensor_move_size && m_old_ground_cnt > m_sensor_move_size && m_is_ground ) ||
		(!m_is_ground && GetBody()->GetLinearVelocity().x == 0 && m_ground_cnt > 0))
	{
		SetDirection(!GetDirection());
	}

	//画面内なら移動
	if(GetInScreen())
	{
		//移動していないかつ、前回移動中だった場合ジャンプ
		GetBody()->SetLinearVelocity(b2Vec2(0.0, liner_velocity.y));
		if (((liner_velocity == b2Vec2(0.0, 0.0) && m_old_state == ENEMY_STATE_MOVE) && m_is_ground) ||
			((m_ground_cnt > m_sensor_move_size + 3 && m_old_state == ENEMY_STATE_MOVE) && m_is_ground))
		{
			if (GetDirection())
			{
				GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-m_speed * m_move_force, m_jump_force * m_move_force), true);
			}
			else if (!GetDirection())
			{
				GetBody()->ApplyLinearImpulseToCenter(b2Vec2(m_speed * m_move_force, m_jump_force * m_move_force), true);
			}
		}
		else
		{
			if (GetDirection())
			{
				GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-m_speed * m_move_force, 0.0), true);
			}
			else if(!GetDirection())
			{
				GetBody()->ApplyLinearImpulseToCenter(b2Vec2(m_speed* m_move_force, 0.0), true);
			}
		}
	}
	else
	{
		//画面外に出ていた場合、ステータスを変え移動用関数を呼び出さない
		m_anim_id = 0;
		SetState(ENEMY_STATE_NULL);
	}

	if (m_ground_cnt < m_sensor_move_size && m_old_ground_cnt < m_sensor_move_size && m_is_ground)
	{
		GetBody()->SetLinearVelocity(b2Vec2(0.0, 0.0));
	}

	//接触中の地面の数を記憶
	m_old_ground_cnt = m_ground_cnt;
	//攻撃クールタイム
	m_attack_cooltime_counter++;
}

//攻撃
void EnemyDynamic::Attack()
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
		GetBody()->SetType(b2_dynamicBody);
		SetState(ENEMY_STATE_NULL);
		m_anim_id = 0;
		m_attack_cooltime_counter = 0;
		return;
	}
}

//死亡
void EnemyDynamic::Destroyed()
{

}

//センサーとプレイヤーが触れた時の処理
void EnemyDynamic::CollisionSensorPlayer()
{
	//エネミーが攻撃中なら何もしない
	if ((GetState() != ENEMY_STATE_ATTACK) && (m_is_ground) && m_attack_cooltime_counter >= m_attack_cooltime)
	{
		SetDirectionBasedOnPlayer();
		m_anim_id = 0;
		//攻撃状態に移行
		SetState(ENEMY_STATE_ATTACK);
	}
}

void EnemyDynamic::SetDirectionBasedOnPlayer()
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