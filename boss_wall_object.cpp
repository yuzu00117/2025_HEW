//-----------------------------------------------------------------------------------------------------
// #name boss_wall_object.h
// #description ボス戦で使う壁のオブジェクト
// #make 2025/02/13		永野義也
// #update 2025/02/13
// #comment 追加・修正予定
//----------------------------------------------------------------------------------------------------




#include"boss_wall_object.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"
#include"sound.h"
#include"anchor_spirit.h"
#include"1-1_boss.h"


Boss_Wall_Objcet::Boss_Wall_Objcet(b2Vec2 position, b2Vec2 size, int splitting_x, int splitting_y,ID3D11ShaderResourceView* g_Texture,bool left)
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
	body.gravityScale=(0.0);

	b2Body* m_Body = world->CreateBody(&body);

	SetBody(m_Body);


	//形の定義
	b2CircleShape shape;
	shape.m_radius = body_size.y * 0.4; // 半径を適切に設定


	//-----------------------------------------------------
	//	fixtureを作る
	b2FixtureDef fixture;

	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.5f;
	fixture.restitution = 0.2f;
	fixture.isSensor = true;
	fixture.filter = createFilterExclude("object_filter", {"Player_filter"});

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// 新しいフィルターを作成


	//カスタムデータを作成
	ObjectData* object_data = new ObjectData{ collider_object };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = Boss_Wall;
	SetID(ID);

	Texture=g_Texture;
	isUse = true;

	left_flag =left;
}

Boss_Wall_Objcet::~Boss_Wall_Objcet()
{
}



void Boss_Wall_Objcet::Initialize()
{

}

void Boss_Wall_Objcet::Update()
{
	if (isUse)
	{
		
		if (m_body != nullptr)
		{


			if (AnchorSpirit::GetAnchorLevel() == 3)
			{
				//アンカーポイントを作る
				CreateAnchorPoint();
			}
			else
			{
				//アンカーポイントを削除する
				DeleteAnchorPoint();
			}

			if (Splitting_end == true)
			{
				Destroy_Cnt++;
			}

			if (180 < Destroy_Cnt)//分解したあと破壊されるフラグ
			{
				DestroySplittedBodies(boss_pillar_body_Splitting);
				isUse = false;
			}



			if (GetrPullingFlag() == true)
			{
				ChangeBody();
				SetPullingFlag(false);
			}

			if (move_flag == true)
			{
				
				WallPullling();
				pulling_cnt++;
			}

			if (120 < pulling_cnt)
			{
				Splitting_Destroy_Flag = true;
			}

			Destroy_Splitting();
		}
		
	}
}


void Boss_Wall_Objcet::Destroy_Splitting()
{
	if (Splitting_Destroy_Flag == true)//破壊のフラグがオンになっている
	{
		if (m_body != nullptr && Splitting_end == false)
		{
			//ワールドのインスタンスを持ってくる
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();


			//破壊されたpositionを取得


			//普通のボディも消す
			b2Vec2 Destroy_position = m_body->GetPosition();
			float angle = m_body->GetAngle(); // 元のボディの角度を取得
			b2Vec2 vec = m_body->GetLinearVelocity();
			float angle_vec = m_body->GetAngularVelocity();

			world->DestroyBody(m_body);
			m_body = nullptr;

			SetBody(nullptr);

			//サウンドを再生
			app_atomex_start(Object_Pillar_Break_Sound);

			//アンカーポイントのボディも消す

			if (GetObjectAnchorPointBody() != nullptr)
			{
				world->DestroyBody(GetObjectAnchorPointBody());
			}

			b2Vec2 size;
			size.x = m_size.x / BOX2D_SCALE_MANAGEMENT;
			size.y = m_size.y / BOX2D_SCALE_MANAGEMENT;



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
					boss_pillar_body_Splitting.push_back(fragment);

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
					fragmentFixture.filter = createFilterExclude("ground_filter", { "Boss_filter","MiniGolem_filter","Shockwave_filter","Player_filter", "object_filter","ground_filter" });

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

void Boss_Wall_Objcet::CreateAnchorPoint()
{

	if (GetObjectAnchorPointBody() == nullptr)
	{
		b2Vec2 size = b2Vec2(1.0f, 1.0f);

	

		b2Vec2 anchorpoint_size;

		anchorpoint_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
		anchorpoint_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

		float left = -1;
		if (left_flag == false)
		{
			left = 1;
		}

		b2Vec2 position = m_body->GetPosition();

		b2BodyDef anchor_point_body;
		anchor_point_body.type = b2_dynamicBody;																   // 動的なオブジェクトにする
		anchor_point_body.position.Set(GetBody()->GetPosition().x+(GetSize().x/BOX2D_SCALE_MANAGEMENT/2)*left, GetBody()->GetPosition().y); // ポジションを設定
		anchor_point_body.angle = 0;																			   // 角度の初期化
		anchor_point_body.userData.pointer = (uintptr_t)this;													   // userDataのポインタを設定
		anchor_point_body.fixedRotation = true;																	   // 回転を固定する

		Box2dWorld& box2d_world = Box2dWorld::GetInstance(); // ワールドのインスタンスを取得する
		b2World* world = box2d_world.GetBox2dWorldPointer(); // ワールドのポインタを取得する

		b2Body* m_anchor_point_body = world->CreateBody(&anchor_point_body); // Bodyをワールドに追加

		SetObjectAnchorPointBody(m_anchor_point_body);

		b2PolygonShape shape;												  // shapeには四角形を設定する
		shape.SetAsBox(anchorpoint_size.x * 0.5f, anchorpoint_size.y * 0.5f); // 四角形を設定する

		b2FixtureDef fixture;
		fixture.shape = &shape;		// シェイプをフィクスチャに設定する
		fixture.density = 1.0f;		// 密度
		fixture.friction = 0.00f;	// 摩擦
		fixture.restitution = 0.0f; // 反発係数
		fixture.isSensor = false;	// センサーかどうか

		b2Fixture* ground_fixture = m_anchor_point_body->CreateFixture(&fixture); // Bodyにフィクスチャを設定する

		// カスタムデータを作成して設定
		// 名前に値を設定
		// 名前にユーザーデータを設定
		ObjectData* data = new ObjectData{ collider_anchor_point };
		ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

		data->need_anchor_level = 3;
		data->id = GetID();

		data->object_name = Boss_Wall;

		// プレイヤーとジョイントする
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;			   // 壁の本体
		jointDef.bodyB = GetObjectAnchorPointBody(); // ボスのアンカーポイントのボディ

		// 壁がわ
		jointDef.localAnchorA.Set(GetSize().x/BOX2D_SCALE_MANAGEMENT*0.5*left, 0.0f);
		// アンカーポイント側
		jointDef.localAnchorB.Set(0.0f, 0.0f);

		jointDef.collideConnected = true; // ジョイント同士の衝突を無効にする

		world->CreateJoint(&jointDef); // ワールドにジョイントを追加
	}
}

void Boss_Wall_Objcet::DeleteAnchorPoint()
{
	// アンカーポイントボディを削除
	if (GetObjectAnchorPointBody() != nullptr)
	{
		Box2dWorld& box2d_world = Box2dWorld::GetInstance(); // ワールドのインスタンスを取得する
		b2World* world = box2d_world.GetBox2dWorldPointer(); // ワールドのポインタを取得する

		world->DestroyBody(GetObjectAnchorPointBody());

		// nullに設定
		SetObjectAnchorPointBody(nullptr);
	}
}


void Boss_Wall_Objcet::DestroySplittedBodies(std::vector<b2Body*>& bodyList) {
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


void Boss_Wall_Objcet::ChangeBody()
{
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	m_body->SetType(b2_dynamicBody);



	move_flag = true;
}


void Boss_Wall_Objcet::WallPullling()
{
	if (m_body != nullptr)
	{
		int minus = 1;

		if (left_flag)
		{
			minus = -1;
		}
	
		m_body->SetLinearVelocity(b2Vec2(50.0f * minus, 0.0f));
		
	}
}

void Boss_Wall_Objcet::Draw()
{
	if (isUse)
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

			GetDeviceContext()->PSSetShaderResources(0, 1, &Texture);


			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				0.0f,
				{ (GetSize().x+6.f) * scale ,GetSize().y * scale*1.2f }
			);
		}


		//分割後の描画
		if (Splitting_end == true)
		{

			for (int i = 0; i < Splitting_x * Splitting_y; i++)
			{


				b2Vec2 bodyPos = boss_pillar_body_Splitting[i]->GetPosition();


				float body_draw_x = ((bodyPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float body_draw_y = ((bodyPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				GetDeviceContext()->PSSetShaderResources(0, 1, &Texture);


				//draw
				DrawSplittingSprite(
					{ body_draw_x,
					body_draw_y },
					boss_pillar_body_Splitting[i]->GetAngle(),
					{ GetSize().x / Splitting_x * scale,GetSize().y / Splitting_y * scale },
					Splitting_x,
					Splitting_y,
					i,
					1.0f
				);

			}
		}
	}

}

void Boss_Wall_Objcet::Finalize()
{
	if (Texture) {
		UnInitTexture(Texture);
	}
}