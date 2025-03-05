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
#include"break_effect.h"

static ID3D11ShaderResourceView* g_Texture = NULL;//フィールドのテクスチャ
static ID3D11ShaderResourceView* g_border1 = NULL;
static ID3D11ShaderResourceView* g_border2 = NULL;
static ID3D11ShaderResourceView* g_border3 = NULL;

boss_pillar::boss_pillar(b2Vec2 position, b2Vec2 size, int splitting_x,int splitting_y,Boss_Room_Level level,int anchor_need_level)
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
	m_objectData = std::make_unique<ObjectData>(collider_object);
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());


	int ID = m_objectData->GenerateID();
	m_objectData->id = ID;
	m_objectData->object_name = Boss_pillar;
	m_objectData->need_anchor_level = anchor_need_level;
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


	//カスタムデータを作成
	m_anchorData = std::make_unique<ObjectData>(collider_anchor_point);
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_anchorData.get());



	m_anchorData->id = ID;


	m_anchorData->object_name = Boss_pillar;
	m_anchorData->need_anchor_level = anchor_need_level;

	b2Vec2 need_power;

	need_power.x = (GetSize().x * GetSize().y);//１は必要に応じて変更して
	need_power.y = 10.0f;//縦に必要な力はない


	m_anchorData->add_force = need_power;
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






	isUse = true;
}

boss_pillar::~boss_pillar()
{
	Finalize();
}



void boss_pillar::Initialize()
{
	if (g_Texture == NULL) {
		g_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\pillar.png");//柱のテクスチャ
		g_border1 = InitTexture(L"asset\\texture\\stage_1_1_object\\pillar_border_lv1.png");
		g_border2 = InitTexture(L"asset\\texture\\stage_1_1_object\\pillar_border_lv2.png");
		g_border3 = InitTexture(L"asset\\texture\\stage_1_1_object\\pillar_border_lv3.png");
	}
}

void boss_pillar::Update()
{
	if (isUse)
	{
		Boss_1_1& boss = Boss_1_1::GetInstance();
		if (boss_room_level < boss.GetBossFieldLevel() && (Splitting_end == false))
		{
			if (AnchorPoint::GetTargetAnchorPointBody() == m_body)
			{
				return;
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

			if (m_body)
			{
				PillarFragmentsManager::GetInstance().Destroy_Splitting_only(m_body->GetPosition(), g_Texture, GetSize(),GetSize().x,GetSize().y);
				for (b2Fixture* fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
					if (!fixture) continue;

					// UserData 取得


					// 無効なポインタならスキップ
					if (!fixture->GetUserData().pointer) {
						continue;
					}


					// ObjectData を削除す
					fixture->GetUserData().pointer = 0;  // ポインタのクリア
				}
				Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(m_body);
				m_body = nullptr;
			}

			if (AnchorPoint_body)
			{
				AnchorPoint::OutsideSensor(AnchorPoint_body);
				for (b2Fixture* fixture = AnchorPoint_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
					if (!fixture) continue;

					// UserData 取得


					// 無効なポインタならスキップ
					if (!fixture->GetUserData().pointer) {
						continue;
					}


					// ObjectData を削除す
					fixture->GetUserData().pointer = 0;  // ポインタのクリア
				}
				Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(AnchorPoint_body);
				AnchorPoint_body = nullptr;
			}


			Splitting_Destroy_Flag = false;
			Splitting_end = true;
		}
	}

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

			if(m_is_border)
			{
				switch (m_anchorData->need_anchor_level)
				{
				case 1:
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_border1);
					break;
				case 2:
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_border2);
					break;
				case 3:
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_border3);
					break;
				default:
					break;
				}
				DrawSprite(
					{ draw_x,
					  draw_y },
					GetBody()->GetAngle(),
					{ GetSize().x * scale * 2 * 1.1f ,GetSize().y * scale * 1.025f }
					,m_border_alpha
				);

				//透過率設定
				m_border_alpha -= 0.01;
				if (m_border_alpha <= m_border_alpha_min)
				{
					m_border_alpha = m_border_alpha_max;
				}
			}


			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale*2 ,GetSize().y * scale }
			);
		}


		
	}

}
void boss_pillar::Finalize()
{

	if (g_Texture) {
		UnInitTexture(g_Texture);
		UnInitTexture(g_border1);
		UnInitTexture(g_border2);
		UnInitTexture(g_border3);
	}

	if (m_body)
	{
		for (b2Fixture* fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
			if (!fixture) continue;

			// UserData 取得


			// 無効なポインタならスキップ
			if (!fixture->GetUserData().pointer) {
				continue;
			}




			// ObjectData を削除す
			fixture->GetUserData().pointer = 0;  // ポインタのクリア
		}
		Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(m_body);
		m_body = nullptr;
	}



	if (AnchorPoint_body)
	{
		for (b2Fixture* fixture = AnchorPoint_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
			if (!fixture) continue;

			// UserData 取得


			// 無効なポインタならスキップ
			if (!fixture->GetUserData().pointer) {
				continue;
			}




			// ObjectData を削除す
			fixture->GetUserData().pointer = 0;  // ポインタのクリア
		}
		Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(AnchorPoint_body);
		AnchorPoint_body = nullptr;
	}
	
}
