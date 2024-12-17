#include "movable_ground.h"
#include"texture.h"
#include"collider_type.h"


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Wood_Texture = NULL;//木のテクスチャ１

int ObjectData::current_id = 0;


movable_ground::movable_ground(b2Vec2 position, b2Vec2 Ground_size, b2Vec2 AnchorPoint_size, int need_level)
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
	ground_size.y = Ground_size.y / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef Ground_body;//木の幹の部分
	Ground_body.type = b2_dynamicBody;
	Ground_body.position.Set(Postion.x, Postion.y);
	Ground_body.fixedRotation = false;

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
	wood_fixture.filter = createFilterExclude("object_filter", {});


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
		Postion.x,
		Postion.y + (wood_size.y / 2) + (anchorpoint_size.y / 2));
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

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

	object_anchorpoint_data->object_name = Object_Wood;


	int ID = object_anchorpoint_data->GenerateID();
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

	b2WeldJointDef jointDef;
	jointDef.bodyA = m_Wood_body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(0.0f, -wood_size.y * 0.5f); // 木の上端
	jointDef.localAnchorB.Set(0.0f, anchorpoint_size.y * 0.5f); // アンカーポイントの下端
	jointDef.collideConnected = false;					  //ジョイントした物体同士の接触を消す

	world->CreateJoint(&jointDef);						  //ワールドにジョイントを追加

	//-------------------------------------------------------------------------------------------
	//木を倒す為に必要な挙動

}

movable_ground::~movable_ground()
{
}

void movable_ground::Initialize()
{
}

void movable_ground::Update()
{
}

void movable_ground::Draw()
{
}

void movable_ground::Finalize()
{
}

void movable_ground::Pulling_ground(b2Vec2 pullingpower)
{
}
