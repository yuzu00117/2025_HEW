//-----------------------------------------------------------------------------------------------------
// #name one-way_platform.cpp
// #description 足場のオブジェクトのヘッダー
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//			・あるなら右だけに倒れたいとかあったらやるかもねー
//----------------------------------------------------------------------------------------------------
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"one-way_platform.h"
#include"create_filter.h"



//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_one_way_platform_Texture = NULL;//足場ブロックのテクスチャ
static ID3D11ShaderResourceView* g_one_way_platform1_Texture = NULL;//足場ブロックテクスチャ
one_way_platform::one_way_platform(b2Vec2 Postion,b2Vec2 local_Postion, b2Vec2 size)
{
	SetSize(size);

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//一つ目のボディをつくる

	//サイズを設定する
	b2Vec2 one_way_platform_size;
	one_way_platform_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	one_way_platform_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	//localpostionの調整

	local_Postion.x=local_Postion.x / BOX2D_SCALE_MANAGEMENT;
	local_Postion.y=local_Postion.y / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef one_way_platform_body;//木の幹の部分
	one_way_platform_body.type = b2_staticBody;
	one_way_platform_body.position.Set(Postion.x+local_Postion.x, Postion.y+local_Postion.y);//ローカルの補正もいれる
	one_way_platform_body.fixedRotation = false;

	b2Body* m_one_way_platform_body = world->CreateBody(&one_way_platform_body);

	SetObject_one_way_platform_Body(m_one_way_platform_body);//ボディを登録


	b2PolygonShape one_way_platform_shape;
	one_way_platform_shape.SetAsBox(one_way_platform_size.x * 0.5, one_way_platform_size.y * 0.5);//シャープを設定

	b2FixtureDef one_way_platform_fixture;

	one_way_platform_fixture.shape = &one_way_platform_shape;//シャープを登録
	one_way_platform_fixture.density = 3.0f;
	one_way_platform_fixture.friction = 0.5f;//摩擦
	one_way_platform_fixture.restitution = 0.0f;//反発係数
	one_way_platform_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	one_way_platform_fixture.filter = createFilterExclude("one-way_platform_filter", {"object_filter"});
	

	b2Fixture* object_one_way_platform_fixture = m_one_way_platform_body->CreateFixture(&one_way_platform_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_one_way_platform_data = new ObjectData{ collider_ground };//一旦壁判定
	object_one_way_platform_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_one_way_platform_data);

	object_one_way_platform_data->object_name = Object_one_way_platform;//オブジェクトの名前を定義しておく


	int ID = object_one_way_platform_data->GenerateID();//IDを取得して
	object_one_way_platform_data->id = ID;//フィクスチャにIDを定義
	SetID(ID);//クラスにIDを定義


};

one_way_platform::~one_way_platform()
{
}


void one_way_platform::Initialize()
{
	//視覚的に今センサーなのか定義している
	//テクスチャのインクルード
	g_one_way_platform_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_one_way_platform.png");//センサーがオフ　あたり判定あり
	g_one_way_platform1_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_one_way_platform1.png");//センサーがオン　あたり判定無し
}

void one_way_platform::Update()
{
	//プレイヤーより高いいちにあるとセンサーをオフに
	//オブジェクトに関してすべて貫通
	if (PlayerPosition::GetPlayerPosition().y+1/BOX2D_SCALE_MANAGEMENT < GetObject_one_way_platform_Body()->GetPosition().y)
	{
		if (GetObject_one_way_platform_Body()->GetFixtureList()->IsSensor() == true)//センサーをがオンいわゆる判定無しの場合
		{
			GetObject_one_way_platform_Body()->GetFixtureList()->SetSensor(false);//センサーをオフに
		}
	}
	else
	{
		if (GetObject_one_way_platform_Body()->GetFixtureList()->IsSensor() == false)//センサーをがオンいわゆる判定無しの場合
		{
			GetObject_one_way_platform_Body()->GetFixtureList()->SetSensor(true);//センサーをオフに
		}
	}
}


void one_way_platform::Draw()
{
	///ここから調整してね
	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;


	b2Vec2  one_way_platform_pos = GetObject_one_way_platform_Body()->GetPosition();

	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	float draw_x = ((one_way_platform_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((one_way_platform_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	if (GetObject_one_way_platform_Body()->GetFixtureList()->IsSensor() == false)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_one_way_platform_Texture);
	}
	else
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_one_way_platform1_Texture);
	}
	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObject_one_way_platform_Body()->GetAngle(),
		{ GetSize().x * scale,GetSize().y * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);
}

void one_way_platform::Finalize()
{
	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetObject_one_way_platform_Body() != nullptr)
	{
		//bodyを削除する
		world->DestroyBody(one_way_platform_body);
	}


	//テクスチャの解放
	UnInitTexture(g_one_way_platform_Texture);
	UnInitTexture(g_one_way_platform1_Texture);


}