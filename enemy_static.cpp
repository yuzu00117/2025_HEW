//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 静的エネミーのcppファイル
// #make 2024/11/19
// #update 2024/12/04
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

EnemyStatic* g_p_enemies_static[ENEMY_MAX] = { nullptr };

EnemyStatic::EnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture)
	:Enemy(ENEMY_STATIC_LIFE, ENEMY_STATIC_DAMAGE, ENEMY_STATIC_SOULGAGE, ENEMY_STATIC_SCORE)
{
	//テクスチャをセット
	SetFieldTexture(texture);


	b2BodyDef body;
	body.type = b2_staticBody;							//静的なオブジェクトにするならture
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	SetFieldBody(world->CreateBody(&body));//Bodyをワールドに固定


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

	b2Fixture* enemy_static_fixture = GetFieldBody()->CreateFixture(&fixture);//Bodyをにフィクスチャを登録する

	// カスタムデータを作成して設定
	// 動的エネミーに値を登録
	// 動的エネミーにユーザーデータを登録
	ObjectData* data = new ObjectData{ collider_enemy_static };
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (!g_p_enemies_static[i])
		{
			g_p_enemies_static[i] = this;
			return;
		}
	}
}

void EnemyStatic::Update()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_static[i])
		{
			g_p_enemies_static[i]->UpdateEnemy();
		}
	}
}

void EnemyStatic::UpdateEnemy()
{

}

void EnemyStatic::CollisionPlayer(b2Body* collision_enemy)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_static[i])
		{
			if (g_p_enemies_static[i]->GetFieldBody() == collision_enemy)
			{
				PlayerStamina::EditPlayerStaminaValue(-g_p_enemies_static[i]->GetDamage());

				Field::DeleteFieldObject(collision_enemy);
				g_p_enemies_static[i] = nullptr;
				return;
			}
		}
	}
}

void EnemyStatic::Finalize()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_static[i])
		{
			g_p_enemies_static[i] = nullptr;
		}
	}
}