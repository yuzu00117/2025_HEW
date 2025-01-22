//-----------------------------------------------------------------------------------------------------
// #name wood.cpp
// #description 木のオブジェクトのCPP
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//			・あるなら右だけに倒れたいとかあったらやるかもねー
//----------------------------------------------------------------------------------------------------

#include"wood.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"create_filter.h"


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Wood_Texture = NULL;//木のテクスチャ１
static ID3D11ShaderResourceView* g_Wood_Texture1 = NULL;//木のテクスチャ２
static ID3D11ShaderResourceView* g_Wood_Texture2 = NULL;//木ののテクスチャ３
static ID3D11ShaderResourceView* g_Stump_Texture = NULL;//木の切り株のテクスチャ


int ObjectData::current_id = 0;

wood::wood(b2Vec2 Position, b2Vec2 Wood_size, b2Vec2 AnchorPoint_size,int need_level)
{
	b2Vec2 Stump_size;
	Stump_size.x = Wood_size.x * 1.5f;
	Stump_size.y = Wood_size.y * 0.2f;

	SetWoodSize(Wood_size);
	SetStumpSize(Stump_size);
	SetAnchorPointSize(AnchorPoint_size);



	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//一つ目のボディをつくる

	//サイズを設定する
	b2Vec2 wood_size;
	wood_size.x = Wood_size.x / BOX2D_SCALE_MANAGEMENT;
	wood_size.y = Wood_size.y / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef Wood_body;//木の幹の部分
	Wood_body.type = b2_dynamicBody;
	Wood_body.position.Set(Position.x, Position.y - (Stump_size.y / BOX2D_SCALE_MANAGEMENT) * 0.5f);
	Wood_body.fixedRotation = false;

	b2Body* m_Wood_body = world->CreateBody(&Wood_body);

	SetObjectWoodBody(m_Wood_body);



	b2PolygonShape Wood_shape;
	Wood_shape.SetAsBox(wood_size.x * 0.5, wood_size.y * 0.5);

	b2FixtureDef wood_fixture;

	wood_fixture.shape = &Wood_shape;
	wood_fixture.density = 3.0f;
	wood_fixture.friction = 0.5f;//摩擦
	wood_fixture.restitution = 0.0f;//反発係数
	wood_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	wood_fixture.filter = createFilterExclude("object_filter",{});


	b2Fixture* object_wood_fixture = m_Wood_body->CreateFixture(&wood_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_wood_data = new ObjectData{ collider_object };//一旦壁判定
	object_wood_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_wood_data);

	//---------------------------------------------------------------------------//
	//2つめのボディ　木の上のアンカーポイントをつくる

	//サイズの補正をいれる
	b2Vec2 anchorpoint_size;
	anchorpoint_size.x = AnchorPoint_size.x / BOX2D_SCALE_MANAGEMENT;
	anchorpoint_size.y = AnchorPoint_size.y / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef anchorpoint_body;//木の幹の部分
	anchorpoint_body.type = b2_dynamicBody;
	anchorpoint_body.position.Set(
		Position.x,
		Position.y + (wood_size.y / 2) + (anchorpoint_size.y / 2));
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
	anchorpoint_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える
	anchorpoint_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

	object_wood_data->object_name = Object_Wood;
	object_anchorpoint_data->object_name = Object_Wood;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	//ジョイントする

	b2WeldJointDef jointDef;
	jointDef.bodyA = m_Wood_body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(0.0f, -wood_size.y * 0.5f); // 木の上端
	jointDef.localAnchorB.Set(0.0f, anchorpoint_size.y * 0.5f); // アンカーポイントの下端
	jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

	world->CreateJoint(&jointDef);						  //ワールドにジョイントを追加

	//-------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------//
	//3つめのボディ　木の下に切り株をつくる

	//サイズの補正をいれる
	b2Vec2 stump_size;
	stump_size.x = Stump_size.x / BOX2D_SCALE_MANAGEMENT;
	stump_size.y = Stump_size.y / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef stump_body;//木の幹の部分
	stump_body.type = b2_dynamicBody;
	stump_body.position.Set(Position.x, Position.y + (wood_size.y * 0.5f) - (stump_size.y * 0.5f));
	stump_body.fixedRotation = false;

	auto m_stump_body = world->CreateBody(&stump_body);
	SetObjectStumpBody(m_stump_body);

	b2PolygonShape stump_shape;
	b2Vec2 vertices[4];
	vertices[0].Set(-stump_size.x * 0.25f, -stump_size.y * 0.5f);
	vertices[1].Set(stump_size.x * 0.25f, -stump_size.y * 0.5f);
	vertices[2].Set(-stump_size.x * 0.25f, stump_size.y * 0.2f);
	vertices[3].Set(stump_size.x * 0.25f, stump_size.y * 0.2f);
	stump_shape.Set(vertices, 4);	//センサーのローカル位置を変更


	b2FixtureDef stump_fixture;

	stump_fixture.shape = &stump_shape;
	stump_fixture.density = 20.0f;
	stump_fixture.friction = 0.5f;//摩擦
	stump_fixture.restitution = 0.0f;//反発係数
	stump_fixture.isSensor = false;//センサーかどうか、trueならあたり判定は消える


	b2Fixture* object_stump_fixture = GetObjectStumpBody()->CreateFixture(&stump_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_stump_data = new ObjectData{ collider_object };//一旦壁判定
	object_stump_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_stump_data);

	object_stump_data->object_name = Object_Wood;




	//ObjecrDataの　ID設定　や　他の設定
	//------------------------------------------------------------------------------------------------------------------------------------------
	int ID=object_anchorpoint_data->GenerateID();
	object_wood_data->id = ID;
	object_anchorpoint_data->id = ID;
	SetID(ID);

	//木を倒しす時に必要になるForce とりあえずサイズに依存でつくる
	b2Vec2 need_power;

	need_power.x = ((GetWoodSize().x * GetWoodSize().y) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 1;//１は必要に応じて変更して
	need_power.y = 10.0f;//縦に必要な力はない
	

	object_anchorpoint_data->add_force = need_power;


	//アンカーレベルの設定
	object_anchorpoint_data->need_anchor_level = need_level;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	//ジョイントする

	b2WeldJointDef jointDef2;
	jointDef2.bodyA = m_Wood_body;
	jointDef2.bodyB = m_stump_body;
	jointDef2.localAnchorA.Set(0.0f, wood_size.y * 0.5f + 0.15f); // 木の下端	(0.15は微調整)
	jointDef2.localAnchorB.Set(0.0f, stump_size.y * 0.5f - 0.1f); // 切り株の上端	(0.1は微調整)
	jointDef2.collideConnected = false;					  //ジョイントした物体同士の接触を消す

	auto joint = world->CreateJoint(&jointDef2);						  //ワールドにジョイントを追加
	SetWoodStumpJoint(joint);	//木を引っ張ったらこのjointを消せるように保存しておく
	//-------------------------------------------------------------------------------------------
	//木を倒す為に必要な挙動

};

wood::~wood()
{
}


void wood::Initialize()
{
	
	if (g_Wood_Texture == NULL) {
		g_Wood_Texture = InitTexture(L"asset\\texture\\wood_texture\\wood.png");
		g_Wood_Texture1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
		g_Wood_Texture2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");
		g_Stump_Texture = InitTexture(L"asset\\texture\\wood_texture\\wood_stump.png");
	}
}

void wood::Update()
{
	//切り株と本体のジョイントを消すフラグがオンになってる場合
	if (m_destory_joint)
	{
		b2Joint* joint = GetWoodStumpJoint();
		if (joint != nullptr)
		{
			//ワールドのインスタンスを持ってくる
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();
			world->DestroyJoint(joint);		//	ワールドからジョイントを消す
			SetWoodStumpJoint(nullptr);		//	自分が保持してるジョイントの情報を消す
		}
		m_destory_joint = false;	//フラグをオフにする
	}


	//listが空じゃない場合のみ音を出すかを判断
	if (!object_collided_when_falling.empty())
	{
		//後でlistの中身を消すためにイテレータを保存するためのlist
		std::list<std::list<ObjectCollided_WhenFalling*>::iterator> IteratorList;
		for (auto a : object_collided_when_falling)
		{
			a->count_down_to_play_sound--; //カウントダウン
			//カウントダウンが0の場合音を鳴らす
			if (a->count_down_to_play_sound == 0)
			{
				app_atomex_start(m_sound_FalledDown);	//音鳴らす
				auto id = std::find(object_collided_when_falling.begin(), object_collided_when_falling.end(), a);	//このオブジェのイテレータを取得
				IteratorList.push_back(id);	//後で消すためにイテレータリストに入れておく
			}
		}

		//イテレータリストが空じゃない場合
		if (!IteratorList.empty())
		{
			for (auto a : IteratorList)
			{
				object_collided_when_falling.erase(a);	//ぶつかったオブジェクトのリストに対応のイテレータを消す
			}
			IteratorList.clear();
			object_collided_when_falling.clear();
		}
	}
}

void wood::Pulling_wood(b2Vec2 pulling_power)
{
	b2Body*body=GetObjectAnchorPointBody();
	//プレイヤー側に倒す
	if (PlayerPosition::GetPlayerPosition().x < body->GetPosition().x)//プレイヤーが左側
	{
		pulling_power.x = pulling_power.x * -1;
	}

	body->SetLinearVelocity(pulling_power);
	SetIfPulling(true);
}



void wood::Add_CollidedObjectWhenFalling_List(b2Vec2 position)
{
	//もしlistが空なら
	if (object_collided_when_falling.empty())
	{
		goto add;	//listに追加
	}
	//listが空じゃない
	for (auto a : object_collided_when_falling)
	{
		//もし今ぶつかったオブジェクトが前にぶつかったオブジェクトのｙ座標がめっちゃ近い
		if ((unsigned)(position.y - a->position.y) < 0.5f)
		{
			a->count_down_to_play_sound = 1;	//音出すカウントダウンをリセット
			return;
		}
	}
	
	add:
		ObjectCollided_WhenFalling object;
		object.position = position;
		object.count_down_to_play_sound = 1;
		object_collided_when_falling.push_back(&object);

}

void wood::Draw()
{

	///ここから調整してね




	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;
	
	//切り株を描く
//--------------------------------------------------------------------------------------------------
// プレイヤー位置を考慮してスクロール補正を加える
//取得したbodyのポジションに対してBox2dスケールの補正を加える

	b2Vec2 Stump_pos = GetObjectStumpBody()->GetPosition();

	float draw_x = ((Stump_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((Stump_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Stump_Texture);

	//描画
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectStumpBody()->GetAngle(),
		{ GetStumpSize().x * scale,GetStumpSize().y * scale }
	);


	//本体を描く
//--------------------------------------------------------------------------------------------------
	b2Vec2 Wood_pos = GetObjectWoodBody()->GetPosition();
	b2Vec2 AnchorPoint_pos = GetObjectAnchorPointBody()->GetPosition();


	b2Vec2 Wood_size = GetWoodSize();
	b2Vec2 AnchorPoint_size = GetAnchorPointSize();
	// 木の中心をローカル座標から計算
	float woodLocalCenterX =0.0f;
	float woodLocalCenterY = 0.0f;

	// アンカーポイントの中心をローカル座標から計算
	float anchorLocalCenterX = 0.0f;
	float anchorLocalCenterY = 0.0f;

	// 回転角度（ラジアン）
	float angle = GetObjectWoodBody()->GetAngle();

	// 回転行列を適用してワールド座標を計算
	b2Vec2 woodWorldCenter(
		Wood_pos.x + woodLocalCenterX * cos(angle) - woodLocalCenterY * sin(angle),
		Wood_pos.y + woodLocalCenterX * sin(angle) + woodLocalCenterY * cos(angle)
	);

	b2Vec2 anchorWorldCenter(
		AnchorPoint_pos.x + anchorLocalCenterX * cos(angle) - anchorLocalCenterY * sin(angle),
		AnchorPoint_pos.y + anchorLocalCenterX * sin(angle) + anchorLocalCenterY * cos(angle)
	);

	// 木とアンカーポイントの中心位置を加重平均で計算
	float totalHeight = Wood_size.y + AnchorPoint_size.y;
	float centerX = (woodWorldCenter.x * Wood_size.y + anchorWorldCenter.x * AnchorPoint_size.y) / totalHeight;
	float centerY = (woodWorldCenter.y * Wood_size.y + anchorWorldCenter.y * AnchorPoint_size.y) / totalHeight;

	// 中心点
	b2Vec2 textureCenter(centerX, centerY);



	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
	draw_x = ((textureCenter.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	draw_y = ((textureCenter.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Wood_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectAnchorPointBody()->GetAngle(),
		{ GetWoodSize().x * scale,totalHeight * scale }///サイズを取得するすべがない　フィクスチャのポインターに追加しようかな？ってレベル
	);

}

void wood::Finalize()
{

	//ワールドのインスタンスを持ってくる
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetObjectWoodBody() != nullptr)
	{
		//ボディの削除
		world->DestroyBody(Wood_body);
	}

	if (GetObjectAnchorPointBody() != nullptr)
	{
		world->DestroyBody(AnchorPoint_body);
	}

	//テクスチャの解放
	UnInitTexture(g_Wood_Texture);
	UnInitTexture(g_Wood_Texture1);
	UnInitTexture(g_Wood_Texture2);
}