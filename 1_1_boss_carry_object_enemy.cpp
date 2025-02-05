//-----------------------------------------------------------------------------------------------------
// #name boss_carry_object_enemy.cpp
// #description　ボスの上空をプカプカしてるエネミーオブジェクトを持ってて落とせる
// #make 2025/02/01　永野義也
// #update 2025/02/01
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------

#include"1_1_boss_carry_object_enemy.h"
#include "world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"sprite.h"
#include"create_filter.h"

static ID3D11ShaderResourceView* g_Enemy_Texture = NULL;//エネミーのテクスチャ
static ID3D11ShaderResourceView* g_Object_Texture = NULL;//エネミーのテクスチャ


boss_carry_object_enemy::boss_carry_object_enemy(b2Vec2 position,b2Vec2 Enemy_size,bool left, float Enemy_speed,b2Vec2 Object_size, int Object_type,int anchor_level)
{

	if (g_Enemy_Texture == NULL)
	{
		g_Enemy_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_floating .png");
		g_Object_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_one_way_platform.png");//オブジェクトのテクスチャ
	}
	// ワールドのインスタンス
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	left_flag = left;
	enemy_speed= Enemy_speed;

	object_type = Object_type;

	//分割数を登録しておく
	Splitting_x = Object_size.x;
	Splitting_y = Object_size.y;

	//サイズを登録
	SetEnemySize(Enemy_size);
	SetObjectSize(Object_size);

	// サイズの補正
	b2Vec2 object_size;
	b2Vec2 enemy_size;

	object_size.x = Object_size.x / BOX2D_SCALE_MANAGEMENT;
	object_size.y = Object_size.y / BOX2D_SCALE_MANAGEMENT;

	enemy_size.x = Enemy_size.x / BOX2D_SCALE_MANAGEMENT;
	enemy_size.y = Enemy_size.y / BOX2D_SCALE_MANAGEMENT;

	b2Vec2 pos = position; 

	// === EnemyBody の作成 ===
	b2BodyDef enemyBodyDef;
	enemyBodyDef.type = b2_dynamicBody;
	enemyBodyDef.position.Set(pos.x, pos.y);
	enemyBodyDef.gravityScale = (0.0f);
	b2Body* enemyBody = world->CreateBody(&enemyBodyDef);
	SetEnemyBody(enemyBody);

	b2PolygonShape enemyShape;
	enemyShape.SetAsBox(enemy_size.x * 0.5f, enemy_size.y * 0.5f);

	b2FixtureDef enemyFixtureDef;
	enemyFixtureDef.shape = &enemyShape;
	enemyFixtureDef.density = 1.0f;
	enemyFixtureDef.friction = 0.3f;
	enemyFixtureDef.isSensor = true;
	b2Fixture* enemy_fixture = enemyBody->CreateFixture(&enemyFixtureDef);

	// カスタムデータを作成して設定
	ObjectData* enemy_data = new ObjectData{ collider_object_carry_enemy };
	enemy_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(enemy_data);

	int ID = enemy_data->GenerateID();
	SetID(ID);

	enemy_data->id = GetID();



	// === ObjectBody の作成 ===
	b2BodyDef objectBodyDef;
	objectBodyDef.type = b2_dynamicBody;
	objectBodyDef.position.Set(pos.x, pos.y + (object_size.y / 2) + (enemy_size.y / 2));
	objectBodyDef.gravityScale = (0.0f);
	b2Body* objectBody = world->CreateBody(&objectBodyDef);
	SetObjectBody(objectBody);

	b2PolygonShape objectShape;
	objectShape.SetAsBox(object_size.x * 0.5f, object_size.y * 0.5f);

	b2FixtureDef objectFixtureDef;
	objectFixtureDef.shape = &objectShape;
	objectFixtureDef.density = 1.0f;
	objectFixtureDef.friction = 0.3f;
	objectFixtureDef.isSensor = true;

	b2Fixture* object_fixture = objectBody->CreateFixture(&objectFixtureDef);
	// カスタムデータを作成して設定
	ObjectData* object_data = new ObjectData{ collider_anchor_point };
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	object_data->need_anchor_level = anchor_level;
	object_data->id = GetID();
	object_data->object_name = Boss_Carry_Object_Enemy;





	// === ジョイントの作成 (Weld Joint) ===
	b2WeldJointDef weldJointDef;
	weldJointDef.bodyA = enemyBody;
	weldJointDef.bodyB = objectBody;
	weldJointDef.localAnchorA.Set(0.0f, enemy_size.y * 0.5f); // エネミーの下
	weldJointDef.localAnchorB.Set(0.0f, -object_size.y * 0.5f); // オブジェクトの上
	weldJointDef.collideConnected = false;  // ジョイントした物体同士の衝突を防ぐ
	world->CreateJoint(&weldJointDef);

	

	isUse = true;
}


void boss_carry_object_enemy::Destroy_Splitting()
{
	if (Splitting_Destroy_Flag == true)//破壊のフラグがオンになっている
	{
		

		if (object_body != nullptr && Splitting_end == false)
		{
			//ワールドのインスタンスを持ってくる
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();


			//破壊されたpositionを取得


			//普通のボディも消す
			b2Vec2 Destroy_position = object_body->GetPosition();
			float angle = object_body->GetAngle(); // 元のボディの角度を取得
			b2Vec2 vec = object_body->GetLinearVelocity();
			float angle_vec = object_body->GetAngularVelocity();

			world->DestroyBody(object_body);
			object_body = nullptr;

			SetObjectBody(nullptr);


			//アンカーポイントのボディも消す

		

			b2Vec2 size;
			size.x = GetObjectSize().x / BOX2D_SCALE_MANAGEMENT;
			size.y = GetObjectSize().y / BOX2D_SCALE_MANAGEMENT;



			// 分割後のボディを配置
			for (int y = 0; y < Splitting_y; y++)
			{
				for (int x = 0; x < Splitting_x; x++)
				{
					// 分割後のボディのローカル座標を計算
					float localX = ((x - (Splitting_x - 1) / 2.0f) * size.x / Splitting_x);
					float localY = ((y - (Splitting_y - 1) / 2.0f) * size.y / Splitting_y);

					// 元の角度を考慮してワールド座標に変換
					float rotatedX = localX * cos(angle) - localY * sin(angle);
					float rotatedY = localX * sin(angle) + localY * cos(angle);

					b2Vec2 fragmentPosition(
						Destroy_position.x + rotatedX,
						Destroy_position.y + rotatedY
					);

					// 分割後のボディを作成
					b2BodyDef fragmentDef;
					fragmentDef.type = b2_dynamicBody;
					fragmentDef.position = fragmentPosition;
					fragmentDef.angle = angle; // 元のボディの角度を引き継ぐ

					b2Body* fragment = world->CreateBody(&fragmentDef);
					boss_Object_body_Splitting.push_back(fragment);

					fragment->SetLinearVelocity(b2Vec2(vec.x * 2, vec.y * 2));
					fragment->SetAngularVelocity(angle_vec);

					// 分割後の形状とフィクスチャを設定
					b2PolygonShape fragmentShape;
					fragmentShape.SetAsBox(size.x / (2.0f * Splitting_x), size.y / (2.0f * Splitting_y));

					b2FixtureDef fragmentFixture;
					fragmentFixture.shape = &fragmentShape;
					fragmentFixture.density = 1.0f; // ボディの密度を設定。密度が大きいほどボディの質量が重くなる。
					fragmentFixture.friction = 0.5f; // 摩擦係数を設定。接触面の滑りやすさを制御し、小さい値ほど滑りやすい。
					fragmentFixture.restitution = 0.0f; // 反発係数を設定。0は反発しない（衝突時にエネルギーを失う）、1は完全に弾む。
					fragmentFixture.filter = createFilterExclude("ground_filter", { "Boss_filter","MiniGolem_filter","Shockwave_filter","Player_filter", "object_filter" });

					b2Fixture* fixture = fragment->CreateFixture(&fragmentFixture);

					// カスタムデータを作成して設定
					ObjectData* object_anchorpoint_data = new ObjectData{ collider_ground };
					fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);


					// 初速度はゼロに設定（必要に応じて速度を追加可能）
					fragment->SetLinearVelocity(b2Vec2(0.0f, 0.0f));


					// ランダムな方向に飛び散るように速度を設定
					fragment->SetLinearVelocity(GetRandomVelocity(5.0f)); // 5.0f は基準速度（調整可）

				}
			}

			Splitting_Destroy_Flag = false;
			Splitting_end = true;
		}




	}

}

boss_carry_object_enemy::~boss_carry_object_enemy()
{

}

void boss_carry_object_enemy::Initialize()
{
	
}

void boss_carry_object_enemy::Update()
{
	if (isUse)
	{
		if (enemy_body != nullptr && object_body != nullptr)
		{
			// 移動方向を決定（左: 負、右: 正）
			float moveSpeed = left_flag ? -fabs(enemy_speed) : fabs(enemy_speed);


			// エネミーを移動
			enemy_body->SetLinearVelocity(b2Vec2(moveSpeed, enemy_body->GetLinearVelocity().y));

			// オブジェクトをエネミーの位置に追従させる
			b2Vec2 enemyPos = enemy_body->GetPosition();
			b2Vec2 objectPos = object_body->GetPosition();

			object_body->SetTransform(b2Vec2(enemyPos.x, objectPos.y), 0);
		}

		if (Anchor_Hit_flag == true)
		{
			AnchorHit();
			Anchor_Hit_flag = false;
		}

		if (Splitting_end == true)
		{
			Destroy_Cnt++;
		}
		if (180 < Destroy_Cnt)//分解したあと破壊されるフラグ
		{
			DestroySplittedBodies(boss_Object_body_Splitting);
			isUse = false;
		}

		// 30秒（1800フレーム）経過後にボディ削除
		lifetime++;
		if (lifetime > 1800)
		{
			Destroy_Body();
		}
	}

	Destroy_Splitting();

}

void boss_carry_object_enemy::AnchorHit()
{
	
		b2Body* enemyBody = GetEnemyBody();
		b2Body* objectBody =GetObjectBody();


		// ワールドのインスタンス取得
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		b2JointEdge* jointEdge = enemyBody->GetJointList();
		while (jointEdge)
		{
			b2Joint* joint = jointEdge->joint;
			jointEdge = jointEdge->next;
			world->DestroyJoint(joint);
		}

		// --- 既存のフィクスチャを削除 ---
		for (b2Fixture* f = enemyBody->GetFixtureList(); f;)
		{
			b2Fixture* next = f->GetNext();
			enemyBody->DestroyFixture(f);
			f = next;
		}

		for (b2Fixture* f = objectBody->GetFixtureList(); f;)
		{
			b2Fixture* next = f->GetNext();
			objectBody->DestroyFixture(f);
			f = next;
		}

		// --- 新しいフィクスチャを作成 ---
		b2PolygonShape newEnemyShape;
		newEnemyShape.SetAsBox(GetEnemySize().x/ BOX2D_SCALE_MANAGEMENT * 0.5f, GetEnemySize().y/ BOX2D_SCALE_MANAGEMENT * 0.5f);

		b2FixtureDef newEnemyFixtureDef;
		newEnemyFixtureDef.shape = &newEnemyShape;
		newEnemyFixtureDef.density = 1.0; // 変更例
		newEnemyFixtureDef.friction = 0.5f;
		newEnemyFixtureDef.isSensor = false;

		b2Fixture* newEnemyFixture = enemyBody->CreateFixture(&newEnemyFixtureDef);

		// --- カスタムデータの更新 ---
		ObjectData* enemyData = new ObjectData{ collider_object };
		newEnemyFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(enemyData);
		enemyData->id = GetID();
		enemyData->object_name = Boss_Carry_Object_Enemy;

		// --- ObjectBodyの再設定 ---
		b2PolygonShape newObjectShape;
		newObjectShape.SetAsBox(GetObjectSize().x / BOX2D_SCALE_MANAGEMENT * 0.5f, GetObjectSize().y / BOX2D_SCALE_MANAGEMENT * 0.5f);

		b2FixtureDef newObjectFixtureDef;
		newObjectFixtureDef.shape = &newObjectShape;
		newObjectFixtureDef.density = 1.0f; // 変更例
		newObjectFixtureDef.friction = 0.4f;
		newObjectFixtureDef.isSensor = false;

		b2Fixture* newObjectFixture = objectBody->CreateFixture(&newObjectFixtureDef);

		// --- カスタムデータの更新 ---
		ObjectData* objectData = new ObjectData{ collider_object };
		objectData-= GetID();
		objectData->object_name = Boss_Carry_Object_Enemy;
		newObjectFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(objectData);
		objectData->id = GetID();

		// === グラヴィティスケールの変更 ===
		enemyBody->SetGravityScale(0.0f);  // 変更後の値（デフォルト1.0）
		objectBody->SetGravityScale(1.0f); // 変更後の値（デフォルト1.0）

	
		
	

}



void boss_carry_object_enemy::Draw()
{
	if (isUse)
	{
		// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
		float scale = SCREEN_SCALE;

		// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;
		//エネミーのボディの描画処理
		if (enemy_body != nullptr)
		{


			b2Vec2 Pos = GetEnemyBody()->GetPosition();


			float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Enemy_Texture);


			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetEnemyBody()->GetAngle(),
				{ GetEnemySize().x * scale ,GetEnemySize().y * scale }
			);
		}

		//オブジェクトの描画処理
		if (GetObjectBody() != nullptr)
		{


			b2Vec2 ObjectPos = GetObjectBody()->GetPosition();


			float object_draw_x = ((ObjectPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float object_draw_y = ((ObjectPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Object_Texture);


			//draw
			DrawSprite(
				{ object_draw_x,
				  object_draw_y },
				GetObjectBody()->GetAngle(),
				{ GetObjectSize().x * scale ,GetObjectSize().y * scale }
			);
		}

		//オブジェクトの描画的処理

			//分割後の描画
		if (Splitting_end == true)
		{

			for (int i = 0; i < Splitting_x * Splitting_y; i++)
			{


				b2Vec2 bodyPos = boss_Object_body_Splitting[i]->GetPosition();


				float body_draw_x = ((bodyPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float body_draw_y = ((bodyPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Object_Texture);


				//draw
				DrawSplittingSprite(
					{ body_draw_x,
					body_draw_y },
					boss_Object_body_Splitting[i]->GetAngle(),
					{ GetObjectSize().x / Splitting_x * scale,GetObjectSize().y / Splitting_y * scale },
					Splitting_x,
					Splitting_y,
					i,
					1.0f
				);

			}
		}
	}
}

void boss_carry_object_enemy::DestroySplittedBodies(std::vector<b2Body*>& bodyList) {
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	for (b2Body*& body : bodyList) {
		if (body != nullptr) {
			world->DestroyBody(body);
			body = nullptr; // ポインタを無効化
		}
	}
}




void boss_carry_object_enemy::Finalize()
{
	if (g_Enemy_Texture != NULL)
	{
		UnInitTexture(g_Enemy_Texture);
		UnInitTexture(g_Object_Texture);

		g_Enemy_Texture = NULL;
		g_Object_Texture = NULL;
	}
}

void boss_carry_object_enemy::Destroy_Body()
{
	if (enemy_body == nullptr && object_body == nullptr) return;

	// ワールドのインスタンスを取得
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// ボディの削除
	if (enemy_body)
	{
		world->DestroyBody(enemy_body);
		enemy_body = nullptr;
	}
	if (object_body)
	{
		world->DestroyBody(object_body);
		object_body = nullptr;
	}

	// 無効化
	isUse = false;
}