//-----------------------------------------------------------------------------------------------------
// #name movable_ground.cpp
// #description 引っ張れる床のCPP
// #make 2024/12/26　王泳心
// #update 2024/12/26
// #comment 追加・修正予定
//         
//----------------------------------------------------------------------------------------------------
#include "movable_ground.h"
#include"texture.h"
#include"collider_type.h"
#include"create_filter.h"
#include"player_position.h"
#include"sprite.h"


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Ground_Texture = NULL;//床のテクスチャ１


movable_ground::movable_ground(b2Vec2 Position, b2Vec2 Ground_size, b2Vec2 AnchorPoint_size, int need_level)
{
	SetGroundSize(Ground_size);
	SetAnchorPointSize(AnchorPoint_size);



	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//一つ目のボディをつくる

	//サイズを設定する
	b2Vec2 ground_size;
	ground_size.x = Ground_size.x / BOX2D_SCALE_MANAGEMENT;
	ground_size.y = Ground_size.y / BOX2D_SCALE_MANAGEMENT * 0.995f;


	b2BodyDef ground_body;//床の部分
	ground_body.type = b2_dynamicBody;
	ground_body.position.Set(Position.x, Position.y);
	ground_body.fixedRotation = true;

	b2Body* p_Ground_body = world->CreateBody(&ground_body);

	SetObjectGroundBody(p_Ground_body);



	b2PolygonShape ground_shape;
	ground_shape.SetAsBox(ground_size.x * 0.48, ground_size.y * 0.5);
	
	b2FixtureDef ground_fixture;

	ground_fixture.shape = &ground_shape;
	ground_fixture.density = 1.0f;
	ground_fixture.friction = 0.3f;//摩擦
	ground_fixture.restitution = 0.0f;//反発係数
	ground_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	ground_fixture.filter = createFilterExclude("object_filter", {});

	//敵をけすかどうかのセンサー
	b2PolygonShape sensor_shape;
	b2Vec2 vertices[4];
	vertices[0].Set(-ground_size.x * 0.5f, -ground_size.y * 0.45f );
	vertices[1].Set( 0.0f , -ground_size.y * 0.45f);
	vertices[2].Set(-ground_size.x * 0.5f, ground_size.y * 0.45f);
	vertices[3].Set(0.0f , ground_size.y*0.45f );
	sensor_shape.Set(vertices, 4);	//センサーのローカル位置を変更

	b2FixtureDef sensor_fixture;

	sensor_fixture.shape = &sensor_shape;
	sensor_fixture.density = 1.0f;
	sensor_fixture.friction = 0.0f;//摩擦
	sensor_fixture.restitution = 0.0f;//反発係数
	sensor_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	sensor_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_ground_fixture = p_Ground_body->CreateFixture(&ground_fixture);
	b2Fixture* object_sensor_fixture = p_Ground_body->CreateFixture(&sensor_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_ground_data = new ObjectData{ collider_ground };//一旦壁判定
	ObjectData* object_sensor_data = new ObjectData{ collider_object_destroyer_of_enemy };//一旦壁判定
	object_ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_ground_data);
	object_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_sensor_data);
	object_ground_data->object_name = Object_Movable_Ground;

	int ID = object_ground_data->GenerateID();
	object_ground_data->id = ID;
	object_sensor_data->id = ID;
	SetID(ID);


	//---------------------------------------------------------------------------//
	//2つめのボディ　床の上のアンカーポイントをつくる

	//サイズの補正をいれる
	b2Vec2 anchorpoint_size;
	anchorpoint_size.x = AnchorPoint_size.x / BOX2D_SCALE_MANAGEMENT;
	anchorpoint_size.y = AnchorPoint_size.y / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef anchorpoint_body;//アンカーポイントの部分
	anchorpoint_body.type = b2_dynamicBody;
	anchorpoint_body.position.Set(
		Position.x,
		Position.y + (ground_size.y / 2) + (anchorpoint_size.y / 2));
	anchorpoint_body.fixedRotation = true;

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

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

	object_anchorpoint_data->object_name = Object_Movable_Ground;


	object_anchorpoint_data->id = id;

	//床を引っ張る時に必要になるForce とりあえずサイズに依存でつくる
	b2Vec2 need_power;

	need_power.x = ((GetGroundSize().x * GetGroundSize().y) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 1;//１は必要に応じて変更して
	need_power.y = 10.0f;//縦に必要な力はない


	object_anchorpoint_data->add_force = need_power;
	m_pulling_power = object_anchorpoint_data->add_force;
	m_pulling_power = need_power;


	//アンカーレベルの設定
	object_anchorpoint_data->need_anchor_level = need_level;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	//ジョイントする

	b2WeldJointDef jointDef;
	jointDef.bodyA = p_Ground_body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(-ground_size.x * 0.5f, -ground_size.y * 0.5f); // 床の上端
	jointDef.localAnchorB.Set(-ground_size.x * 0.5f, anchorpoint_size.y * 0.5f); // アンカーポイントの下端
	jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

	world->CreateJoint(&jointDef);						  //ワールドにジョイントを追加

	//-------------------------------------------------------------------------------------------
	//床を引っ張る為に必要な挙動

}

movable_ground::~movable_ground()
{
}

void movable_ground::Initialize()
{
	g_Ground_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
}

void movable_ground::Update()
{
	if (pulling)
	{
		Pulling_ground();

		for (auto w : enemy_static)
		{
			w->CollisionPulledObject();	
		}
		enemy_static.clear();

		for (auto w : enemy_dynamic)
		{
			w->CollisionPulledObject();
		}
		enemy_dynamic.clear();
	}
}


void movable_ground::Draw()
{
	///ここから調整してね




	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 GroundPos = GetObjectGroundBody()->GetPosition();




	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((GroundPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((GroundPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);

	b2Vec2 size = GetGroundSize();

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectAnchorPointBody()->GetAngle(),
		{ GetGroundSize().x * scale,GetGroundSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);

}

void movable_ground::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//ボディの削除
	world->DestroyBody(Ground_body);
	world->DestroyBody(AnchorPoint_body);

	//テクスチャの解放

	if (g_Ground_Texture != NULL)
	{
		UnInitTexture(g_Ground_Texture);
		g_Ground_Texture = NULL;

	}


}

void movable_ground::Pulling_ground()
{
	b2Body* body = GetObjectAnchorPointBody();
	b2Vec2 pulling_power = m_pulling_power;

	//プレイヤー側に引っ張る
	if (PlayerPosition::GetPlayerPosition().x < body->GetPosition().x )//プレイヤーが左側
	{
		pulling_power.x = pulling_power.x * -5;
	}
	else
	{
		pulling_power.x = 0.0f;
	}

	body->SetLinearVelocity(pulling_power);
}

