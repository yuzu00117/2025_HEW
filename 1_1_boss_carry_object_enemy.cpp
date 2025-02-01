//-----------------------------------------------------------------------------------------------------
// #name boss_carry_object_enemy.cpp
// #description　ボスの上空をプカプカしてるエネミーオブジェクトを持ってて落とせる
// #make 2025/02/01　永野義也
// #update 2025/02/01
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------


#include"1_1_boss_carry_object_enemy.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"

static ID3D11ShaderResourceView* g_Texture = NULL;//スポナーのテクスチャ
static ID3D11ShaderResourceView* g_Enemy_Texture = NULL;//エネミーのテクスチャ
static ID3D11ShaderResourceView* g_Object_Texture = NULL;//エネミーのテクスチャ


boss_carry_object_enemy::boss_carry_object_enemy(b2Vec2 position, b2Vec2 size, Boss_Room_Level level,b2Vec2 enemy_size,b2Vec2 enemy_speed,b2Vec2 max_obejct_size,int object_need_levl)
{
	SetSize(size);//描画用のサイズを保存

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//サイズを調整する
	b2Vec2 body_size;
	body_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	body_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	//ボディを作成する
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(position.x, position.y);
	body.fixedRotation = false;

	b2Body* m_Body = world->CreateBody(&body);

	SetBody(m_Body);


	//形の定義
	b2PolygonShape shape;
	shape.SetAsBox(body_size.x * 0.5, body_size.y * 0.5);


	//-----------------------------------------------------
	//	fixtureを作る
	b2FixtureDef fixture;

	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.01f;
	fixture.restitution = 0.3f;
	fixture.isSensor = true;
	fixture.filter = createFilterExclude("ground_filter", {});

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// 新しいフィルターを作成


	//カスタムデータを作成
	ObjectData* object_data = new ObjectData{ collider_ground };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = NULL_object;
	SetID(ID);


	//各種特殊な変数をセット
	BossRoomLevel = level;

	Enemy_Speed = enemy_speed;

	//enemySize = enemy_size;

	//Max_object_size=max_obejct_size;


	Object_need_level = object_need_levl;

	CreateEnemyBodyandObjectBody(b2Vec2(5.0f, 1.0f), b2Vec2(3.f, 3.f));

	
}

boss_carry_object_enemy::~boss_carry_object_enemy()
{
}



void boss_carry_object_enemy::Initialize()
{
	if (g_Texture == NULL) {
		g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");//グラウンドのテクスチャ
		g_Enemy_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_floating .png");
		g_Object_Texture=InitTexture(L"asset\\texture\\sample_texture\\sample_one_way_platform.png");//オブジェクトのテクスチャ
	}
}

void boss_carry_object_enemy::Update()
{
	Boss_1_1& boss = Boss_1_1::GetInstance();
	if (m_body != nullptr)
	{
		if (boss.GetBossFieldLevel() > BossRoomLevel)
		{
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();
			if (m_body != nullptr)
			{
				world->DestroyBody(m_body);
				SetBody(nullptr);
			}
		}
	}

	EnemyUpdate();




}

void boss_carry_object_enemy::CreateEnemyBodyandObjectBody(b2Vec2 Object_size, b2Vec2 Enemy_size) {

	if (m_body != nullptr)
	{
		// ワールドのインスタンス
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		// サイズの補正
		b2Vec2 object_size;
		b2Vec2 enemy_size;

		object_size.x = Object_size.x / BOX2D_SCALE_MANAGEMENT;
		object_size.y = Object_size.y / BOX2D_SCALE_MANAGEMENT;

		enemy_size.x = Enemy_size.x / BOX2D_SCALE_MANAGEMENT;
		enemy_size.y = Enemy_size.y / BOX2D_SCALE_MANAGEMENT;

		b2Vec2 pos = this->m_body->GetPosition();  // this-> を明示

		// === EnemyBody の作成 ===
		b2BodyDef enemyBodyDef;
		enemyBodyDef.type = b2_dynamicBody;
		enemyBodyDef.position.Set(pos.x, pos.y);
		enemyBodyDef.gravityScale = (0.0f);
		b2Body* enemyBody = world->CreateBody(&enemyBodyDef);

		b2PolygonShape enemyShape;
		enemyShape.SetAsBox(enemy_size.x * 0.5f, enemy_size.y * 0.5f);

		b2FixtureDef enemyFixtureDef;
		enemyFixtureDef.shape = &enemyShape;
		enemyFixtureDef.density = 1.0f;
		enemyFixtureDef.friction = 0.3f;
		enemyFixtureDef.isSensor = true;
		enemyBody->CreateFixture(&enemyFixtureDef);

		// === ObjectBody の作成 ===
		b2BodyDef objectBodyDef;
		objectBodyDef.type = b2_dynamicBody;
		objectBodyDef.position.Set(pos.x, pos.y + (object_size.y / 2) + (enemy_size.y / 2));
		objectBodyDef.gravityScale = (0.0f);
		b2Body* objectBody = world->CreateBody(&objectBodyDef);

		b2PolygonShape objectShape;
		objectShape.SetAsBox(object_size.x * 0.5f, object_size.y * 0.5f);

		b2FixtureDef objectFixtureDef;
		objectFixtureDef.shape = &objectShape;
		objectFixtureDef.density = 1.0f;
		objectFixtureDef.friction = 0.3f;
		objectFixtureDef.isSensor = true;
		objectBody->CreateFixture(&objectFixtureDef);

		// === ジョイントの作成 (Weld Joint) ===
		b2WeldJointDef weldJointDef;
		weldJointDef.bodyA = enemyBody;
		weldJointDef.bodyB = objectBody;
		weldJointDef.localAnchorA.Set(0.0f, enemy_size.y * 0.5f); // エネミーの下
		weldJointDef.localAnchorB.Set(0.0f, -object_size.y * 0.5f); // オブジェクトの上
		weldJointDef.collideConnected = false;  // ジョイントした物体同士の衝突を防ぐ
		world->CreateJoint(&weldJointDef);

		// 構造体にまとめて追加（this-> を使用）
		this->enemyObjectPairs.push_back({ enemyBody, objectBody, Enemy_size, Object_size, 0.0f });
	}
}


void boss_carry_object_enemy::EnemyUpdate() {
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// フレームごとにカウントを増加
	spawnTimer++;

	// 10秒ごとに新しいエネミーを作成（60FPS × 10秒 = 600フレーム）
	if (spawnTimer >= spawnIntervalFrames) {
		CreateEnemyBodyandObjectBody(b2Vec2(5.0f, 1.0f), b2Vec2(3.f, 3.f));  // エネミー生成
		spawnTimer = 0;  // タイマーをリセット
	}

	// 移動速度（負の値で左方向へ移動）
	float moveSpeed = -0.009f;

	for (size_t i = 0; i < enemyObjectPairs.size(); ) {
		enemyObjectPairs[i].lifetime++;  // フレームごとにカウントを増加

		// エネミーとオブジェクトを左へ移動
		b2Vec2 enemyPos = enemyObjectPairs[i].enemyBody->GetPosition();
		b2Vec2 objectPos = enemyObjectPairs[i].objectBody->GetPosition();

		enemyObjectPairs[i].enemyBody->SetTransform(b2Vec2(enemyPos.x + moveSpeed, enemyPos.y), 0);
		enemyObjectPairs[i].objectBody->SetTransform(b2Vec2(objectPos.x + moveSpeed, objectPos.y), 0);

		// 一定時間経過で削除
		if (enemyObjectPairs[i].lifetime >= maxLifetimeFrames) {
			world->DestroyBody(enemyObjectPairs[i].enemyBody);
			world->DestroyBody(enemyObjectPairs[i].objectBody);
			enemyObjectPairs.erase(enemyObjectPairs.begin() + i);
		}
		else {
			++i;
		}
	}
}

void boss_carry_object_enemy::Draw()
{
	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	if (m_body != nullptr)
	{
	

		b2Vec2 Pos = GetBody()->GetPosition();


		float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale ,GetSize().y * scale }
		);
	}

	for (const auto& pair : this->enemyObjectPairs) {
		//----------------------------------------------------------
		// エネミーの描画処理

		b2Vec2 posEnemy = pair.enemyBody->GetPosition();

		float Enemy_draw_x = ((posEnemy.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float Enemy_draw_y = ((posEnemy.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Enemy_Texture);

		// Draw
		DrawSprite(
			{ Enemy_draw_x, Enemy_draw_y },
			pair.enemyBody->GetAngle(),
			{ pair.enemySize.x * scale, pair.enemySize.y * scale }
		);

		//----------------------------------------------------------
		// オブジェクトの描画処理

		b2Vec2 posObject = pair.objectBody->GetPosition();

		float Object_draw_x = ((posObject.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float Object_draw_y = ((posObject.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Object_Texture);

		// Draw
		DrawSprite(
			{ Object_draw_x, Object_draw_y },
			pair.objectBody->GetAngle(),
			{ pair.objectSize.x * scale, pair.objectSize.y * scale}
		);

	}

}

void boss_carry_object_enemy::Finalize()
{
	UnInitTexture(g_Texture);
}