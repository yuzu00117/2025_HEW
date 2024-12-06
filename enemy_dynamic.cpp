//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のcppファイル
// #make 2024/11/20
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ステータス調整
//			・必要に応じた移動方法ノ変更(地走バージョンノ作成など)
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

EnemyDynamic* g_p_enemies_dynamic[ENEMY_MAX] = { nullptr };

EnemyDynamic::EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture)
	:Enemy(ENEMY_DYNAMIC_LIFE, ENEMY_DYNAMIC_DAMAGE, ENEMY_DYNAMIC_SOULGAGE, ENEMY_DYNAMIC_SCORE, true)
{
	//テクスチャをセット
	SetFieldTexture(texture);


	b2BodyDef body;
	body.type = b2_dynamicBody;							//静的なオブジェクトにするならture
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる
	SetFieldBody(world->CreateBody(&body));				//Bodyをワールドに固定


	SetSize(body_size);//表示用にサイズをセットしとく、表示のときにGetSizeを呼び出す



	b2Vec2 size;
	size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;                         //shapeには色々な型がある　サークルとかもあるよ
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//あたり判定を登録する4点　*0.5するのは

	b2FixtureDef fixture;
	fixture.shape = &shape;    //シャープをフィクスチャに登録する
	fixture.density = 1.0f;    //密度
	fixture.friction = 0.05f;  //摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = false;  //センサーかどうか、trueならあたり判定は消える

	b2Fixture* enemy_dynamic_fixture = GetFieldBody()->CreateFixture(&fixture);//Bodyにフィクスチャを登録する

	// カスタムデータを作成して設定
	// 動的エネミーに値を登録
	// 動的エネミーにユーザーデータを登録
	ObjectData* data = new ObjectData{ collider_enemy_dynamic };
	enemy_dynamic_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (!g_p_enemies_dynamic[i])
		{
			g_p_enemies_dynamic[i] = this;
			return;
		}
	}
}

void EnemyDynamic::Update()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_dynamic[i])
		{
			if(g_p_enemies_dynamic[i]->GetUse())
			{
				g_p_enemies_dynamic[i]->UpdateEnemy();
			}
			else
			{
				//ワールドに登録したbodyの削除(追加予定)
				Box2dWorld& box2d_world = Box2dWorld::GetInstance();
				b2World* world = box2d_world.GetBox2dWorldPointer();
				world->DestroyBody(g_p_enemies_dynamic[i]->GetFieldBody());

				Field::DeleteFieldObject(g_p_enemies_dynamic[i]->GetFieldBody());
				g_p_enemies_dynamic[i] = nullptr;
			}
		}
	}
}

void EnemyDynamic::UpdateEnemy()
{
	//プレイヤー追従(簡易)
	//プレイヤーのポジション取得
	b2Vec2 player_position;
	player_position.x = PlayerPosition::GetPlayerPosition().x;
	player_position.y = PlayerPosition::GetPlayerPosition().y;

	//移動方向
	b2Vec2 enemy_vector;
	enemy_vector.x = player_position.x - GetFieldBody()->GetPosition().x;
	enemy_vector.y = player_position.y - GetFieldBody()->GetPosition().y;

	//移動量
	b2Vec2 enemy_move;
	enemy_move.x = (enemy_vector.x * m_speed);
	enemy_move.y = (enemy_vector.y * m_speed);

	GetFieldBody()->ApplyForceToCenter(b2Vec2(enemy_move.x, enemy_move.y), true);
}

void EnemyDynamic::CollisionPlayer(b2Body* collision_enemy)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_dynamic[i])
		{
			if (g_p_enemies_dynamic[i]->GetFieldBody() == collision_enemy)
			{
				PlayerStamina::EditPlayerStaminaValue(-g_p_enemies_dynamic[i]->GetDamage());
				g_p_enemies_dynamic[i]->SetUse(false);
				return;
			}
		}
	}
}

void EnemyDynamic::CollisionPulledObject(b2Body* collision_enemy)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_dynamic[i])
		{
			if (g_p_enemies_dynamic[i]->GetFieldBody() == collision_enemy)
			{
				AnchorSpirit::EditAnchorSpiritValue(-g_p_enemies_dynamic[i]->GetDamage());
				g_p_enemies_dynamic[i]->SetUse(false);
				return;
			}
		}
	}
}

void EnemyDynamic::Finalize()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_dynamic[i])
		{
			g_p_enemies_dynamic[i] = nullptr;
		}
	}
}