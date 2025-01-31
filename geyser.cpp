//-----------------------------------------------------------------------------------------------------
// #name geyser.cpp
// #description 間欠泉のオブジェクトのCPP
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//----------------------------------------------------------------------------------------------------

#include"geyser.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"create_filter.h"
#include"player.h"
#include"easing.h"



//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Geyser_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Geyser_Water_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Geyser_on_Rock_Texture = NULL;//アンカーのテクスチャ




geyser::geyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize, b2Vec2 RangeFlyWaterSize, int splitting_x, int splitting_y, Boss_Room_Level level)
{
	//ボディは一つで　フィクスチャを二つ付ける構造にする
	SetGeyserSize(GeyserSize);					//間欠泉のサイズをセット
	SetRangeFlyWaterSize(RangeFlyWaterSize);	//間欠泉のが噴き出す水の飛ぶ範囲


	SetFlag(false);



	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//一つ目のボディをつくる 間欠泉

	b2Vec2 geyser_size;

	geyser_size.x = GeyserSize.x / BOX2D_SCALE_MANAGEMENT;
	geyser_size.y = GeyserSize.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef geyser_body;//間欠泉本体のボディ
	geyser_body.type = b2_staticBody;
	geyser_body.position.Set(GeyserPosition.x, GeyserPosition.y);
	geyser_body.fixedRotation = true;//回転はしない

	b2Body* m_geyser_body = world->CreateBody(&geyser_body);

	SetGeyserBody(m_geyser_body);



	//間欠泉自体のフィクスチャ
	b2PolygonShape geyser_shape;
	geyser_shape.SetAsBox(geyser_size.x * 0.5, geyser_size.y * 0.5);

	b2FixtureDef geyser_fixture;
	geyser_fixture.shape = &geyser_shape;
	geyser_fixture.density = 3.0f;
	geyser_fixture.restitution = 0.0f;//反発係数
	geyser_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	geyser_fixture.filter = createFilterExclude("ground_filter", {});


	b2Fixture* object_geyser_fixture = m_geyser_body->CreateFixture(&geyser_fixture);

	// カスタムデータを作成して設定
	ObjectData* geyser_object_data = new ObjectData{ collider_object };
	object_geyser_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(geyser_object_data);


	//噴き出す水のフィクスチャを付ける


	//間欠泉の水のサイズのスケールの調整
	b2Vec2 range_fly_water_size;
	range_fly_water_size.x = RangeFlyWaterSize.x / BOX2D_SCALE_MANAGEMENT;
	range_fly_water_size.y = RangeFlyWaterSize.y / BOX2D_SCALE_MANAGEMENT;

	//間欠泉の水のフィクスチャ
	b2PolygonShape range_fly_water_shape;

	b2Vec2 vertices[4] = { b2Vec2(0.0f,0.0f) };

	vertices[0].Set(-range_fly_water_size.x / 2, -geyser_size.y / 2);//左上
	vertices[1].Set(range_fly_water_size.x / 2, -geyser_size.y / 2);//右上
	vertices[2].Set(range_fly_water_size.x / 2, -geyser_size.y / 2 - range_fly_water_size.y);//左下
	vertices[3].Set(-range_fly_water_size.x / 2, -geyser_size.y / 2 - range_fly_water_size.y);//右下

	range_fly_water_shape.Set(vertices, 4); // 頂点を指定して三角形を設定

	b2FixtureDef range_fly_water_fixture;
	range_fly_water_fixture.shape = &range_fly_water_shape;
	range_fly_water_fixture.density = 3.0f;
	range_fly_water_fixture.restitution = 0.0f;//反発係数
	range_fly_water_fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える
	range_fly_water_fixture.filter = createFilterExclude("object_filter", {});


	b2Fixture* object_range_fly_water_fixture = m_geyser_body->CreateFixture(&range_fly_water_fixture);

	// カスタムデータを作成して設定
	ObjectData* range_fly_water_object_data = new ObjectData{ collider_geyser_water };//設定を間欠泉の水に
	object_range_fly_water_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(range_fly_water_object_data);

	range_fly_water_object_data->object_name = Object_Geyser_Water;


	range_fly_water_object_data->id = GetID();

	boss_room_level = level;

	Splitting_x = splitting_x;
	Splitting_y = splitting_y;

	isUse = true;
};

geyser::~geyser()
{
}


void geyser::Initialize()
{
	//間欠泉周りのテクスチャ！！！（日本語）
	g_Geyser_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_gyaser.png");
	g_Geyser_Water_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_geyser_water.png");
	g_Geyser_on_Rock_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_geyser_on_rock.png");

}

void geyser::Update()
{
	if (isUse)
	{
		Boss_1_1& boss = Boss_1_1::GetInstance();
		if (boss_room_level < boss.GetBossFieldLevel() && (Splitting_end == false))
		{
			Splitting_Destroy_Flag = true;
		}


		if (geyser_body != nullptr)
		{
			JumpPlayer();
		}


		if (Splitting_end == true)
		{
			Destroy_Cnt++;
		}

		if (180 < Destroy_Cnt)//分解したあと破壊されるフラグ
		{
			DestroySplittedBodies(boss_geyser_body_Splitting);
			isUse = false;
		}

		Destroy_Splitting();
	}
}





void geyser::JumpPlayer()
{


	if (GetFlag() == true)
	{
		//上に上げる所
		easing_rate += 0.02;

		if (1.5 < easing_rate)
		{
			easing_rate = 1.5;
		}

		// イージングがかかった値を保存する変数
		double easingRate;
		easingRate = Ease::InCubic(easing_rate);


		Player::GetOutSidePlayerBody()->ApplyForceToCenter(b2Vec2(0.0f, -easingRate), true);
		//-------------------------------------------------------------------------------------------

		b2Vec2 vec = Player::GetOutSidePlayerBody()->GetLinearVelocity();


		Player::GetOutSidePlayerBody()->SetLinearVelocity(b2Vec2(vec.x / 2, vec.y));
	}
	else
	{
		easing_rate = 0.8;
	}
}



void geyser::Destroy_Splitting()
{
	if (Splitting_Destroy_Flag == true)//破壊のフラグがオンになっている
	{
		if (geyser_body != nullptr && Splitting_end == false)
		{
			//ワールドのインスタンスを持ってくる
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();


			//破壊されたpositionを取得


			//普通のボディも消す
			b2Vec2 Destroy_position = geyser_body->GetPosition();
			float angle = geyser_body->GetAngle(); // 元のボディの角度を取得
			b2Vec2 vec = geyser_body->GetLinearVelocity();
			float angle_vec = geyser_body->GetAngularVelocity();

			world->DestroyBody(geyser_body);
			geyser_body = nullptr;

			SetGeyserBody(nullptr);


		

			b2Vec2 size;
			size.x = geyser_size.x / BOX2D_SCALE_MANAGEMENT;
			size.y = geyser_size.y / BOX2D_SCALE_MANAGEMENT;



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
					boss_geyser_body_Splitting.push_back(fragment);

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
				}
			}

			Splitting_Destroy_Flag = false;
			Splitting_end = true;
		}




	}

}


void geyser::DestroySplittedBodies(std::vector<b2Body*>& bodyList) 
{
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

void geyser::Destroy_Joint()
{
	// ワールドのインスタンスを取得
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// ボディのジョイントリストを確認
	b2Body* body = GetGeyserBody();
	if (body == nullptr) {
		return; // ボディが無効なら何もしない
	}

	b2JointEdge* jointEdge = body->GetJointList();
	if (jointEdge == nullptr) {
		return; // ジョイントが存在しない場合は何もしない
	}

	// ジョイントを破棄
	world->DestroyJoint(jointEdge->joint);
}

void geyser::PullingOnRock()
{
	float player_pos_x = PlayerPosition::GetPlayerPosition().x;
	float rock_pos_x = GetGeyserOnRockBody()->GetPosition().x;

	float add_force = GetGeyserOnRockSize().x * GetGeyserOnRockSize().y * 10;

	if (player_pos_x < rock_pos_x)
	{
		add_force = add_force * -1;
	}


	GetGeyserOnRockBody()->ApplyForceToCenter(b2Vec2(add_force, 0.0f), true);
}


void geyser::Draw()
{
	if (isUse)
	{
		// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
		float scale = SCREEN_SCALE;

		// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;
		///ここから調整してね
		if (GetGeyserBody() != nullptr)
		{


			b2Vec2 geyser_pos = GetGeyserBody()->GetPosition();




			// プレイヤー位置を考慮してスクロール補正を加える
			//取得したbodyのポジションに対してBox2dスケールの補正を加える
			float draw_x = ((geyser_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((geyser_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Texture);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetGeyserBody()->GetAngle(),
				{ GetGeyserSize().x * scale,GetGeyserSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
			);


			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Water_Texture);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y - (GetGeyserSize().y / 2 * scale) - (GetRangeFlyWaterSize().y / 2 * scale) },
				GetGeyserBody()->GetAngle(),
				{ GetRangeFlyWaterSize().x * scale,GetRangeFlyWaterSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
			);
		}

		//分割後の描画
		if (Splitting_end == true)
		{

			for (int i = 0; i < Splitting_x * Splitting_y; i++)
			{


				b2Vec2 bodyPos = boss_geyser_body_Splitting[i]->GetPosition();


				float body_draw_x = ((bodyPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float body_draw_y = ((bodyPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Texture);


				//draw
				DrawSplittingSprite(
					{ body_draw_x,
					body_draw_y },
					boss_geyser_body_Splitting[i]->GetAngle(),
					{ GetGeyserSize().x / Splitting_x * scale,GetGeyserSize().y / Splitting_y * scale },
					Splitting_x,
					Splitting_y,
					i,
					1.0f
				);

			}
		}
	}
}




void geyser::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetGeyserBody() != nullptr)
	{
		//ボディの削除
		world->DestroyBody(geyser_body);
	}

	if (GetGeyserOnRockBody() != nullptr)
	{
		world->DestroyBody(geyser_on_rock_body);
	}
	//画像の解放
	UnInitTexture(g_Geyser_on_Rock_Texture);
	UnInitTexture(g_Geyser_Texture);
	UnInitTexture(g_Geyser_Water_Texture);
}