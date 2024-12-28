//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のcppファイル
// #make 2024/11/20
// #update 2024/12/20
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

static ID3D11ShaderResourceView* g_EnemyDynamic_Texture;//動的エネミーのテクスチャ
static ID3D11ShaderResourceView* g_EnemySensor_Texture = NULL;	//エネミーのセンサーのテクスチャ

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



	b2PolygonShape shape;                         //shapeには色々な型がある　サークルとかもあるよ
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//あたり判定を登録する4点　*0.5するのは

	b2FixtureDef fixture;
	fixture.shape = &shape;    //シャープをフィクスチャに登録する
	fixture.density = 1.0f;    //密度
	fixture.friction = 0.1f;  //摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

	//====================================================================================================
	//センサーの登録
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

	b2Fixture* enemy_static_fixture = GetBody()->CreateFixture(&fixture);//Bodyにフィクスチャを登録する
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
	SetID(ID);

	m_state = ENEMY_STATE_MOVE;
}

void EnemyDynamic::Initialize()
{
	g_EnemyDynamic_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");//動的エネミーのテクスチャ
	g_EnemySensor_Texture=InitTexture(L"asset\\texture\\sample_texture\\xxx_enemy_sensor.png");//エネミーのセンサーのテクスチャ
}

void EnemyDynamic::Finalize()
{

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetBody() != nullptr)
	{
		world->DestroyBody(GetBody());
	}
	UnInitTexture(g_EnemyDynamic_Texture);
	UnInitTexture(g_EnemySensor_Texture);

	//オブジェクトマネージャー内のエネミー削除
	ObjectManager& object_manager = ObjectManager::GetInstance();
	object_manager.DestroyEnemyDynamic(GetID());


	world->DestroyBody(GetBody());
	SetBody(nullptr);
}

void EnemyDynamic::Update()
{
	if (GetUse() && GetInScreen())
	{
		switch (GetState())
		{
		case ENEMY_STATE_MOVE:
			Move();
			break;
		case ENEMY_STATE_ATTACK:
			Attack();
			break;
		case ENEMY_STATE_DESTROYED:
			break;
		default:
			break;
		}
	}
	else if (!GetUse())
	{
		//ワールドに登録したbodyの削除
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());

		//オブジェクトマネージャー内のエネミー削除
		ObjectManager& object_manager = ObjectManager::GetInstance();
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

	//貼るテクスチャを指定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyDynamic_Texture);



	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ GetSize().x * scale , GetSize().y * scale }
	);


	//============================================================
	//テスト:センサー描画
	//貼るテクスチャを指定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemySensor_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ m_size_sensor.x * scale , m_size_sensor.y * scale }
	);
}

//移動
void EnemyDynamic::Move()
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

	//プレイヤーの方向を向く
	if (enemy_vector.x > 0)
	{
		SetDirection(false);
	}
	else
	{
		SetDirection(true);
	}

	enemy_vector.Normalize();

	//移動量
	b2Vec2 enemy_move;
	enemy_move.x = (enemy_vector.x * m_speed) / 5;
	enemy_move.y = (enemy_vector.y * m_speed) / 5;

	if (GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0))
	{
		GetBody()->ApplyLinearImpulseToCenter(b2Vec2(enemy_move.x, 0.0), true);
	}
	else
	{
		GetBody()->ApplyLinearImpulseToCenter(b2Vec2(enemy_move.x, -0.02), true);
	}
}

//攻撃
void EnemyDynamic::Attack()
{
	//攻撃中は停止
	GetBody()->SetType(b2_staticBody);

	//攻撃動作が全て終了したら、動くように戻して移動状態にする
	if (m_attack_counter > m_attack_finish)
	{
		GetBody()->SetType(b2_dynamicBody);
		SetState(ENEMY_STATE_MOVE);
		return;
	}

	//攻撃発生フレームになったら攻撃を生成
	if (m_attack_counter == m_attack_birth)
	{
		ObjectManager& object_manager = ObjectManager::GetInstance();
		b2Vec2 pos = GetBody()->GetPosition();
		if (GetDirection())
		{
			object_manager.AddEnemyAttack(b2Vec2(pos.x - GetSize().x / 4, pos.y), GetSize(), 0.0f);
		}
		else
		{
			object_manager.AddEnemyAttack(b2Vec2(pos.x + GetSize().x / 4, pos.y), GetSize(), 0.0f);
		}
	}

	//フレームカウント
	m_attack_counter++;
}

//センサーとプレイヤーが触れた時の処理
void EnemyDynamic::CollisionSensorPlayer()
{
	//エネミーが攻撃中なら何もしない
	if (GetState() == ENEMY_STATE_ATTACK)
	{
		return;
	}

	//攻撃状態に移行
	SetState(ENEMY_STATE_ATTACK);
	//フレームのカウンターを0に
	m_attack_counter = 0;
}