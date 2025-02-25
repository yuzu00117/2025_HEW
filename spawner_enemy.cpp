//-----------------------------------------------------------------------------------------------------
// #name spawner_enemy.cpp
// #description スポナーのソースファイル
// #make 2025/02/21
// #update 2025/02/24
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#include "spawner_enemy.h"
#include"include/box2d/box2d.h"
#include"enemy_dynamic.h"
#include"world_box2d.h"
#include"texture.h"
#include"sprite.h"
#include"contactlist.h"
#include<Windows.h>
#include"create_filter.h"
#include"object_manager.h"
#include"player_position.h"

static ID3D11ShaderResourceView* g_EnemySpawner_Texture;		  //動的エネミーのテクスチャ

SpawnerEnemy::SpawnerEnemy(b2Vec2 position, b2Vec2 size, float angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;							//静的なオブジェクトにするならture
	body.position.Set(position.x, position.y);			//ポジションをセット
	body.angle = angle;									//角度の定義
	body.userData.pointer = (uintptr_t)this;			//userDataのポインタを定義 
	body.fixedRotation = true;							//回転を固定する、　これをオンにすると回転しない


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//ワールドのインスタンスを取得する
	b2World* world = box2d_world.GetBox2dWorldPointer();//ワールドのポインタを持ってくる

	SetBody(world->CreateBody(&body));//Bodyをワールドに固定


	SetSize(size);//表示用にサイズをセットしとく、表示のときにGetSizeを呼び出す

	size.x = size.x / BOX2D_SCALE_MANAGEMENT;//サイズを１にすると　1m*1mになるため　サイズをさげて、物理演算の挙動を操作しやすくする
	size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	b2PolygonShape shape;                         //shapeには色々な型がある　サークルとかもあるよ
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//あたり判定を登録する4点　*0.5するのは

	b2FixtureDef fixture;
	fixture.shape = &shape;    //シャープをフィクスチャに登録する
	fixture.density = 0.1f;    //密度
	fixture.friction = 0.05f;  //摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = true;  //センサーかどうか、trueならあたり判定は消える

	//Bodyにフィクスチャを登録する
	b2Fixture* enemy_static_fixture = GetBody()->CreateFixture(&fixture);

	// カスタムデータを作成して設定
	ObjectData* data = new ObjectData{ collider_spawner_enemy };
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

	//ID登録
	data->object_name = Object_Spawner_Enemy;
	int ID = data->GenerateID();
	data->id = ID;
	SetID(ID);
}

void SpawnerEnemy::Initialize()
{
	if(!g_EnemySpawner_Texture)
	{
		g_EnemySpawner_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_spawner.png");
	}
}

void SpawnerEnemy::Finalize()
{
	if (g_EnemySpawner_Texture)
	{
		UnInitTexture(g_EnemySpawner_Texture);
		g_EnemySpawner_Texture = NULL;
	}

	//ワールドに登録したbodyの削除
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	world->DestroyBody(GetBody());
	SetBody(nullptr);
}

void SpawnerEnemy::Update()
{
	if(m_in_screen)
	{
		m_spawn_counter++;
		m_spawn_counter %= m_spawn_space;

		if (m_spawn_counter == 0)
		{
			ObjectManager& object_manager = ObjectManager::GetInstance();
			b2Vec2 pos = GetBody()->GetPosition();
			object_manager.AddEnemyDynamic(pos, b2Vec2(1.0f, 1.0f), 0.0f);
		}
	}
}

void SpawnerEnemy::Draw()
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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemySpawner_Texture);

	DrawSprite(
		{ draw_x,
		  draw_y },
		0,
		{ GetSize().x * scale,GetSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);
}