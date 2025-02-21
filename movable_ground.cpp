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
#include"gokai.h"
#include"hit_stop.h"
#include"camera_shake.h"


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Ground_Texture = NULL;//床のテクスチャ１
static ID3D11ShaderResourceView* g_Border_Texture_Lv1 = NULL;
static ID3D11ShaderResourceView* g_Border_Texture_Lv2 = NULL;
static ID3D11ShaderResourceView* g_Border_Texture_Lv3 = NULL;


bool	g_pulled = false;	//もう引っ張られたかどうかを取得（反発した瞬間で引っ張られた扱いになる）

int		g_need_level = 1;	//Updateで自分用にアンカーレベルを取っておく

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
	vertices[0].Set(-ground_size.x * 0.5f, -ground_size.y * 0.45f );	//左上
	vertices[1].Set(-ground_size.x * 0.42f, -ground_size.y * 0.45f);		//右上
	vertices[2].Set(-ground_size.x * 0.5f, ground_size.y * 0.45f);		//左下
	vertices[3].Set(-ground_size.x * 0.42f, ground_size.y*0.45f );		//右下
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
		Position.x+ground_size.x / 2,
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

	need_power.x = ((GetGroundSize().x * GetGroundSize().y) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 1.0;//１は必要に応じて変更して
	need_power.y = 10.0f;//縦に必要な力はない


	object_anchorpoint_data->add_force = need_power;
	m_pulling_power = object_anchorpoint_data->add_force;
	m_pulling_power = need_power;


	//アンカーレベルの設定
	object_anchorpoint_data->need_anchor_level = need_level;
	g_need_level = need_level;

	//アンカーレベルをメンバ変数で保持
	m_need_level = need_level;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	//ジョイントする

	b2WeldJointDef jointDef;
	jointDef.bodyA = p_Ground_body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(-ground_size.x * 0.5f, -ground_size.y * 0.5f); // 床の上端
	jointDef.localAnchorB.Set(0.0, anchorpoint_size.y * 0.5f); // アンカーポイントの下端
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
	g_Border_Texture_Lv1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");
	g_Border_Texture_Lv2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
	g_Border_Texture_Lv3 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
}

void movable_ground::Update()
{
	if (pulling)
	{
		Pulling_ground();

		int cunt_enemy = 0;		//敵何匹倒したかのカウント
		//敵を倒す処理
		if (Ground_body->GetLinearVelocity().x != 0.0f)
		{

			for (auto w : enemy_static)
			{
				w->CollisionPulledObject();
				app_atomex_start(Player_Dead_Sound);
				HitStop::StartHitStop(15);
				CameraShake::StartCameraShake(5, 3, 15);
				cunt_enemy++;
			}
			enemy_static.clear();

			for (auto w : enemy_dynamic)
			{
				w->CollisionPulledObject();
				app_atomex_start(Player_Dead_Sound);
				HitStop::StartHitStop(15);
				CameraShake::StartCameraShake(5, 3, 15);
				cunt_enemy++;
			}
			enemy_dynamic.clear();

			for (auto w : enemy_floating)
			{
				w->CollisionPulledObject();
				app_atomex_start(Player_Dead_Sound);
				HitStop::StartHitStop(15);
				CameraShake::StartCameraShake(5, 3, 15);
				cunt_enemy++;
			}
			enemy_floating.clear();
		}
		
		//豪快度関連
		//=================================================
		switch (g_need_level)
		{
		case 1:
			Gokai_UI::AddGokaiCount(100 * cunt_enemy);
			break;
		case 2:
			Gokai_UI::AddGokaiCount(500 * cunt_enemy);
			break;
		case 3:
			Gokai_UI::AddGokaiCount(1000 * cunt_enemy);
			break;
		default:
			break;
		}


		//反発しすぎないための処理
		//=============================================================================================
		if (Ground_body->GetLinearVelocity().x > 0)
		{
			Ground_body->SetLinearVelocity({ 0.0f,0.0f });
			g_pulled = true;
		}
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
	
	b2Vec2 size = GetGroundSize();

	if (m_is_border)
	{
		switch (m_need_level)
		{
		case 1:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Border_Texture_Lv1);
			break;
		case 2:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Border_Texture_Lv2);
			break;
		case 3:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Border_Texture_Lv3);
			break;
		default:
			break;
		}
		DrawSprite(
			{ draw_x,
			  draw_y },
			GetObjectAnchorPointBody()->GetAngle(),
			{ GetGroundSize().x * scale*1.1f,GetGroundSize().y * scale*1.1f }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
			, m_border_alpha
		);

		//透過率設定
		m_border_alpha -= 0.01;
		if (m_border_alpha <= m_border_alpha_min)
		{
			m_border_alpha = m_border_alpha_max;
		}
	}

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);

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
		UnInitTexture(g_Border_Texture_Lv1);
		UnInitTexture(g_Border_Texture_Lv2);
		UnInitTexture(g_Border_Texture_Lv3);

		g_Ground_Texture = NULL;
		g_Border_Texture_Lv1 = NULL;
		g_Border_Texture_Lv2 = NULL;
		g_Border_Texture_Lv3 = NULL;
	}


}

void movable_ground::Pulling_ground()
{
	b2Body* body = GetObjectAnchorPointBody();
	b2Vec2 pulling_power = m_pulling_power;

	//プレイヤー側に引っ張る
	if (PlayerPosition::GetPlayerPosition().x < body->GetPosition().x )//プレイヤーが左側
	{
		pulling_power.x = pulling_power.x * -8;
	}
	else
	{
		pulling_power.x = 0.0f;
	}

	body->SetLinearVelocity(pulling_power);

	//縁の描画終了
	m_is_border = false;
}

bool movable_ground::GetIfPulled()
{
	return g_pulled;
}

