//-----------------------------------------------------------------------------------------------------
// #name static_to_dynamic_block.cpp
// #description 岩のオブジェクトのCPP
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//        
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"player_position.h"
#include"static_to_dynamic_block.h"
#include"create_filter.h"
#include"collider_type.h"


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_BoxRock_Texture = NULL;//四角のコライダーのテクスチャ
static ID3D11ShaderResourceView* g_coconut_Texture = NULL;//丸のコライダーのテクスチャ





static_to_dynamic_block::static_to_dynamic_block(b2Vec2 Position, b2Vec2 size, collider_type_Box_or_Circle collider_type, int need_anchor_level)
{

	//サイズをセット
	SetSize(size);
	//コライダーをセット
	SetBox_or_Circle(collider_type);
	//フラグをセット
	Set_Change_Dynamic_flag(false);




	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//サイズの補正をする
	b2Vec2 object_size;
	object_size.x = size.x / BOX2D_SCALE_MANAGEMENT*0.95;
	object_size.y = size.y / BOX2D_SCALE_MANAGEMENT*0.95;

	b2BodyDef body;

	body.type = b2_staticBody;
	body.position.Set(Position.x, Position.y);
	body.fixedRotation = false;//回転する

	b2Body* m_body = world->CreateBody(&body);

	

	b2FixtureDef fixture;//フィクスチャを作成

	// クラス内に b2Shape をメンバーとして保持する場合の例
	b2PolygonShape shape; // クラスのメンバー変数として保持
	b2CircleShape circleShape;

	if (collider_type == Box_collider)
	{
		shape.SetAsBox(object_size.x * 0.5, object_size.y * 0.5);
		fixture.shape = &shape; // メンバー変数を使用
	}
	else if (collider_type == Circle_collider)
	{
		circleShape.m_radius = object_size.x * 0.5;
		fixture.shape = &circleShape; // メンバー変数を使用
	}

	fixture.density = 3.0f;//密度
	fixture.friction = 0.3f;//摩擦
	fixture.restitution = 0.0f;//反発係数
	fixture.isSensor = false;//センサーかどうか
	fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* m_fixture = m_body->CreateFixture(&fixture);//フィクスチャをボディに登録

	SetObjectBody(m_body);//ボディを登録する

	ObjectData* object_data = new ObjectData{ collider_object };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);
	object_data->need_anchor_level = need_anchor_level;
	////--------------------------------------------------------------------------------------------------
	//アンカーポイントのフィクスチャ
	b2FixtureDef fixture_anchorpoint;

	// クラス内に b2Shape をメンバーとして保持する場合の例
	b2PolygonShape shape_anchorpoint; // クラスのメンバー変数として保持
	b2CircleShape circleShape_anchorpoint;

	if (collider_type == Box_collider)
	{
		shape_anchorpoint.SetAsBox(object_size.x * 0.5*0.5, object_size.y * 0.5*0.5);
		fixture_anchorpoint.shape = &shape_anchorpoint; // メンバー変数を使用
	}
	else if (collider_type == Circle_collider)
	{
		circleShape_anchorpoint.m_radius = object_size.x * 0.5*0.5;
		fixture_anchorpoint.shape = &circleShape_anchorpoint; // メンバー変数を使用
	}

	fixture_anchorpoint.density = 3.0f;//密度
	fixture_anchorpoint.friction = 0.5f;//摩擦
	fixture_anchorpoint.restitution = 0.0f;//反発係数
	fixture_anchorpoint.isSensor = false;//センサーかどうか
	fixture_anchorpoint.filter = createFilterExclude("object_filter", {});

	b2Fixture* m_anchorpoint_fixture = m_body->CreateFixture(&fixture_anchorpoint);


	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	m_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);


	int ID = object_anchorpoint_data->GenerateID();
	object_anchorpoint_data->id = ID;
	SetID(ID);

	object_anchorpoint_data->object_name = Object_Static_to_Dynamic;

	object_anchorpoint_data->need_anchor_level = need_anchor_level;


};

static_to_dynamic_block::~static_to_dynamic_block()
{

}


void static_to_dynamic_block::Initialize()
{
	if (g_coconut_Texture == NULL) {
		g_coconut_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_coconut.png");
		g_BoxRock_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_fall_rock.png");
	}

}

void static_to_dynamic_block::Update()
{
	//flagがオンなら動的に
	if (Get_Change_Dynamic_flag() == true)
	{
		b2Body* body = GetObjectBody();
		body->SetType(b2_dynamicBody);//動的に変更
	}

}
void static_to_dynamic_block::Change_dynamic()
{
	Set_Change_Dynamic_flag(true);
	SetIfPulling(true);
}


void static_to_dynamic_block::Draw()
{

	///ここから調整してね




	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 RockPos = GetObjectBody()->GetPosition();




	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((RockPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((RockPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	if (GetBox_or_Circle() == Box_collider)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_BoxRock_Texture);
	}
	else
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_coconut_Texture);
	}

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectBody()->GetAngle(),
		{ GetSize().x * scale,GetSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);




}

void static_to_dynamic_block::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetObjectBody() != nullptr)
	{
		//ボディの削除
		world->DestroyBody(object_body);
	}

	

	//テクスチャの解放
	UnInitTexture(g_BoxRock_Texture);
	UnInitTexture(g_coconut_Texture);

}

