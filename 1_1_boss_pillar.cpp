//-----------------------------------------------------------------------------------------------------
// #name 1_1_boss_pillar.cpp
// #description ボス戦で利用する柱のオブジェクト
// #make 2025/01/25		永野義也
// #update 2025/01/25
// #comment 追加・修正予定
//----------------------------------------------------------------------------------------------------



#include"1_1_boss_pillar.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"
#include"sound.h"
#include"camera_shake.h"
#include"anchor_point.h"
#include"anchor.h"

static ID3D11ShaderResourceView* g_Texture = NULL;//フィールドのテクスチャ

boss_pillar::boss_pillar(b2Vec2 position, b2Vec2 size, int splitting_x,int splitting_y,Boss_Room_Level level)
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
	body.type = b2_dynamicBody;
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
	fixture.restitution = 0.0f;
	fixture.isSensor = false;
	fixture.filter = createFilterExclude("object_filter", {"Player_filter","MiniGolem_filter","Shockwave_filter"});

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// 新しいフィルターを作成


	//カスタムデータを作成
	ObjectData* object_data = new ObjectData{ collider_object };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = Boss_pillar;
	SetID(ID);


	//サイズの補正をいれる
	b2Vec2 anchorpoint_size;
	anchorpoint_size.x = body_size.x;
	anchorpoint_size.y = 0.5 / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef anchorpoint_body;//木の幹の部分
	anchorpoint_body.type = b2_dynamicBody;
	anchorpoint_body.position.Set(
		position.x,
		position.y + (body_size.y / 2) + (anchorpoint_size.y / 2));
	anchorpoint_body.fixedRotation = false;

	b2Body* m_AnchorPoint_body = world->CreateBody(&anchorpoint_body);

	SetObjectAnchorPointBody(m_AnchorPoint_body);



	b2PolygonShape anchorpoint_shape;
	anchorpoint_shape.SetAsBox(anchorpoint_size.x * 0.5, anchorpoint_size.y * 0.5);

	b2FixtureDef anchorpoint_fixture;

	anchorpoint_fixture.shape = &anchorpoint_shape;
	anchorpoint_fixture.density = 1.0f;
	anchorpoint_fixture.friction = 0.05f;//摩擦
	anchorpoint_fixture.restitution = 0.0f;//反発係数
	anchorpoint_fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える
	anchorpoint_fixture.filter = createFilterExclude("object_filter", { "Boss_filter","MiniGolem_filter","Shockwave_filter", "object_filter" });

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);


	// カスタムデータを作成して設定
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);


	object_anchorpoint_data->id = ID;


	object_anchorpoint_data->object_name = Boss_pillar;
	object_anchorpoint_data->need_anchor_level = 1;

	b2Vec2 need_power;

	need_power.x = (GetSize().x * GetSize().y);//１は必要に応じて変更して
	need_power.y = 10.0f;//縦に必要な力はない


	object_anchorpoint_data->add_force = need_power;
	m_pulling_power = need_power;

	b2WeldJointDef jointDef;
	jointDef.bodyA = m_Body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(0.0f, -body_size.y * 0.5f); // 木の上端
	jointDef.localAnchorB.Set(0.0f, anchorpoint_size.y * 0.5f); // アンカーポイントの下端
	jointDef.collideConnected = true;					  //ジョイントした物体同士の接触を消す

	world->CreateJoint(&jointDef);						  //ワールドにジョイントを追加

	//分割すうを保存
	Splitting_x = splitting_x;
	Splitting_y = splitting_y;


	boss_room_level = level;

	//ボディの領域を事前に確保しておく
	boss_pillar_body_Splitting.reserve(Splitting_x * Splitting_y);




	isUse = true;
}

boss_pillar::~boss_pillar()
{
}



void boss_pillar::Initialize()
{
	if (g_Texture == NULL) {
		g_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\pillar.png");//柱のテクスチャ
	}
}

void boss_pillar::Update()
{
	if (isUse)
	{
		Boss_1_1& boss = Boss_1_1::GetInstance();
		if (boss_room_level < boss.GetBossFieldLevel() && (Splitting_end == false))
		{
			if (AnchorPoint::GetTargetAnchorPointBody() != m_body)
			{
				if (Anchor::GetAnchorState() != Nonexistent_state)return;
			}

			Splitting_Destroy_Flag = true;
		}

		if (Splitting_end == true)
		{
			Destroy_Cnt++;
		}

		Pulling_pillar();


		if (m_body != nullptr)
		{
			float angle = m_body->GetAngle();  // ラジアンで取得
			float angleDeg = angle * 57.2958f; // 度数法に変換 (180 / π)

			// 角度を -180° ~ 180° の範囲に正規化
			while (angleDeg > 180.0f) angleDeg -= 360.0f;
			while (angleDeg < -180.0f) angleDeg += 360.0f;

			// 横倒しを判定（85°以上 or -85°以下で壊れる）
			if (angleDeg >= 87.0f || angleDeg <= -87.0f)
			{
				// 角度が閾値を超えたらカウント
				angle_delete_cnt++;
			}
			// 角度が閾値を超えたら壊す
			if (10 < angle_delete_cnt)
			{
				Splitting_Destroy_Flag = true;
			}
		}


		Destroy_Splitting();

		if (180 < Destroy_Cnt)//分解したあと破壊されるフラグ
		{
			DestroySplittedBodies(boss_pillar_body_Splitting);
			isUse = false;
		}
	}
}


void boss_pillar::Destroy_Splitting()
{
	if (Splitting_Destroy_Flag == true)//破壊のフラグがオンになっている
	{
		if (m_body != nullptr && Splitting_end == false)
		{
			CameraShake::StartCameraShake(50, 30, 30);


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

			b2Body* m_body = GetObjectAnchorPointBody();
			// ObjectData の削除
			if ((m_body)) {
				for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext()) {
					ObjectData* data = reinterpret_cast<ObjectData*>(f->GetUserData().pointer);
					if (data) {
						delete data;
						f->GetUserData().pointer = 0;
					}
				}
			}

			AnchorPoint::OutsideSensor(GetObjectAnchorPointBody());
			world->DestroyBody(GetObjectAnchorPointBody());
			SetObjectAnchorPointBody(nullptr);

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

					fragment->SetLinearVelocity(b2Vec2(vec.x*2,vec.y*2));
					fragment->SetAngularVelocity(angle_vec);

					// 分割後の形状とフィクスチャを設定
					b2PolygonShape fragmentShape;
					fragmentShape.SetAsBox(size.x / (2.0f * Splitting_x), size.y / (2.0f * Splitting_y));

					b2FixtureDef fragmentFixture;
					fragmentFixture.shape = &fragmentShape;
					fragmentFixture.density = 1.0f; // ボディの密度を設定。密度が大きいほどボディの質量が重くなる。
					fragmentFixture.friction = 0.5f; // 摩擦係数を設定。接触面の滑りやすさを制御し、小さい値ほど滑りやすい。
					fragmentFixture.restitution = 0.0f; // 反発係数を設定。0は反発しない（衝突時にエネルギーを失う）、1は完全に弾む。
					fragmentFixture.isSensor = true;
					/*fragmentFixture.filter = createFilterExclude("ground_filter", {"Boss_filter","MiniGolem_filter","Shockwave_filter","Player_filter", "object_filter","Shockwave_filter" });*/

					b2Fixture*fixture=fragment->CreateFixture(&fragmentFixture);

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


void boss_pillar::DestroySplittedBodies(std::vector<b2Body*>& bodyList) {
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	for (b2Body*& body : bodyList) {
		if (body != nullptr) {
			world->DestroyBody(body);
			body = nullptr; // ポインタを無効化
		}
	}
	bodyList.clear();  // vectorを空にする
	
}

void boss_pillar::Pulling_pillar()
{
	if (pulling_flag == true)
	{

		// 本体のフィクスチャのフィルターを更新する
		if (GetBody() != nullptr) {
			for (b2Fixture* f = GetBody()->GetFixtureList(); f; f = f->GetNext()) {
				// 新しいフィルターデータを作成（createFilterExclude は既存の関数と仮定）
				b2Filter newFilter = createFilterExclude("object_filter", {"Player_filter"});
				f->SetFilterData(newFilter);
				f->Refilter();  // 設定変更をワールドに反映
			}
		}

		b2Body* anchor_body = GetObjectAnchorPointBody();
		b2Body* body = GetBody();
		b2Vec2 pulling_power = m_pulling_power;

		Boss_1_1& boss = Boss_1_1::GetInstance();

		//プレイヤー側に倒す

		if (boss.GetOutSideBody() != nullptr)
		{
			if (boss.GetOutSideBody()->GetPosition().x < anchor_body->GetPosition().x)//プレイヤーが左側
			{
				pulling_power.x = pulling_power.x * -1;
			}
		}

		anchor_body->SetLinearVelocity(pulling_power);
		body->SetLinearVelocity(b2Vec2(pulling_power.x/20, 0.0f));

		pulling_flag = false;
	}

}

void boss_pillar::Draw()
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

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale*2 ,GetSize().y * scale }
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

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


				//draw
				DrawSplittingSprite(
					{ body_draw_x,
					body_draw_y },
					boss_pillar_body_Splitting[i]->GetAngle(),
					{ GetSize().x / Splitting_x * scale*2,GetSize().y / Splitting_y * scale },
					Splitting_x,
					Splitting_y,
					i,
					1.0f
				);

			}
		}
	}

}

void boss_pillar::Finalize()
{
	if (g_Texture) {
		UnInitTexture(g_Texture);
	}
}