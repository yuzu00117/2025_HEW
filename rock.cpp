//-----------------------------------------------------------------------------------------------------
// #name rock.cpp
// #description 岩のオブジェクトのCPP
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//			・あるなら右だけに倒れたいとかあったらやるかもねー
//----------------------------------------------------------------------------------------------------

#include"rock.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"create_filter.h"
#include"sound.h"
#include"easing.h"


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Rock_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Rock_Texture1 = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Rock_Texture2 = NULL;//アンカーのテクスチャ

static ID3D11ShaderResourceView* g_Rock_Texture_Lv1 = NULL;//岩のテクスチャLv1
static ID3D11ShaderResourceView* g_Rock_Border_Texture_Lv1 = NULL;//岩のテクスチャLv1(縁のみ)
static ID3D11ShaderResourceView* g_Rock_Texture_Lv2 = NULL;//岩のテクスチャLv1
static ID3D11ShaderResourceView* g_Rock_Border_Texture_Lv2 = NULL;//岩のテクスチャLv1(縁のみ)
static ID3D11ShaderResourceView* g_Rock_Texture_Lv3 = NULL;//岩のテクスチャLv1
static ID3D11ShaderResourceView* g_Rock_Border_Texture_Lv3 = NULL;//岩のテクスチャLv1(縁のみ)



rock::rock(b2Vec2 Position, float radius, int set_need_anchor_level,bool left)
{
	//ボディは一つで　フィクスチャを二つ付ける構造にする



	SetRockSize(b2Vec2(radius * 2, radius * 2));
	//SetAnchorPointSize(AnchorPoint_size);


	//radiusの補正をいれる
	radius = radius / BOX2D_SCALE_MANAGEMENT;


	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//一つ目のボディをつくる

	//サイズを設定する
	b2Vec2 rock_size;
	rock_size.x = radius * 2 / BOX2D_SCALE_MANAGEMENT;
	rock_size.y = radius * 2 / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef Rock_body;//木の幹の部分
	Rock_body.type = b2_dynamicBody;
	Rock_body.position.Set(Position.x, Position.y);
	Rock_body.fixedRotation = false;

	b2Body* m_Rock_body = world->CreateBody(&Rock_body);

	SetObjectAnchorPointBody(m_Rock_body);


	//岩のフィクスチャをつくる
	// 円形のシェイプを定義
	b2CircleShape circleShape;
	circleShape.m_radius = radius; // 円の半径を設定

	b2FixtureDef rock_fixture;

	rock_fixture.shape = &circleShape;
	rock_fixture.density = 3.0f;
	rock_fixture.friction = 0.5f;//摩擦
	rock_fixture.restitution = 0.0f;//反発係数
	rock_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	rock_fixture.filter=createFilterExclude("object_filter", {});

	b2Fixture* object_rock_fixture = m_Rock_body->CreateFixture(&rock_fixture);

	// カスタムデータを作成して設定
	// ユニークポインターを使って ObjectData を作成
	m_objectData = std::make_unique<ObjectData>(collider_object);
	object_rock_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());
	m_objectData->object_name = Object_Rock;


	//2つめのフィクスチャを　岩の中にアンカーポイントをつくる
	b2CircleShape anchorpoint_circlesShape;
	anchorpoint_circlesShape.m_radius = radius / 2;

	b2FixtureDef rock_anchorpoint_fixture;

	rock_anchorpoint_fixture.shape = &anchorpoint_circlesShape;
	rock_anchorpoint_fixture.density = 3.0f;
	rock_anchorpoint_fixture.friction = 0.1f;//摩擦
	rock_anchorpoint_fixture.restitution = 0.1f;//反発係数
	rock_anchorpoint_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	rock_anchorpoint_fixture.filter = createFilterExclude("object_filter", {});


	b2Fixture* object_rock_anchorpoint_fixture = m_Rock_body->CreateFixture(&rock_anchorpoint_fixture);

	// カスタムデータを作成して設定


	m_anchor_point_objectData = std::make_unique<ObjectData>(collider_anchor_point);
	object_rock_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_anchor_point_objectData.get());

	m_anchor_point_objectData->object_name = Object_Rock;


	int ID = m_anchor_point_objectData->GenerateID();
	m_objectData->id = ID;
	m_anchor_point_objectData->id = ID;
	SetID(ID);
	m_objectData->need_anchor_level = set_need_anchor_level;
	m_anchor_point_objectData->need_anchor_level = set_need_anchor_level;

	//アンカーレベルをメンバ変数で保持
	m_need_level = set_need_anchor_level;
	


	//岩を倒しす時に必要になるForce とりあえずサイズに依存でつくる
	b2Vec2 need_power;
	need_power.x = ((radius * radius) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 10;//１は必要に応じて変更して
	need_power.y = 10.0f;//縦に必要な力はない

	m_anchor_point_objectData->add_force = need_power;
	m_pulling_power = need_power;


	//左右判定
	left_flag = left;

};

rock::~rock()
{
	Finalize();
}


void rock::Initialize()
{
	if (g_Rock_Texture == NULL) {
		g_Rock_Texture = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_big.png");
		g_Rock_Texture1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
		g_Rock_Texture2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");

		g_Rock_Texture_Lv1		   = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_big_lv1.png");
		g_Rock_Border_Texture_Lv1  = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_big_lv1_border.png");
		g_Rock_Texture_Lv2		   = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_big_lv2.png");
		g_Rock_Border_Texture_Lv2  = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_big_lv2_border.png");
		g_Rock_Texture_Lv3		   = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_big_lv3.png");
		g_Rock_Border_Texture_Lv3  = InitTexture(L"asset\\texture\\stage_1_1_object\\rock_big_lv3_border.png");
	}
}

void rock::Update()
{


}

void rock::Pulling_rock()
{
	b2Body* body = GetObjectAnchorPointBody();
	b2Vec2 pulling_power = m_pulling_power;

	//プレイヤー側に倒す
	if (left_flag)//プレイヤーが左側
	{
		pulling_power.x = pulling_power.x * -1;
	}

	body->SetLinearVelocity(pulling_power);
	SetIfPulling(true);

	//縁の描画終了
	m_is_border = false;

	//サウンドを再生
	app_atomex_start(Object_Rock_Roll_Sound);
}

void rock::Draw()
{

	///ここから調整してね




	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 RockPos = GetObjectAnchorPointBody()->GetPosition();




	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((RockPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((RockPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	if(m_is_border)
	{
		//アンカーレベルに応じた縁取りを付けたテクスチャを設定
		switch (m_need_level)
		{
		case 1:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Rock_Border_Texture_Lv1);
			break;
		case 2:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Rock_Border_Texture_Lv2);
			break;
		case 3:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Rock_Border_Texture_Lv3);
			break;
		default:
			break;
		}
		//縁を描画
		DrawSprite(
			{ draw_x,
			  draw_y },
			GetObjectAnchorPointBody()->GetAngle(),
			{ GetRockSize().x * scale * 1.1f,GetRockSize().y * scale * 1.1f }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
			, m_border_alpha
		);

		//透過率設定
		m_border_alpha -= 0.01;
		if (m_border_alpha <= m_border_alpha_min)
		{
			m_border_alpha = m_border_alpha_max;
		}
	}

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Rock_Texture);
	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectAnchorPointBody()->GetAngle(),
		{ GetRockSize().x * scale,GetRockSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);
}

void rock::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetObjectRockBody() != nullptr)
	{

		for (b2Fixture* fixture = Rock_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
			if (!fixture) continue;

			// UserData 取得


			// 無効なポインタならスキップ
			if (!fixture->GetUserData().pointer) {
				continue;
			}




			// ObjectData を削除す
			fixture->GetUserData().pointer = 0;  // ポインタのクリア
		}
		//ボディの削除
		world->DestroyBody(Rock_body);
		Rock_body = nullptr;
	}

	if (GetObjectAnchorPointBody() != nullptr)
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
		//ボディの削除
		world->DestroyBody(AnchorPoint_body);
		AnchorPoint_body = nullptr;
	}

	if (g_Rock_Texture) UnInitTexture(g_Rock_Texture);
	if (g_Rock_Texture1) UnInitTexture(g_Rock_Texture1);
	if (g_Rock_Texture2) UnInitTexture(g_Rock_Texture2);

	if (g_Rock_Texture_Lv1) UnInitTexture(g_Rock_Texture_Lv1);
	if (g_Rock_Border_Texture_Lv1) UnInitTexture(g_Rock_Border_Texture_Lv1);
	if (g_Rock_Texture_Lv2) UnInitTexture(g_Rock_Texture_Lv2);
	if (g_Rock_Border_Texture_Lv2) UnInitTexture(g_Rock_Border_Texture_Lv2);
	if (g_Rock_Texture_Lv3) UnInitTexture(g_Rock_Texture_Lv3);
	if (g_Rock_Border_Texture_Lv3) UnInitTexture(g_Rock_Border_Texture_Lv3);

}