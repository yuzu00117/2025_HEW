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
#include"sound.h"
#include"break_effect.h"


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_BoxRock_Texture = NULL;//四角のコライダーのテクスチャ
static ID3D11ShaderResourceView* g_Dameged_BoxRock_Texture = NULL;//四角のコライダーのテクスチャ
static ID3D11ShaderResourceView* g_coconut_Texture = NULL;//丸のコライダーのテクスチャ







static_to_dynamic_block::static_to_dynamic_block(b2Vec2 Position, b2Vec2 size, collider_type_Box_or_Circle collider_type, int need_anchor_level,bool break_flag)
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
	body.fixedRotation = true;//回転する

	b2Body* m_body = world->CreateBody(&body);
	SetObjectBody(m_body);//ボディを登録する
	

	b2FixtureDef fixture;//フィクスチャを作成
	b2FixtureDef topFixture;
	b2FixtureDef bottomFixture;

	// クラス内に b2Shape をメンバーとして保持する場合の例
	b2PolygonShape shape; // クラスのメンバー変数として保持
	b2CircleShape circleShape;

	

	if (collider_type == Box_collider)
	{
		// 上半分のボックス
		b2PolygonShape topShape;
		topShape.SetAsBox(object_size.x * 0.5, object_size.y * 0.25, b2Vec2(0, -object_size.y * 0.25), 0);
		topFixture.shape = &topShape;
		topFixture.density = 6.0f;//密度  密度を上げた
		topFixture.friction = 0.3f;//摩擦
		topFixture.restitution = 0.0f;//反発係数
		topFixture.isSensor = false;//センサーかどうか
		topFixture.filter = createFilterExclude("ground_filter", {}); // ground_filter を適用
		b2Fixture* m_top_fixture = m_body->CreateFixture(&topFixture);

		if (m_top_fixture)
		{
			ObjectData* object_top_data = new ObjectData{ collider_ground };
			m_top_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_top_data);
			object_top_data->need_anchor_level = need_anchor_level;
		}

		// 下半分のボックス
		b2PolygonShape bottomShape;
		bottomShape.SetAsBox(object_size.x * 0.5, object_size.y * 0.25, b2Vec2(0, object_size.y * 0.25), 0);
		bottomFixture.shape = &bottomShape;
		bottomFixture.density = 6.0f;//密度  密度を上げた
		bottomFixture.friction = 0.3f;//摩擦
		bottomFixture.restitution = 0.0f;//反発係数
		bottomFixture.isSensor = false;//センサーかどうか
		bottomFixture.filter = createFilterExclude("object_filter", {}); // object_filter を適用
		b2Fixture* m_bottom_fixture = m_body->CreateFixture(&bottomFixture);

		if (m_bottom_fixture)
		{
			ObjectData* object_bottom_data = new ObjectData{ collider_object };
			m_bottom_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_bottom_data);
			object_bottom_data->need_anchor_level = need_anchor_level;
		}
	}
	else if (collider_type == Circle_collider)
	{
		circleShape.m_radius = object_size.x * 0.5;
		fixture.shape = &circleShape; // メンバー変数を使用
	}

	

	
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

	Break_Flag = break_flag;
};

static_to_dynamic_block::~static_to_dynamic_block()
{

}


void static_to_dynamic_block::Initialize()
{
	if (g_coconut_Texture == NULL) {
		g_coconut_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_down.png");

		g_BoxRock_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_down.png");
		g_Dameged_BoxRock_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_down_damege.png");
	}

}

void static_to_dynamic_block::Update()
{
	//flagがオンなら動的に
	if (Get_Change_Dynamic_flag() == true)
	{
		b2Body* body = GetObjectBody();
		body->SetType(b2_dynamicBody);//動的に変更

		//効果音の再生
		app_atomex_start(Object_Rock_Fall_Sound);

		Set_Change_Dynamic_flag(false);
	}


	if (Break_Flag == true)
	{
		const float BREAK_ACCELERATION_THRESHOLD = 5.0f; 
		b2Body* body = GetObjectBody();

		// 現在の速度を取得
		b2Vec2 currentVelocity = body->GetLinearVelocity();

		// 速度変化（加速度）を計算
		b2Vec2 velocityDiff = currentVelocity - prevVelocity;
		float acceleration = velocityDiff.Length();

		// 閾値を超えたら壊れる
		if (acceleration > BREAK_ACCELERATION_THRESHOLD) {
			PillarFragmentsManager::GetInstance().Destroy_Splitting(body,g_BoxRock_Texture, GetSize());

			SetObjectBody(nullptr);
			Break_Flag = false;
		}

		// 次のフレーム用に現在の速度を保存
		prevVelocity = currentVelocity;
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

	if (GetObjectBody() != nullptr)
	{


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
			if (Break_Flag == true)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Dameged_BoxRock_Texture);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_BoxRock_Texture);
			}
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
	if (g_BoxRock_Texture != NULL)
	{
		UnInitTexture(g_BoxRock_Texture);
		UnInitTexture(g_coconut_Texture);
		UnInitTexture(g_Dameged_BoxRock_Texture);


		g_BoxRock_Texture = NULL;
		g_coconut_Texture = NULL;
		g_Dameged_BoxRock_Texture = NULL;	

	}

}

