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


//テクスチャの入れ物
//グローバル変数
static ID3D11ShaderResourceView* g_Wood_Texture = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Wood_Texture1 = NULL;//アンカーのテクスチャ
static ID3D11ShaderResourceView* g_Wood_Texture2 = NULL;//アンカーのテクスチャ


int ObjectData::current_id = 0;

wood::wood(b2Vec2 Postion, b2Vec2 Wood_size, b2Vec2 AnchorPoint_size,bool left)
{

	SetWoodSize(Wood_size);
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
	Wood_body.position.Set(Postion.x, Postion.y);
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

	b2Fixture* object_wood_fixture = m_Wood_body->CreateFixture(&wood_fixture);

	// カスタムデータを作成して設定
	ObjectData* object_wood_data = new ObjectData{ collider_wall };//一旦壁判定
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


	int ID=object_anchorpoint_data->GenerateID();
	object_anchorpoint_data->id = ID;
	SetID(ID);

	//木を倒しす時に必要になるForce とりあえずサイズに依存でつくる
	b2Vec2 need_power;

	need_power.x = ((GetWoodSize().x * GetWoodSize().y) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 1;//１は必要に応じて変更して
	need_power.y = 10.0f;//縦に必要な力はない
	

	object_anchorpoint_data->add_force = need_power;
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

};

wood::~wood()
{
}


void wood::Initialize()
{
	//アンカーの錨の部分（日本語）
	g_Wood_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_wood.png");
	g_Wood_Texture1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
	g_Wood_Texture2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");

}

void wood::Update()
{


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
	float draw_x = ((textureCenter.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((textureCenter.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


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

}