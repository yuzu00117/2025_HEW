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


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Geyser_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Geyser_Water_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Geyser_on_Rock_Texture = NULL;//アンカーのテクスチャ




geyser::geyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize,b2Vec2 RangeFlyWaterSize,b2Vec2 GeyserOnRockSize,int set_rock_need_anchor_level)
{
	//ボディは一つで　フィクスチャを二つ付ける構造にする
	SetGeyserSize(GeyserSize);					//間欠泉のサイズをセット
	SetRangeFlyWaterSize(RangeFlyWaterSize);	//間欠泉のが噴き出す水の飛ぶ範囲
	SetGeyserOnRockSize(GeyserOnRockSize);		//間欠泉の上に乗ってる岩のサイズ

	//フラグをセット
	Set_geyser_on_rock_flag(true);



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
	geyser_fixture.filter = createFilterExclude("object_filter", {});
	

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

	vertices[0].Set(- range_fly_water_size.x/ 2, -geyser_size.y / 2);//左下
	vertices[1].Set( range_fly_water_size.x / 2, -geyser_size.y / 2);//右下
	vertices[2].Set(range_fly_water_size.x / 2, -geyser_size.y / 2-range_fly_water_size.y);//左下
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
	ObjectData* range_fly_water_object_data = new ObjectData{collider_geyser_water };//設定を間欠泉の水に
	object_range_fly_water_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(range_fly_water_object_data);

	range_fly_water_object_data->object_name = Object_Geyser_Water;



	


	//岩のボディをつくる

	//岩のサイズのスケールの調整
	b2Vec2  geyser_on_rock_size;
	geyser_on_rock_size.x = GeyserOnRockSize.x / BOX2D_SCALE_MANAGEMENT;
	geyser_on_rock_size.y = GeyserOnRockSize.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef geyser_on_rock_body;
	geyser_on_rock_body.type = b2_dynamicBody;
	geyser_on_rock_body.position.Set(

		GeyserPosition.x,
		GeyserPosition.y - (geyser_size.y / 2) - (geyser_on_rock_size.y / 2)
	);
	geyser_on_rock_body.fixedRotation = false;//回転を固定しない

	b2Body* m_geyser_on_rock_body = world->CreateBody(&geyser_on_rock_body);
	SetGeyserOnRockBody(m_geyser_on_rock_body);


	b2PolygonShape geyser_on_rock_body_shape;
	geyser_on_rock_body_shape.SetAsBox(geyser_on_rock_size.x * 0.5, geyser_on_rock_size.y * 0.5 );

	b2FixtureDef geyser_on_rock_fixture;
	geyser_on_rock_fixture.shape = &geyser_on_rock_body_shape;
	geyser_on_rock_fixture.density = 1.0f;
	geyser_on_rock_fixture.friction = 0.05f;//摩擦
	geyser_on_rock_fixture.restitution = 0.0f;//反発係数
	geyser_on_rock_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	b2Fixture* object_geyser_on_rock_fixture = m_geyser_on_rock_body->CreateFixture(&geyser_on_rock_fixture);

	ObjectData* object_geyser_on_rock_data = new ObjectData{ collider_object };
	object_geyser_on_rock_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_geyser_on_rock_data);

	b2PolygonShape anchor_point_geyser_on_rock_body_shape;
	anchor_point_geyser_on_rock_body_shape.SetAsBox(geyser_on_rock_size.x * 0.5*0.5, geyser_on_rock_size.y * 0.5*0.5);

	b2FixtureDef anchor_point_geyser_on_rock_fixture;
	anchor_point_geyser_on_rock_fixture.shape = &anchor_point_geyser_on_rock_body_shape;
	anchor_point_geyser_on_rock_fixture.density = 1.0f;
	anchor_point_geyser_on_rock_fixture.friction = 0.05f;//摩擦
	anchor_point_geyser_on_rock_fixture.restitution = 0.0f;//反発係数
	anchor_point_geyser_on_rock_fixture.isSensor = true;//センサーかどうか、trueならあたり判定は消える

	b2Fixture* anchor_point_object_geyser_on_rock_fixture = m_geyser_on_rock_body->CreateFixture(&anchor_point_geyser_on_rock_fixture);

	// カスタムデータを作成して設
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	anchor_point_object_geyser_on_rock_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);


	int ID = object_anchorpoint_data->GenerateID();
	object_anchorpoint_data->id = ID;

	range_fly_water_object_data->id = ID;

	object_anchorpoint_data->object_name = Object_Geyser_on_Rock;
	SetID(ID);
	object_anchorpoint_data->need_anchor_level =set_rock_need_anchor_level;



	//-----------------------------------------------------------------------------------------------------------------------------------------
	//ジョイントする

	b2WeldJointDef jointDef;
	jointDef.bodyA = m_geyser_body;
	jointDef.bodyB = m_geyser_on_rock_body;
	jointDef.localAnchorA.Set(0.0f, -geyser_size.y * 0.5f); // 間欠泉のの上端
	jointDef.localAnchorB.Set(0.0f, geyser_on_rock_size.y * 0.5f); // 岩の下端
	jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

	world->CreateJoint(&jointDef);						  //ワールドにジョイントを追加
	//------------------------------------------------------------------------------------------


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
	if (Get_geyser_on_rock_flag() == false)
	{
		Destroy_Joint();
		PullingOnRock();
		Set_geyser_on_rock_flag(true);
	}
}


void geyser::JumpPlayer()
{
	if (Get_geyser_on_rock_flag() == false)
	{
		Player::GetOutSidePlayerBody()->ApplyForceToCenter(b2Vec2(0.0f, -100.f), true);
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


	GetGeyserOnRockBody()->ApplyForceToCenter(b2Vec2(add_force,0.0f), true);
}


void geyser::Draw()
{

	///ここから調整してね

	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

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
		  draw_y-(GetRangeFlyWaterSize().y*scale)},
		GetGeyserBody()->GetAngle(),
		{ GetRangeFlyWaterSize().x * scale,GetRangeFlyWaterSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);

	b2Vec2 geyser_on_rock_pos=GetGeyserOnRockBody()->GetPosition();

	float rock_draw_x = ((geyser_on_rock_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float rock_draw_y = ((geyser_on_rock_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_on_Rock_Texture);

	//draw
	DrawSprite(
		{ rock_draw_x,
		  rock_draw_y },
		GetGeyserOnRockBody()->GetAngle(),
		{ GetGeyserOnRockSize().x * scale,GetGeyserOnRockSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);



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